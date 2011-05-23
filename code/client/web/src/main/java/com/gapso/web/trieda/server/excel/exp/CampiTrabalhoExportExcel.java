package com.gapso.web.trieda.server.excel.exp;

import java.util.List;

import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Professor;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class CampiTrabalhoExportExcel extends AbstractExportExcel {
	
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
	
	public CampiTrabalhoExportExcel(Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages) {
		super(cenario,i18nConstants,i18nMessages);
		this.cellStyles = new HSSFCellStyle[ExcelCellStyleReference.values().length];
		this.removeUnusedSheets = true;
		this.sheetName = ExcelInformationType.CAMPI_TRABALHO.getSheetName();
		this.initialRow = 6;
	}
	
	public CampiTrabalhoExportExcel(boolean removeUnusedSheets, Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages) {
		super(cenario,i18nConstants,i18nMessages);
		this.cellStyles = new HSSFCellStyle[ExcelCellStyleReference.values().length];
		this.removeUnusedSheets = removeUnusedSheets;
		this.sheetName = ExcelInformationType.CAMPI_TRABALHO.getSheetName();
		this.initialRow = 6;
	}
	
	@Override
	public String getFileName() {
		return getI18nConstants().campiTrabalho();
	}
	
	@Override
	protected String getPathExcelTemplate() {
		return "/templateExport.xls";
	}

	@Override
	protected String getReportName() {
		return getI18nConstants().campiTrabalho();
	}

	@Override
	protected boolean fillInExcel(HSSFWorkbook workbook) {
		
		List<Campus> campi = Campus.findByCenario(getCenario());
		boolean flag = false;
		for(Campus campus : campi) {
			if(!campus.getProfessores().isEmpty()) {
				flag = true;
				break;
			}
		}
		if(flag) {
			if (this.removeUnusedSheets) {
				removeUnusedSheets(this.sheetName, workbook);
			}
			HSSFSheet sheet = workbook.getSheet(this.sheetName);
			fillInCellStyles(sheet);
			int nextRow = this.initialRow;
			for(Campus campus : campi) {
				for (Professor professor : campus.getProfessores()) {
					nextRow = writeData(campus.getCodigo(), professor.getCpf(), professor.getNome(), nextRow, sheet);
				}
			}
		}

		return flag;
	}
	
	private int writeData(String campusCodigo, String cpf, String professorNome, int row, HSSFSheet sheet) {
		// Campus Código
		setCell(row,2,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()], campusCodigo);
		// CPF
		setCell(row,3,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()], cpf);
		// Nome
		setCell(row,4,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()], professorNome);
		
		row++;
		return row;
	}
	
	private void fillInCellStyles(HSSFSheet sheet) {
		for (ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values()) {
			cellStyles[cellStyleReference.ordinal()] = getCell(cellStyleReference.getRow(),cellStyleReference.getCol(),sheet).getCellStyle();
		}
	}
}

