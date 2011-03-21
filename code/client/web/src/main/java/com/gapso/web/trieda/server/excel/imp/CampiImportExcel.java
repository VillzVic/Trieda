package com.gapso.web.trieda.server.excel.imp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;

import com.gapso.web.trieda.shared.excel.ExcelInformationType;

public class CampiImportExcel extends AbstractImportExcel<CampiImportExcelBean> {
	
	static public final String CODIGO_COLUMN_NAME = "Código";
	static public final String NOME_COLUMN_NAME = "Nome";
	static public final String ESTADO_COLUMN_NAME = "Estado";
	static public final String MUNICIPIO_COLUMN_NAME = "Município";
	static public final String BAIRRO_COLUMN_NAME = "Bairro";
	static public final String CUSTO_CREDITO_COLUMN_NAME = "Custo Médio do Crédito (R$)";
	
	private List<String> headerColumnsNames;
	
	public CampiImportExcel() {
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
		for (ImportExcelError erro : syntacticErrorsMap.keySet()) {
			List<Integer> linhasComErro = syntacticErrorsMap.get(erro);
			//Object params[] = new Object[] {linhasComErro.toString(),erro.getColumnName()};//TODO:
			//getMensagens().add(MessageBundleUtils.getMenssage(erro.getErrorMsgKey(),params));//TODO:
		}
		
		return syntacticErrorsMap.isEmpty();
	}

	private boolean doLogicValidation(String sheetName, List<CampiImportExcelBean> sheetContent) {
		// TODO Auto-generated method stub
		return false;
	}

	private void updateDataBase(String sheetName, List<CampiImportExcelBean> sheetContent) {
		// TODO Auto-generated method stub
	}
}