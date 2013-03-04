package com.gapso.web.trieda.server.excel.exp;

import java.text.SimpleDateFormat;
import java.util.List;

import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Professor;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;
import com.ibm.icu.util.Calendar;

public class DisponibilidadesProfessoresExportExcel extends AbstractExportExcel {
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

	private CellStyle[] cellStyles;
	private boolean removeUnusedSheets;
	private int initialRow;

	public DisponibilidadesProfessoresExportExcel(Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension) {
		this(true, cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension);
	}

	public DisponibilidadesProfessoresExportExcel(boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension) {
		super(true, ExcelInformationType.DISPONIBILIDADES_PROFESSORES.getSheetName(), cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension);

		this.cellStyles = new CellStyle[ExcelCellStyleReference.values().length];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 6;
	}

	@Override
	public String getFileName() {
		return getI18nConstants().disponibilidadesProfessores();
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
		return getI18nConstants().disponibilidadesProfessores();
	}

	@Override
	protected boolean fillInExcel(Workbook workbook) {
		List<Professor> professores = Professor.findByCenario(this.instituicaoEnsino,getCenario());

		if (!professores.isEmpty()) {
			if (this.removeUnusedSheets) {
				removeUnusedSheets(this.getSheetName(), workbook);
			}

			Sheet sheet = workbook.getSheet(this.getSheetName());
			fillInCellStyles(sheet);
			int nextRow = this.initialRow;

			for (Professor professor : professores) {
				for (HorarioDisponivelCenario hdc : professor.getHorarios()) {
					nextRow = writeData(professor, hdc, nextRow, sheet);
				}
			}

			return true;
		}

		return false;
	}

	private int writeData(Professor professor, HorarioDisponivelCenario hdc, int row, Sheet sheet) {
		// CPF
		setCell(row,2,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],professor.getCpf());
		// Dia
		setCell(row,3,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],hdc.getDiaSemana().name());

		SimpleDateFormat sdf = new SimpleDateFormat("HH:mm");
		// Horário Inicial
		setCell(row,4,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],sdf.format(hdc.getHorarioAula().getHorario()));
		// Horário Final
		Calendar horarioFinal = Calendar.getInstance();
		horarioFinal.setTime(hdc.getHorarioAula().getHorario());
		horarioFinal.add(Calendar.MINUTE,hdc.getHorarioAula().getSemanaLetiva().getTempo());
		setCell(row,5,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],sdf.format(horarioFinal.getTime()));

		row++;
		return row;
	}

	private void fillInCellStyles(Sheet sheet) {
		for (ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values()) {
			cellStyles[cellStyleReference.ordinal()] = getCell(cellStyleReference.getRow(),cellStyleReference.getCol(),sheet).getCellStyle();
		}
	}
}