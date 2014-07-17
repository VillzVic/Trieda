package com.gapso.web.trieda.server.excel.exp;

import java.util.List;

import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;

import com.gapso.trieda.domain.Aluno;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class AlunosDisciplinasCursadasExportExcel
	extends AbstractExportExcel
{
	enum ExcelCellStyleReference
	{
		TEXT(6, 2),
		NUMBER(6, 5);

		private int row;
		private int col;

		private ExcelCellStyleReference(int row, int col)
		{
			this.row = row;
			this.col = col;
		}

		public int getRow()
		{
			return row;
		}

		public int getCol()
		{
			return col;
		}
	}

	private CellStyle [] cellStyles;
	private boolean removeUnusedSheets;
	private int initialRow;

	public AlunosDisciplinasCursadasExportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( true, cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );
	}

	public AlunosDisciplinasCursadasExportExcel( boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		super( true, ExcelInformationType.ALUNOS_DISCIPLINAS_CURSADAS.getSheetName(), cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );

		this.cellStyles = new CellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 6;
	}

	@Override
	public String getFileName() {
		return getI18nConstants().alunosDisciplinasCursadas();
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
		return getI18nConstants().alunosDisciplinasCursadas();
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel( Workbook workbook )
	{
		List< Object[] > curriculosDisciplinasPreRequisitos
			= CurriculoDisciplina.findBy(instituicaoEnsino, getCenario(), null, null, null, null, null, "cursadoPor");

		if (this.removeUnusedSheets) {
			removeUnusedSheets(this.getSheetName(), workbook);
		}
		
		if (!curriculosDisciplinasPreRequisitos.isEmpty()) {
			Sheet sheet = workbook.getSheet(this.getSheetName());
			fillInCellStyles(sheet);
			int nextRow = this.initialRow;
			for (Object[] curriculoDisciplinaRequisito : curriculosDisciplinasPreRequisitos) {
				nextRow = writeData( (CurriculoDisciplina)curriculoDisciplinaRequisito[0], 
						(Aluno)curriculoDisciplinaRequisito[1], nextRow, sheet);
			}

			return true;
		}

		return false;
	}

	private int writeData(CurriculoDisciplina curriculoDisciplina, Aluno aluno, int row, Sheet sheet) {
		// Matricula Aluno
		setCell(row, 2, sheet,
				cellStyles[ExcelCellStyleReference.TEXT.ordinal()],
				aluno.getMatricula());
		// Codigo Curso
		setCell(row, 3, sheet,
				cellStyles[ExcelCellStyleReference.TEXT.ordinal()],
				curriculoDisciplina.getCurriculo().getCurso().getCodigo());
		// Codigo Curriculo
		setCell(row, 4, sheet,
				cellStyles[ExcelCellStyleReference.TEXT.ordinal()],
				curriculoDisciplina.getCurriculo().getCodigo());
		// Periodo
		setCell(row, 5, sheet,
				cellStyles[ExcelCellStyleReference.NUMBER.ordinal()],
				curriculoDisciplina.getPeriodo());
		// Codigo Disciplina
		setCell(row, 6, sheet,
				cellStyles[ExcelCellStyleReference.TEXT.ordinal()],
				curriculoDisciplina.getDisciplina().getCodigo());

		row++;
		return row;
	}

	private void fillInCellStyles(Sheet sheet) {
		for (ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference
				.values()) {
			cellStyles[cellStyleReference.ordinal()] = getCell(
					cellStyleReference.getRow(), cellStyleReference.getCol(),
					sheet).getCellStyle();
		}
	}
}
