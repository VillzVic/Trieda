package com.gapso.web.trieda.server.excel.exp;

import java.util.List;

import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;

import com.gapso.trieda.domain.Unidade;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;

public class UnidadesExportExcel extends AbstractExportExcel {
	
	enum ExcelCellStyleReference {
		TEXT(6,2);
		private int row;
		private int col;
		private ExcelCellStyleReference(int row, int col) {
			this.row = row;
			this.col = col;
		}
		public int getRow() {
			return row;
		}
		public int getCol() {
			return col;
		}
	}
	private HSSFCellStyle[] cellStyles;
	
	private boolean removeUnusedSheets;
	private String sheetName;
	private int initialRow;
	
	public UnidadesExportExcel() {
		this.cellStyles = new HSSFCellStyle[ExcelCellStyleReference.values().length];
		this.removeUnusedSheets = true;
		this.sheetName = ExcelInformationType.UNIDADES.getSheetName();
		this.initialRow = 6;
	}
	
	public UnidadesExportExcel(boolean removeUnusedSheets) {
		this.cellStyles = new HSSFCellStyle[ExcelCellStyleReference.values().length];
		this.removeUnusedSheets = removeUnusedSheets;
		this.sheetName = ExcelInformationType.UNIDADES.getSheetName();
		this.initialRow = 6;
	}
	
	@Override
	public String getFileName() {
		return "Unidades";
	}
	
	@Override
	protected String getPathExcelTemplate() {
		return "/templateExport.xls";
	}

	@Override
	protected String getReportName() {
		return "Unidades";
	}

	@Override
	protected boolean fillInExcel(HSSFWorkbook workbook) {
		List<Unidade> unidades = Unidade.findAll();
		
		if (!unidades.isEmpty()) {
			if (this.removeUnusedSheets) {
				removeUnusedSheets(this.sheetName,workbook);
			}
			HSSFSheet sheet = workbook.getSheet(this.sheetName);
			fillInCellStyles(sheet);
			int nextRow = this.initialRow;
			for (Unidade u : unidades) {
				nextRow = writeData(u,nextRow,sheet);
			}
			
			return true;
		}
		
		return false;
	}
	
	private int writeData(Unidade unidade, int row, HSSFSheet sheet) {
		// Codigo
		setCell(row,2,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],unidade.getCodigo());
		// Nome
		setCell(row,3,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],unidade.getNome());
		// Campus
		setCell(row,4,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],unidade.getCampus().getCodigo());
		
		row++;
		return row;
	}
	
	private void fillInCellStyles(HSSFSheet sheet) {
		for (ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values()) {
			cellStyles[cellStyleReference.ordinal()] = getCell(cellStyleReference.getRow(),cellStyleReference.getCol(),sheet).getCellStyle();
		}
	}
}