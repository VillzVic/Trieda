package com.gapso.web.trieda.server.excel.exp;

import java.util.List;

import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.ProfessorDisciplina;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class HabilitacoesProfessoresExportExcel extends AbstractExportExcel {
	
	enum ExcelCellStyleReference {
		TEXT(6,2),
		NUMBER_INT(6,5);
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
	
	public HabilitacoesProfessoresExportExcel(Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages) {
		this(true, cenario, i18nConstants, i18nMessages);
	}
	
	public HabilitacoesProfessoresExportExcel(boolean removeUnusedSheets, Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages) {
		super(cenario,i18nConstants,i18nMessages);
		this.cellStyles = new HSSFCellStyle[ExcelCellStyleReference.values().length];
		this.removeUnusedSheets = removeUnusedSheets;
		this.sheetName = ExcelInformationType.HABILITACAO_PROFESSORES.getSheetName();
		this.initialRow = 6;
	}
	
	@Override
	public String getFileName() {
		return getI18nConstants().habilitacaoProfessores();
	}
	
	@Override
	protected String getPathExcelTemplate() {
		return "/templateExport.xls";
	}

	@Override
	protected String getReportName() {
		return getI18nConstants().habilitacaoProfessores();
	}

	@Override
	protected boolean fillInExcel(HSSFWorkbook workbook) {
		List<ProfessorDisciplina> professoresDisciplinas = ProfessorDisciplina.findAll();
		
		if (!professoresDisciplinas.isEmpty()) {
			if (this.removeUnusedSheets) {
				removeUnusedSheets(this.sheetName,workbook);
			}
			HSSFSheet sheet = workbook.getSheet(this.sheetName);
			fillInCellStyles(sheet);
			int nextRow = this.initialRow;
			for (ProfessorDisciplina professorDisciplina : professoresDisciplinas) {
				nextRow = writeData(professorDisciplina,nextRow,sheet);
			}
			
			return true;
		}
		
		return false;
	}
	
	private int writeData(ProfessorDisciplina pd, int row, HSSFSheet sheet) {
		Professor professor = pd.getProfessor();
		// CPF
		setCell(row,2,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],professor.getCpf());
		// Nome
		setCell(row,3,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],professor.getNome());
		// Disciplina
		setCell(row,4,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],pd.getDisciplina().getCodigo());
		// Preferência
		setCell(row,5,sheet,cellStyles[ExcelCellStyleReference.NUMBER_INT.ordinal()],pd.getPreferencia());
		// Nota Desempenho
		setCell(row,6,sheet,cellStyles[ExcelCellStyleReference.NUMBER_INT.ordinal()],pd.getNota());
		
		row++;
		return row;
	}
	
	private void fillInCellStyles(HSSFSheet sheet) {
		for (ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values()) {
			cellStyles[cellStyleReference.ordinal()] = getCell(cellStyleReference.getRow(),cellStyleReference.getCol(),sheet).getCellStyle();
		}
	}
}

