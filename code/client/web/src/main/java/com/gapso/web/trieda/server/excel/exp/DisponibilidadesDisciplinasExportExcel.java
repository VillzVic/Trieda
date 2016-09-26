package com.gapso.web.trieda.server.excel.exp;

import java.text.SimpleDateFormat;
import java.util.List;

import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;

import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.DisponibilidadeDisciplina;
import com.gapso.trieda.domain.DisponibilidadeProfessor;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
public class DisponibilidadesDisciplinasExportExcel extends AbstractExportExcel {	
	enum ExcelCellStyleReference {
		TEXT(6, 2);

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

	private Sheet sheet;
	private CellStyle[] cellStyles;
	private boolean removeUnusedSheets;
	private int initialRow;

	public DisponibilidadesDisciplinasExportExcel(Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension) {
		this(true, cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension);
	}

	public DisponibilidadesDisciplinasExportExcel(boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension) {
		super(true, ExcelInformationType.DISPONIBILIDADES_DISCIPLINAS.getSheetName(), cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension);

		this.cellStyles = new CellStyle[ExcelCellStyleReference.values().length];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 6;
	}

	@Override
	public String getFileName() {
		return getI18nConstants().disponibilidadesDisciplinas();
	}

	@Override
	protected String getPathExcelTemplate() {
		if ( fileExtension.equals("xlsx") )
		{
			return "/templateExport.xlsx";
		}
		else
			return "/templateExport.xls";
	}

	@Override
	protected String getReportName() {
		return getI18nConstants().disponibilidadesDisciplinas();
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel(Workbook workbook) {
		List<Disciplina> disciplinas = Disciplina.findByCenario(this.instituicaoEnsino,getCenario());
		
		if (this.removeUnusedSheets) {
			removeUnusedSheets(this.getSheetName(), workbook);
		}
		if (!disciplinas.isEmpty()) {
			sheet = workbook.getSheet(this.getSheetName());
			fillInCellStyles(sheet);
			int nextRow = this.initialRow;

			for (Disciplina disciplina : disciplinas) {
				for (DisponibilidadeDisciplina disp : disciplina.getDisponibilidades()) {
					for (Semanas diaSemana : disp.getDiasSemana())
					{
					nextRow = writeData(disciplina, disp, diaSemana, nextRow);
					}
				}
			}

			return true;
		}

		return false;
	}

	private int writeData(Disciplina disciplina, DisponibilidadeDisciplina disp, Semanas diaSemana, int row) {
		
		if (isXls()){
			Sheet newSheet = restructuringWorkbookIfRowLimitIsViolated(row,1,sheet);
			if (newSheet != null) {
				row = this.initialRow;
				sheet = newSheet;
			}
		}
		
		// CPF
		setCell(row,2,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],disciplina.getCodigo());
		// Dia
		setCell(row,3,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],diaSemana.name());

		SimpleDateFormat sdf = new SimpleDateFormat("HH:mm");
		// Horário Inicial
		setCell(row,4,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],sdf.format(disp.getHorarioInicio()));
		// Horário Final
		setCell(row,5,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],sdf.format(disp.getHorarioFim()));

		row++;
		return row;
	}

	private void fillInCellStyles(Sheet sheet) {
		for (ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values()) {
			cellStyles[cellStyleReference.ordinal()] = getCell(cellStyleReference.getRow(),cellStyleReference.getCol(),sheet).getCellStyle();
		}
	}
	
}
