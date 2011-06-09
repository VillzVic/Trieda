package com.gapso.web.trieda.server.excel.exp;

import java.util.List;

import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.AreaTitulacao;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curso;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class CursoAreasTitulacaoExportExcel extends AbstractExportExcel {
	
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
	
	public CursoAreasTitulacaoExportExcel(Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages) {
		super(cenario,i18nConstants,i18nMessages);
		this.cellStyles = new HSSFCellStyle[ExcelCellStyleReference.values().length];
		this.removeUnusedSheets = true;
		this.sheetName = ExcelInformationType.CURSO_AREAS_TITULACAO.getSheetName();
		this.initialRow = 6;
	}
	
	public CursoAreasTitulacaoExportExcel(boolean removeUnusedSheets, Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages) {
		super(cenario,i18nConstants,i18nMessages);
		this.cellStyles = new HSSFCellStyle[ExcelCellStyleReference.values().length];
		this.removeUnusedSheets = removeUnusedSheets;
		this.sheetName = ExcelInformationType.CURSO_AREAS_TITULACAO.getSheetName();
		this.initialRow = 6;
	}
	
	@Override
	public String getFileName() {
		return HtmlUtils.htmlUnescape(getI18nConstants().cursosAreasTitulacao());
	}
	
	@Override
	protected String getPathExcelTemplate() {
		return "/templateExport.xls";
	}

	@Override
	protected String getReportName() {
		return HtmlUtils.htmlUnescape(getI18nConstants().cursosAreasTitulacao());
	}

	@Override
	protected boolean fillInExcel(HSSFWorkbook workbook) {
		List<Curso> cursos = Curso.findByCenario(getCenario());
		
		boolean empty = true;
		for(Curso curso : cursos) {
			if(!curso.getAreasTitulacao().isEmpty()) {
				empty = false;
				break;
			}
		}
		if(empty) {
			return false;
		}
	
		if(this.removeUnusedSheets) {
			removeUnusedSheets(this.sheetName,workbook);
		}
			
		HSSFSheet sheet = workbook.getSheet(this.sheetName);
		fillInCellStyles(sheet);
		
		int nextRow = this.initialRow;
		for(Curso curso : cursos) {
			for(AreaTitulacao area : curso.getAreasTitulacao()) {
				nextRow = writeData(curso, area,nextRow,sheet);
			}
		}
		
		return true;
	}
	
	private int writeData(Curso curso, AreaTitulacao areaTitulacao, int row, HSSFSheet sheet) {
		// Curso
		setCell(row,2,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],curso.getCodigo());
		// Area de Titulacao
		setCell(row,3,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],areaTitulacao.getCodigo());
		
		row++;
		return row;
	}
	
	private void fillInCellStyles(HSSFSheet sheet) {
		for (ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values()) {
			cellStyles[cellStyleReference.ordinal()] = getCell(cellStyleReference.getRow(),cellStyleReference.getCol(),sheet).getCellStyle();
		}
	}
}
