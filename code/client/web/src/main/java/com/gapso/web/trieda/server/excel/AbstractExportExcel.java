package com.gapso.web.trieda.server.excel;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFRichTextString;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.hssf.util.CellRangeAddress;

public abstract class AbstractExportExcel implements IExportExcel {
	
	protected List<String> errors;
	protected List<String> warnings;
	
	protected AbstractExportExcel() {
		errors = new ArrayList<String>();
		warnings = new ArrayList<String>();
	}
	
	protected abstract String getPathExcelTemplate();
	protected abstract String getReportName();
	protected abstract boolean fillInExcel(HSSFWorkbook workbook);

	@Override
	public HSSFWorkbook export() {
		errors.clear();
		warnings.clear();
		
		HSSFWorkbook workbook = null;
		try {
			workbook = getExcelTemplate(getPathExcelTemplate());
		} catch (IOException e) {
			e.printStackTrace();
			String msg = e.getMessage() + (e.getCause() != null ? e.getCause().getMessage() : ""); 
			Object params[] = new Object[] {getPathExcelTemplate(),getReportName(),msg};
			//errors.add(MessageBundleUtils.getMenssage("excel.msg.err.obterExcelTemplate",params));//TODO: msg de erro
		}
		
		export(workbook);
		
		return workbook;
	}

	@Override
	public boolean export(HSSFWorkbook workbook) {
		if (workbook != null) {
			errors.clear();
			warnings.clear();
								
			return fillInExcel(workbook);
		} else {
			Object params[] = new Object[] {getReportName()};
			//errors.add(MessageBundleUtils.getMenssage("excel.msg.err.invalidWorkbook",params));//TODO: msg de erro
		}
		
		return false;
	}

	@Override
	public List<String> getErrors() {
		return errors;
	}

	@Override
	public List<String> getWarnings() {
		return warnings;
	}

	protected void mergeCells(int rowI, int rowF, int colI, int colF, HSSFSheet sheet) {
		sheet.addMergedRegion(new CellRangeAddress(rowI-1,rowF-1,colI-1,colF-1));
	}
	
	protected void mergeCells(int rowI, int rowF, int colI, int colF, HSSFSheet sheet, HSSFCellStyle style) {
		for (int row = rowI; row <= rowF; row++) {
			for (int col = colI; col <= colF; col++) {
				getCell(row,col,sheet).setCellStyle(style);
			}
		}
		sheet.addMergedRegion(new CellRangeAddress(rowI-1,rowF-1,colI-1,colF-1));
	}
	
	protected void setCell(int row, int col, HSSFSheet sheet, Calendar date) {
		final HSSFCell cell = getCell(row,col,sheet);
		cell.setCellValue(date);
	}
	
	protected void setCell(int row, int col, HSSFSheet sheet, HSSFCellStyle style, int mes, int ano) {
		final HSSFCell cell = getCell(row,col,sheet);
		Calendar date = Calendar.getInstance();
		date.clear(); // utilizado para limpar todas as informações e não escrever no excel hh:mm:ss
		date.set(ano,mes-1,1);
		cell.setCellValue(date);
		cell.setCellStyle(style);
	}
	
	protected void setCell(int row, int col, HSSFSheet sheet, HSSFCellStyle style, Calendar date) {
		final HSSFCell cell = getCell(row,col,sheet);
		cell.setCellValue(date);
		cell.setCellStyle(style);
	}
	
	protected void setCell(int row, int col, HSSFSheet sheet, HSSFCellStyle style, Date date) {
		final HSSFCell cell = getCell(row,col,sheet);
		cell.setCellValue(date);
		cell.setCellStyle(style);
	}
	
	protected void setCell(int row, int col, HSSFSheet sheet, String value) {
		HSSFCell cell = getCell(row,col,sheet);
		cell.setCellValue(new HSSFRichTextString(value));//cell.setCellValue(value);
	}
	
	protected void setCell(int row, int col, HSSFSheet sheet, HSSFCellStyle style, String value) {
		HSSFCell cell = getCell(row,col,sheet);
		cell.setCellValue(new HSSFRichTextString(value));//cell.setCellValue(value);
		cell.setCellStyle(style);
	}
	
	protected void setCell(int row, int col, HSSFSheet sheet, double value) {
		HSSFCell cell = getCell(row,col,sheet);
		cell.setCellValue(value);
	}
	
	protected void setCell(int row, int col, HSSFSheet sheet, HSSFCellStyle style, double value) {
		HSSFCell cell = getCell(row,col,sheet);
		cell.setCellValue(value);
		cell.setCellStyle(style);
	}
	
	protected HSSFCell getCell(int row, int col, HSSFSheet sheet) {
		final HSSFRow hssfRow = getRow(row-1,sheet);
		final HSSFCell cell = getCell(hssfRow,(col-1));
		return cell;
	}
	
	private HSSFRow getRow(int index, HSSFSheet sheet) {
		HSSFRow row = sheet.getRow(index);
		if (row == null) {
			row = sheet.createRow(index);
		}
		return row;
	}
	
	private HSSFCell getCell(HSSFRow row, int col) {
		HSSFCell cell = row.getCell(col);
		if (cell == null) {
			cell = row.createCell(col);
		}
		return cell;
	}
	
	private HSSFWorkbook getExcelTemplate(String pathExcelTemplate) throws IOException {
		final InputStream inTemplate = ExportExcelServlet.class.getResourceAsStream(pathExcelTemplate);
		HSSFWorkbook workBook = null;
		try {
			workBook = new HSSFWorkbook(inTemplate);
		} catch (IOException e) {
			throw e;
		} finally {
			if (inTemplate != null) {
				inTemplate.close();
			}
		}
		return workBook;
	}
}