package com.gapso.web.trieda.server.excel.imp;

import java.util.List;

import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;

import com.gapso.web.trieda.shared.excel.ExcelInformationType;

public class CampiImportExcel extends AbstractImportExcel<CampiImportExcelBean> {
	
	static public enum CampiExcelHeader {
		CODIGO("Código"),
		NOME("Nome"),
		ESTADO("Estado"),
		MUNICIPIO("Município"),
		BAIRRO("Bairro"),
		CUSTO_MEDIO_CREDITO("Custo Médio do Crédito (R$)");
		
		private final String columnName;

	    private CampiExcelHeader(String columnName) {
	        this.columnName = columnName;
	    }
	    
		public String getColumnName() {
			return columnName;
		}

		public boolean isColumnValid(String name) {
			return name.equals(columnName);
		}

		@Override
		public String toString() {
			return columnName;
		}
		
		static public String print() {
			StringBuffer headerToStr = new StringBuffer();
			CampiExcelHeader[] header = CampiExcelHeader.values();
			for (int i = 0; i < header.length-1; i++) {
				headerToStr.append(header[i].toString()+",");
			}
			headerToStr.append(header[header.length-1].toString());
			return headerToStr.toString();
		}
	}

	@Override
	protected boolean sheetMustBeProcessed(int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook) {
		String sheetName = workbook.getSheetName(sheetIndex);
		return ExcelInformationType.CAMPI.getSheetName().equals(sheetName);
	}

	@Override
	protected boolean isHeaderValid(HSSFRow header, int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook) {
		if (header != null) {
    		boolean[] columnStatus = new boolean[CampiExcelHeader.values().length];
    		
    		// para cada coluna do header a ser verificado
            for (int cellIndex = header.getFirstCellNum(); cellIndex <= header.getLastCellNum(); cellIndex++) {
            	HSSFCell cell = header.getCell(cellIndex);
            	if (cell != null && cell.getCellType() == HSSFCell.CELL_TYPE_STRING) {
	            	String columnName = cell.getRichStringCellValue().getString();
	                if (CampiExcelHeader.BAIRRO.isColumnValid(columnName)) {
	                	columnStatus[CampiExcelHeader.BAIRRO.ordinal()] = true;
	                } else if (CampiExcelHeader.CODIGO.isColumnValid(columnName)) {
	                	columnStatus[CampiExcelHeader.CODIGO.ordinal()] = true;
	                } else if (CampiExcelHeader.CUSTO_MEDIO_CREDITO.isColumnValid(columnName)) {
	                	columnStatus[CampiExcelHeader.CUSTO_MEDIO_CREDITO.ordinal()] = true;
	                } else if (CampiExcelHeader.ESTADO.isColumnValid(columnName)) {
	                	columnStatus[CampiExcelHeader.ESTADO.ordinal()] = true;
	                } else if (CampiExcelHeader.MUNICIPIO.isColumnValid(columnName)) {
	                	columnStatus[CampiExcelHeader.MUNICIPIO.ordinal()] = true;
	                } else if (CampiExcelHeader.NOME.isColumnValid(columnName)) {
	                	columnStatus[CampiExcelHeader.NOME.ordinal()] = true;
	                }
            	}
            }
            
            // verifica se todas as colunas necessárias foram encontradas no header
            boolean test = true;
            for (int i = 0; i < columnStatus.length; i++) {
            	test = test && columnStatus[i];
            }
            return test;
    	}
    	return false;
	}

	@Override
	protected CampiImportExcelBean createExcelBean(HSSFRow header, HSSFRow row, int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook) {
		return null;
	}

	@Override
	protected String getHeaderToString() {
		return CampiExcelHeader.print();
	}

	@Override
	protected void processSheetContent(String sheetName, List<CampiImportExcelBean> sheetContent) {
		
	}
}