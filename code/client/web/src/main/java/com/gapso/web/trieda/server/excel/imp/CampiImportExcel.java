package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class CampiImportExcel extends AbstractImportExcel<CampiImportExcelBean> {
	
	static public String CODIGO_COLUMN_NAME;
	static public String NOME_COLUMN_NAME;
	static public String ESTADO_COLUMN_NAME;
	static public String MUNICIPIO_COLUMN_NAME;
	static public String BAIRRO_COLUMN_NAME;
	static public String CUSTO_CREDITO_COLUMN_NAME;
	
	private List<String> headerColumnsNames;
	
	public CampiImportExcel(Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages) {
		super(cenario,i18nConstants,i18nMessages);
		resolveHeaderColumnNames();
		this.headerColumnsNames = new ArrayList<String>();
		this.headerColumnsNames.add(CODIGO_COLUMN_NAME);
		this.headerColumnsNames.add(NOME_COLUMN_NAME);
		this.headerColumnsNames.add(ESTADO_COLUMN_NAME);
		this.headerColumnsNames.add(MUNICIPIO_COLUMN_NAME);
		this.headerColumnsNames.add(BAIRRO_COLUMN_NAME);
		this.headerColumnsNames.add(CUSTO_CREDITO_COLUMN_NAME);
	}

	@Override
	protected boolean sheetMustBeProcessed(int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook) {
		String sheetName = workbook.getSheetName(sheetIndex);
		return ExcelInformationType.CAMPI.getSheetName().equals(sheetName);
	}
	
	@Override
	protected List<String> getHeaderColumnsNames(int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook) {
		return this.headerColumnsNames;
	}

	@Override
	protected CampiImportExcelBean createExcelBean(HSSFRow header, HSSFRow row, int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook) {
		CampiImportExcelBean bean = new CampiImportExcelBean(row.getRowNum()+1);
        for (int cellIndex = row.getFirstCellNum(); cellIndex <= row.getLastCellNum(); cellIndex++) {
            HSSFCell cell = row.getCell(cellIndex);        	
        	if (cell != null) {
        		HSSFCell headerCell = header.getCell(cell.getColumnIndex());
        		if(headerCell != null){
        			String columnName = headerCell.getRichStringCellValue().getString();
					String cellValue = getCellValue(cell);
					if (CODIGO_COLUMN_NAME.equals(columnName)) {
						bean.setCodigoStr(cellValue);
					} else if (NOME_COLUMN_NAME.endsWith(columnName)) {
						bean.setNomeStr(cellValue);
					} else if (ESTADO_COLUMN_NAME.equals(columnName)) {
						bean.setEstadoStr(cellValue);
					} else if (MUNICIPIO_COLUMN_NAME.equals(columnName)) {
						bean.setMunicipioStr(cellValue);
					} else if (BAIRRO_COLUMN_NAME.equals(columnName)) {
						bean.setBairroStr(cellValue);
					} else if (CUSTO_CREDITO_COLUMN_NAME.equals(columnName)) {
						bean.setCustoMedioCreditoStr(cellValue);
					}
        		}
        	}
        }
		return bean;
	}

	@Override
	protected String getHeaderToString() {
		return this.headerColumnsNames.toString();
	}

	@Override
	protected void processSheetContent(String sheetName, List<CampiImportExcelBean> sheetContent) {
		if (doSyntacticValidation(sheetName,sheetContent)) {
			if (doLogicValidation(sheetName,sheetContent)) {
				updateDataBase(sheetName,sheetContent);
			}
		}
	}

	private boolean doSyntacticValidation(String sheetName, List<CampiImportExcelBean> sheetContent) {
		// map utilizado para associar um erro às linhas do arquivo onde o mesmo ocorre
		// [ImportExcelError -> Lista de linhas onde o erro ocorre]
		Map<ImportExcelError,List<Integer>> syntacticErrorsMap = new HashMap<ImportExcelError,List<Integer>>();

		for (CampiImportExcelBean bean : sheetContent) {
			List<ImportExcelError> errorsBean = bean.checkSyntacticErrors();
			for (ImportExcelError error : errorsBean) {
				List<Integer> rowsWithErrors = syntacticErrorsMap.get(error);
				if (rowsWithErrors == null) {
					rowsWithErrors = new ArrayList<Integer>();
					syntacticErrorsMap.put(error,rowsWithErrors);
				}
				rowsWithErrors.add(bean.getRow());
			}
		}
		
		// coleta os erros e adiciona os mesmos na lista de mensagens
		for (ImportExcelError error : syntacticErrorsMap.keySet()) {
			List<Integer> linhasComErro = syntacticErrorsMap.get(error);
			getErrors().add(error.getMessage(linhasComErro.toString(),getI18nMessages()));
		}
		
		return syntacticErrorsMap.isEmpty();
	}

	private boolean doLogicValidation(String sheetName, List<CampiImportExcelBean> sheetContent) {
		// verifica se algum campus apareceu mais de uma vez no arquivo de entrada
		checkUniqueness(sheetContent);
		
		return getErrors().isEmpty();
	}

	private void checkUniqueness(List<CampiImportExcelBean> sheetContent) {
		// map com os códigos dos campi e as linhas em que o mesmo aparece no arquivo de entrada
		// [CódigoCampus -> Lista de Linhas do Arquivo de Entrada]
		Map<String,List<Integer>> campusCodigoToRowsMap = new HashMap<String,List<Integer>>();
		
		// 
		for (CampiImportExcelBean bean : sheetContent) {
			List<Integer> rows = campusCodigoToRowsMap.get(bean.getCodigoStr());
			if (rows == null) {
				rows = new ArrayList<Integer>();
				campusCodigoToRowsMap.put(bean.getCodigoStr(),rows);
			}
			rows.add(bean.getRow());
		}
		
		// verifica se algum campus apareceu mais de uma vez no arquivo de entrada
		for (Entry<String,List<Integer>> entry : campusCodigoToRowsMap.entrySet()) {
			if (entry.getValue().size() > 1) {
				getErrors().add(getI18nMessages().excelErroLogicoUnicidadeViolada(entry.getKey(),entry.getValue().toString()));
			}
		}
	}

	@Transactional
	private void updateDataBase(String sheetName, List<CampiImportExcelBean> sheetContent) {
		List<Campus> campiBDList = Campus.findByCenario(getCenario());
		
		Map<String,Campus> campiBDMap = new HashMap<String,Campus>();
		for (Campus campus : campiBDList) {
			campiBDMap.put(campus.getCodigo(),campus);
		}
		
		for (CampiImportExcelBean campusExcel : sheetContent) {
			Campus campusBD = campiBDMap.get(campusExcel.getCodigoStr());
			if (campusBD != null) {
				// update
				campusBD.setNome(campusExcel.getNomeStr());
				campusBD.setEstado(campusExcel.getEstado());
				campusBD.setMunicipio(campusExcel.getMunicipioStr());
				campusBD.setBairro(campusExcel.getBairroStr());
				campusBD.setValorCredito(campusExcel.getCustoMedioCredito());
				
				campusBD.merge();
			} else {
				// insert
				Campus newCampus = new Campus();
				newCampus.setCenario(getCenario());
				newCampus.setCodigo(campusExcel.getCodigoStr());
				newCampus.setNome(campusExcel.getNomeStr());
				newCampus.setEstado(campusExcel.getEstado());
				newCampus.setMunicipio(campusExcel.getMunicipioStr());
				newCampus.setBairro(campusExcel.getBairroStr());
				newCampus.setValorCredito(campusExcel.getCustoMedioCredito());
				
				newCampus.persist();
			}
		}
	}
	
	private void resolveHeaderColumnNames() {
		if (CODIGO_COLUMN_NAME == null) {
			
			CODIGO_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().codigo());
			NOME_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().nome());
			ESTADO_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().estado());
			MUNICIPIO_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().municipio());
			BAIRRO_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().bairro());
			CUSTO_CREDITO_COLUMN_NAME = HtmlUtils.htmlUnescape(getI18nConstants().custoMedioCreditoExcel());
		}
	}
}