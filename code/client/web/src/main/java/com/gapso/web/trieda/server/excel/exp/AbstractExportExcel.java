package com.gapso.web.trieda.server.excel.exp;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.Iterator;
import java.util.List;
import java.util.Set;

import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFClientAnchor;
import org.apache.poi.hssf.usermodel.HSSFComment;
import org.apache.poi.hssf.usermodel.HSSFPatriarch;
import org.apache.poi.hssf.usermodel.HSSFRichTextString;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.ss.util.CellRangeAddress;

import com.gapso.trieda.domain.Cenario;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.google.gwt.dev.util.collect.HashSet;

public abstract class AbstractExportExcel implements IExportExcel {
	
	protected List<String> errors;
	protected List<String> warnings;
	private Cenario cenario;
	private TriedaI18nConstants i18nConstants;
	private TriedaI18nMessages i18nMessages;
	
	protected AbstractExportExcel(Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages) {
		this.errors = new ArrayList<String>();
		this.warnings = new ArrayList<String>();
		this.cenario = cenario;
		this.i18nConstants = i18nConstants;
		this.i18nMessages = i18nMessages;
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
		} catch (Exception e) {
			e.printStackTrace();
			String msg = e.getMessage() + (e.getCause() != null ? e.getCause().getMessage() : ""); 
			errors.add(getI18nMessages().excelErroObterExcelTemplate(getPathExcelTemplate(),getReportName(),msg));
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
	
	protected Cenario getCenario() {
		return cenario;
	}
	
	protected TriedaI18nConstants getI18nConstants() {
		return i18nConstants;
	}
	
	protected TriedaI18nMessages getI18nMessages() {
		return i18nMessages;
	}
	
	protected void autoSizeColumns(short firstColumn, short lastColumn, HSSFSheet sheet) {
		for (short col = firstColumn; col <= lastColumn; col++) {
			sheet.autoSizeColumn(col);
		}
	}
	
	protected void removeUnusedSheet(String unusedSheetName, HSSFWorkbook workbook) {
//		workbook.removeSheetAt(workbook.getSheetIndex(unusedSheetName));
	}
	
	protected void removeUnusedSheets(String usedSheetName, HSSFWorkbook workbook) {
		while (workbook.getNumberOfSheets() > 1) {
			int sheetIx = 0;
			while (workbook.getSheetName(sheetIx).equals(usedSheetName)) sheetIx++;
			workbook.removeSheetAt(sheetIx);
		}
	}
	
	protected void removeUnusedSheets(List<String> usedSheetsName, HSSFWorkbook workbook) {
		Set<HSSFSheet> usedSheets = new HashSet<HSSFSheet>();
		for (String usedSheetName : usedSheetsName) {
			HSSFSheet sheet = workbook.getSheet(usedSheetName);
			if (sheet != null) {
				usedSheets.add(sheet);
			}
		}
		
		int sheetIx = 0;
		while (workbook.getNumberOfSheets() > usedSheets.size()) {
			HSSFSheet sheet = workbook.getSheetAt(sheetIx++);
			if (!usedSheets.contains(sheet)) {
				workbook.removeSheetAt(sheetIx);
				sheetIx = 0;
			}
		}
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
	
	protected void setCell(int row, int col, HSSFSheet sheet, HSSFCellStyle style, HSSFPatriarch patriarch, String value, String comment) {
		HSSFCell cell = getCell(row,col,sheet);
		cell.setCellValue(new HSSFRichTextString(value));//cell.setCellValue(value);
		HSSFComment cellComment = cell.getCellComment();
		if (cellComment == null) {
			cellComment = patriarch.createComment(new HSSFClientAnchor(0,0,0,0,(short)col,row,(short)(col+3),(row+9)));
			cell.setCellComment(cellComment);
		}
		cellComment.setString(new HSSFRichTextString(comment));
		cell.setCellStyle(style);
	}
	
	protected void setCell(int row, int col, HSSFSheet sheet, HSSFCellStyle style, Iterator<HSSFComment> itExcelCommentsPool, String value, String comment) {
		HSSFCell cell = getCell(row,col,sheet);
		cell.setCellValue(new HSSFRichTextString(value));//cell.setCellValue(value);
		HSSFComment cellComment = cell.getCellComment();
		if (cellComment == null) {
			if (itExcelCommentsPool.hasNext()) {
				cellComment = itExcelCommentsPool.next();
				cell.setCellComment(cellComment);
				cellComment.setString(new HSSFRichTextString(comment));
			}
		} else {
			cellComment.setString(new HSSFRichTextString(comment));
		}
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