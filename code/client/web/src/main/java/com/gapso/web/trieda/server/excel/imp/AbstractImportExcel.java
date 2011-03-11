package com.gapso.web.trieda.server.excel.imp;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.poifs.filesystem.POIFSFileSystem;

public abstract class AbstractImportExcel<ExcelBeanType> implements IImportExcel {
	
	protected List<String> errors;
	protected List<String> warnings;
	
	protected AbstractImportExcel() {
		errors = new ArrayList<String>();
		warnings = new ArrayList<String>();
	}
	
	protected abstract boolean sheetMustBeProcessed(int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook);
	protected abstract boolean isHeaderValid(HSSFRow header, int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook);
	protected abstract ExcelBeanType createExcelBean(HSSFRow header, HSSFRow row, int sheetIndex, HSSFSheet sheet, HSSFWorkbook workbook);
	protected abstract String getHeaderToString();
	protected abstract void processSheetContent(String sheetName, List<ExcelBeanType> sheetContent);

	@Override
	public boolean load(String fileName, InputStream inputStream) {
		errors.clear();
		warnings.clear();
		
		Map<String,List<ExcelBeanType>> excelBeansMap = readInputStream(fileName,inputStream);
		for (Entry<String,List<ExcelBeanType>> entry : excelBeansMap.entrySet()) {
			processSheetContent(entry.getKey(),entry.getValue());
		}
		
		return errors.isEmpty();
	}

	@Override
	public List<String> getErrors() {
		return errors;
	}

	@Override
	public List<String> getWarnings() {
		return warnings;
	}
	
	private Map<String,List<ExcelBeanType>> readInputStream(String fileName, InputStream inputStream) {
		// [SheetName, List<ExcelBeanType>]
		Map<String,List<ExcelBeanType>> excelBeansMap = new HashMap<String,List<ExcelBeanType>>();
		
		try {
			POIFSFileSystem poifs = new POIFSFileSystem(inputStream);
			HSSFWorkbook workbook = new HSSFWorkbook(poifs);
			
			for (int sheetIndex = 0; sheetIndex < workbook.getNumberOfSheets(); sheetIndex++) {
				HSSFSheet sheet = workbook.getSheetAt(sheetIndex);
				
				// verifica se a aba deve ou não ser processada
				if (sheetMustBeProcessed(sheetIndex,sheet,workbook)) {
					List<ExcelBeanType> excelBeansList = new ArrayList<ExcelBeanType>();
					excelBeansMap.put(workbook.getSheetName(sheetIndex),excelBeansList);
					
					// procura cabeçalho
					int rowIndex = sheet.getFirstRowNum();
	                HSSFRow header = null; 
	                while ((rowIndex < sheet.getLastRowNum()) && !isHeaderValid((header = sheet.getRow(rowIndex++)),sheetIndex,sheet,workbook));
	                
	                if (header != null) {
	                	List<Integer> nullRows = new ArrayList<Integer>();
	                	
	                	// efetua a leitura dos dados do arquivo
	                    for (; rowIndex <= sheet.getLastRowNum(); rowIndex++) {
	                    	HSSFRow row = sheet.getRow(rowIndex);
	                    	if (row != null) {
	                    		excelBeansList.add(createExcelBean(header,row,sheetIndex,sheet,workbook));
	                    	} else {
	                    		nullRows.add(rowIndex);
	                    	}
	                    }
	                    
	                    // verifica se existem linhas nulas
	                    if (!nullRows.isEmpty()) {
	                    	Object params[] = new Object[] {nullRows.toString(),fileName};
	        				//errors.add(MessageBundleUtils.getMenssage("excel.msg.err.linhas.invalidas",params));//TODO: msg de erro
	                    }
	                } else {
	                	Object params[] = new Object[] {getHeaderToString(),fileName};
	                	//errors.add(MessageBundleUtils.getMenssage("excel.msg.err.planilha.sem.cabecalho",params));//TODO: msg de erro
	                }
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
			Object params[] = new Object[] {fileName,extraiMensagem(e)};
			//errors.add(MessageBundleUtils.getMenssage("excel.msg.err.arquivo.invalido",params));//TODO: msg de erro
		} finally {
			if (inputStream != null) {
				try {
					inputStream.close();
				} catch (IOException e) {
					errors.add(extraiMensagem(e));
					e.printStackTrace();
				}
			}
		}
		
		return excelBeansMap;
	}
	
	private String extraiMensagem(Exception e) {
		StringBuffer msg = new StringBuffer();
		msg.append(e.getMessage());
		if (e.getCause() != null) {
			msg.append(" " + e.getCause().getMessage());
		}
		return msg.toString();
	}
	
	protected String getCellValue(HSSFCell cell) {
		switch (cell.getCellType()) {
    		case HSSFCell.CELL_TYPE_STRING:
    			return cell.getRichStringCellValue().getString().trim();
    		
    		case HSSFCell.CELL_TYPE_NUMERIC:
    			return Double.toString(cell.getNumericCellValue());
    	}
		return null;
	}
}