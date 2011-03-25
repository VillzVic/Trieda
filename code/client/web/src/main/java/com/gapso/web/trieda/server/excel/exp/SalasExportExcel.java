package com.gapso.web.trieda.server.excel.exp;

import java.util.List;

import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;

import com.gapso.trieda.domain.Sala;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class SalasExportExcel extends AbstractExportExcel {
	
	enum ExcelCellStyleReference {
		TEXT(6,2),
		NUMBER(6,7);
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
	
	public SalasExportExcel(TriedaI18nMessages i18nMessages) {
		super(i18nMessages);
		this.cellStyles = new HSSFCellStyle[ExcelCellStyleReference.values().length];
		this.removeUnusedSheets = true;
		this.sheetName = ExcelInformationType.SALAS.getSheetName();
		this.initialRow = 6;
	}
	
	public SalasExportExcel(boolean removeUnusedSheets, TriedaI18nMessages i18nMessages) {
		super(i18nMessages);
		this.cellStyles = new HSSFCellStyle[ExcelCellStyleReference.values().length];
		this.removeUnusedSheets = removeUnusedSheets;
		this.sheetName = ExcelInformationType.SALAS.getSheetName();
		this.initialRow = 6;
	}
	
	@Override
	public String getFileName() {
		return "Salas";
	}
	
	@Override
	protected String getPathExcelTemplate() {
		return "/templateCampi.xls";
	}

	@Override
	protected String getReportName() {
		return "Salas";
	}

	@Override
	protected boolean fillInExcel(HSSFWorkbook workbook) {
		List<Sala> salas = Sala.findAll();
		
		if (!salas.isEmpty()) {
			if (this.removeUnusedSheets) {
				removeUnusedSheets(this.sheetName,workbook);
			}
			HSSFSheet sheet = workbook.getSheet(this.sheetName);
			fillInCellStyles(sheet);
			int nextRow = this.initialRow;
			for (Sala s : salas) {
				nextRow = writeData(s,nextRow,sheet);
			}
			
			return true;
		}
		
		return false;
	}
	
	private int writeData(Sala sala, int row, HSSFSheet sheet) {
		// Codigo
		setCell(row,2,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],sala.getCodigo());
		// Tipo
		setCell(row,3,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],sala.getTipoSala().getNome());
		// Unidade
		setCell(row,4,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],sala.getUnidade().getCodigo());
		// Numero
		setCell(row,5,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],sala.getNumero());
		// Andar
		setCell(row,6,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],sala.getAndar());
		// Capacidade
		setCell(row,7,sheet,cellStyles[ExcelCellStyleReference.NUMBER.ordinal()],sala.getCapacidade());
		
		row++;
		return row;
	}
	
	private void fillInCellStyles(HSSFSheet sheet) {
		for (ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values()) {
			cellStyles[cellStyleReference.ordinal()] = getCell(cellStyleReference.getRow(),cellStyleReference.getCol(),sheet).getCellStyle();
		}
	}
}

