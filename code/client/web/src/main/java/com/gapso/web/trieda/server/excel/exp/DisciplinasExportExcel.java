package com.gapso.web.trieda.server.excel.exp;

import java.util.List;

import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class DisciplinasExportExcel
	extends AbstractExportExcel
{
	enum ExcelCellStyleReference
	{
		TEXT(6, 2),
		NUMBER(6, 4);

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

	private HSSFCellStyle [] cellStyles;
	private boolean removeUnusedSheets;
	private String sheetName;
	private int initialRow;

	public DisciplinasExportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino )
	{
		this( true, cenario, i18nConstants, i18nMessages, instituicaoEnsino );
	}

	public DisciplinasExportExcel( boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino )
	{
		super( cenario, i18nConstants, i18nMessages, instituicaoEnsino );

		this.cellStyles = new HSSFCellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.sheetName = ExcelInformationType.DISCIPLINAS.getSheetName();
		this.initialRow = 6;
	}

	@Override
	public String getFileName() {
		return getI18nConstants().disciplinas();
	}

	@Override
	protected String getPathExcelTemplate() {
		return "/templateExport.xls";
	}

	@Override
	protected String getReportName() {
		return getI18nConstants().disciplinas();
	}

	@Override
	protected boolean fillInExcel( HSSFWorkbook workbook )
	{
		List< Disciplina > disciplinas
			= Disciplina.findByCenario( this.instituicaoEnsino, getCenario() );

		if (!disciplinas.isEmpty()) {
			if (this.removeUnusedSheets) {
				removeUnusedSheets(this.sheetName, workbook);
			}
			HSSFSheet sheet = workbook.getSheet(this.sheetName);
			fillInCellStyles(sheet);
			int nextRow = this.initialRow;
			for (Disciplina disciplina : disciplinas) {
				nextRow = writeData(disciplina, nextRow, sheet);
			}

			return true;
		}

		return false;
	}

	private int writeData(Disciplina disciplina, int row, HSSFSheet sheet) {
		// Codigo
		setCell(row, 2, sheet,
				cellStyles[ExcelCellStyleReference.TEXT.ordinal()],
				disciplina.getCodigo());
		// Nome
		setCell(row, 3, sheet,
				cellStyles[ExcelCellStyleReference.TEXT.ordinal()],
				disciplina.getNome());
		// Créditos Teóricos
		setCell(row, 4, sheet,
				cellStyles[ExcelCellStyleReference.NUMBER.ordinal()],
				disciplina.getCreditosTeorico());
		// Créditos Práticos
		setCell(row, 5, sheet,
				cellStyles[ExcelCellStyleReference.NUMBER.ordinal()],
				disciplina.getCreditosPratico());
		// Usa Laboratório?
		setCell(row, 6, sheet,
				cellStyles[ExcelCellStyleReference.TEXT.ordinal()],
				(disciplina.getLaboratorio() ? getI18nConstants().sim()
						: HtmlUtils.htmlUnescape(getI18nConstants().nao())));
		// Tipo
		setCell(row, 7, sheet,
				cellStyles[ExcelCellStyleReference.TEXT.ordinal()], disciplina
						.getTipoDisciplina().getNome());
		// Nível de Dificuldade
		setCell(row, 8, sheet,
				cellStyles[ExcelCellStyleReference.TEXT.ordinal()], disciplina
						.getDificuldade().toString());
		// Max Alunos Teóricos
		setCell(row, 9, sheet,
				cellStyles[ExcelCellStyleReference.NUMBER.ordinal()],
				disciplina.getMaxAlunosTeorico());
		// Max Alunos Práticos
		setCell(row, 10, sheet,
				cellStyles[ExcelCellStyleReference.NUMBER.ordinal()],
				disciplina.getMaxAlunosPratico());

		row++;
		return row;
	}

	private void fillInCellStyles(HSSFSheet sheet) {
		for (ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference
				.values()) {
			cellStyles[cellStyleReference.ordinal()] = getCell(
					cellStyleReference.getRow(), cellStyleReference.getCol(),
					sheet).getCellStyle();
		}
	}
}
