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
import com.gapso.trieda.domain.Unidade;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class UnidadesImportExcel extends AbstractImportExcel<UnidadesImportExcelBean> {
	
	static public String CODIGO_COLUMN_NAME;
	static public String NOME_COLUMN_NAME;
	static public String CAMPUS_COLUMN_NAME;
	
	private List<String> headerColumnsNames;
	
	public UnidadesImportExcel(Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages) {
		super(cenario,i18nConstants,i18nMessages);
		resolveHeaderColumnNames();
		this.headerColumnsNames = new ArrayList<String>();
		this.headerColumnsNames.add(CODIGO_COLUMN_NAME);
		this.headerColumnsNames.add(NOME_COLUMN_NAME);
		this.headerColumnsNames.add(CAMPUS_COLUMN_NAME);
	}

	@Override
	protected boolean sheetMustBeProcessed(int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook) {
		String sheetName = workbook.getSheetName(sheetIndex);
		return ExcelInformationType.UNIDADES.getSheetName().equals(sheetName);
	}
	
	@Override
	protected List<String> getHeaderColumnsNames(int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook) {
		return this.headerColumnsNames;
	}

	@Override
	protected UnidadesImportExcelBean createExcelBean(HSSFRow header, HSSFRow row, int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook) {
		UnidadesImportExcelBean bean = new UnidadesImportExcelBean(row.getRowNum()+1);
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
					} else if (CAMPUS_COLUMN_NAME.equals(columnName)) {
						bean.setCodigoCampusStr(cellValue);
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
	public String getSheetName() {
		return ExcelInformationType.UNIDADES.getSheetName();
	}
	
	@Override
	protected void processSheetContent(String sheetName, List<UnidadesImportExcelBean> sheetContent) {
		if (doSyntacticValidation(sheetName,sheetContent) && doLogicValidation(sheetName,sheetContent)) {
			updateDataBase(sheetName,sheetContent);
		}
	}

	private boolean doSyntacticValidation(String sheetName, List<UnidadesImportExcelBean> sheetContent) {
		// map utilizado para associar um erro às linhas do arquivo onde o mesmo ocorre
		// [ImportExcelError -> Lista de linhas onde o erro ocorre]
		Map<ImportExcelError,List<Integer>> syntacticErrorsMap = new HashMap<ImportExcelError,List<Integer>>();

		for (UnidadesImportExcelBean bean : sheetContent) {
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

	private boolean doLogicValidation(String sheetName, List<UnidadesImportExcelBean> sheetContent) {
		// verifica se alguma unidade apareceu mais de uma vez no arquivo de entrada
		checkUniqueness(sheetContent);
		// verifica se há referência a algum campus não cadastrado
		checkNonRegisteredCampus(sheetContent);
		
		return getErrors().isEmpty();
	}

	private void checkUniqueness(List<UnidadesImportExcelBean> sheetContent) {
		// map com os códigos das unidades e as linhas em que a mesmo aparece no arquivo de entrada
		// [CódigoUnidade -> Lista de Linhas do Arquivo de Entrada]
		Map<String,List<Integer>> unidadeCodigoToRowsMap = new HashMap<String,List<Integer>>();
		
		// 
		for (UnidadesImportExcelBean bean : sheetContent) {
			List<Integer> rows = unidadeCodigoToRowsMap.get(bean.getCodigoStr());
			if (rows == null) {
				rows = new ArrayList<Integer>();
				unidadeCodigoToRowsMap.put(bean.getCodigoStr(),rows);
			}
			rows.add(bean.getRow());
		}
		
		// verifica se alguma unidade apareceu mais de uma vez no arquivo de entrada
		for (Entry<String,List<Integer>> entry : unidadeCodigoToRowsMap.entrySet()) {
			if (entry.getValue().size() > 1) {
				getErrors().add(getI18nMessages().excelErroLogicoUnicidadeViolada(entry.getKey(),entry.getValue().toString()));
			}
		}
	}
	
	private void checkNonRegisteredCampus(List<UnidadesImportExcelBean> sheetContent) {
		// [CódigoCampus -> Campus]
		Map<String,Campus> campiBDMap = Campus.buildCampusCodigoToCampusMap(Campus.findByCenario(getCenario()));
		
		List<Integer> rowsWithErrors = new ArrayList<Integer>();
		for (UnidadesImportExcelBean bean : sheetContent) {
			Campus campus = campiBDMap.get(bean.getCodigoCampusStr());
			if (campus != null) {
				bean.setCampus(campus);
			} else {
				rowsWithErrors.add(bean.getRow());
			}
		}
		
		if (!rowsWithErrors.isEmpty()) {
			getErrors().add(getI18nMessages().excelErroLogicoEntidadesNaoCadastradas(CAMPUS_COLUMN_NAME,rowsWithErrors.toString()));
		}
	}

	@Transactional
	private void updateDataBase(String sheetName, List<UnidadesImportExcelBean> sheetContent) {
		Map<String,Unidade> unidadesBDMap = Unidade.buildUnidadeCodigoToUnidadeMap(Unidade.findByCenario(getCenario()));
		
		for (UnidadesImportExcelBean unidadeExcel : sheetContent) {
			Unidade unidadeBD = unidadesBDMap.get(unidadeExcel.getCodigoStr());
			if (unidadeBD != null) {
				// update
				unidadeBD.setNome(unidadeExcel.getNomeStr());
				unidadeBD.setCampus(unidadeExcel.getCampus());
				
				unidadeBD.merge();
			} else {
				// insert
				Unidade newUnidade = new Unidade();
				newUnidade.setCodigo(unidadeExcel.getCodigoStr());
				newUnidade.setNome(unidadeExcel.getNomeStr());
				newUnidade.setCampus(unidadeExcel.getCampus());
				
				newUnidade.persist();
			}
		}
	}
	
	private void resolveHeaderColumnNames()
	{
		if ( CODIGO_COLUMN_NAME == null )
		{
			CODIGO_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().codigoUnidade() );
			NOME_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().nome() );
			CAMPUS_COLUMN_NAME = HtmlUtils.htmlUnescape( getI18nConstants().codigoCampus() );
		}
	}
}
