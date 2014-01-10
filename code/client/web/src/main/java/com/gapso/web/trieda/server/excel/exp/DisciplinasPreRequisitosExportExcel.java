package com.gapso.web.trieda.server.excel.exp;

import java.util.List;

import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
public class DisciplinasPreRequisitosExportExcel
	extends AbstractExportExcel
{
	enum ExcelCellStyleReference
	{
		TEXT(6, 2),
		NUMBER(6, 3);

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

	public DisciplinasPreRequisitosExportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( true, cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );
	}

	public DisciplinasPreRequisitosExportExcel( boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		super( true, ExcelInformationType.DISCIPLINAS_PRE_REQUISITOS.getSheetName(), cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );

		this.cellStyles = new CellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 6;
	}

	@Override
	public String getFileName() {
		return getI18nConstants().disciplinasPreRequisitos();
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
		return getI18nConstants().disciplinasPreRequisitos();
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel( Workbook workbook, Workbook templateWorkbook )
	{
		List< Object[] > curriculosDisciplinasPreRequisitos
			= CurriculoDisciplina.findBy(instituicaoEnsino, getCenario(), null, null, null, "preRequisitos");
		
		if (this.removeUnusedSheets) {
			removeUnusedSheets(this.getSheetName(), workbook);
		}
		if (!curriculosDisciplinasPreRequisitos.isEmpty()) {
			Sheet sheet = workbook.getSheet(this.getSheetName());
			if (isXls()) {
				fillInCellStyles(sheet);
			}
			else {
				Sheet templateSheet = templateWorkbook.getSheet(this.getSheetName());
				fillInCellStyles(templateSheet);
			}
			int nextRow = this.initialRow;
			for (Object[] curriculoDisciplinaRequisito : curriculosDisciplinasPreRequisitos) {
				nextRow = writeData( (CurriculoDisciplina)curriculoDisciplinaRequisito[0], 
						(Disciplina)curriculoDisciplinaRequisito[1], nextRow, sheet);
			}

			return true;
		}

		return false;
	}

	private int writeData(CurriculoDisciplina curriculoDisciplina, Disciplina disciplina, int row, Sheet sheet) {
		// Codigo Curriculo
		setCell(row, 2, sheet,
				cellStyles[ExcelCellStyleReference.TEXT.ordinal()],
				curriculoDisciplina.getCurriculo().getCodigo());
		// Periodo
		setCell(row, 3, sheet,
				cellStyles[ExcelCellStyleReference.NUMBER.ordinal()],
				curriculoDisciplina.getPeriodo());
		// Codigo Disciplina
		setCell(row, 4, sheet,
				cellStyles[ExcelCellStyleReference.TEXT.ordinal()],
				curriculoDisciplina.getDisciplina().getCodigo());
		// Disciplina Pre-Requisito
		setCell(row, 5, sheet,
				cellStyles[ExcelCellStyleReference.TEXT.ordinal()],
				disciplina.getCodigo());

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
