package com.gapso.web.trieda.server.excel.exp;

import java.util.List;

import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Aluno;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
public class AlunosExportExcel extends AbstractExportExcel {
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

	private CellStyle [] cellStyles;
	private boolean removeUnusedSheets;
	private int initialRow;

	public AlunosExportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( true, cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );
	}

	public AlunosExportExcel( boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		super( true, ExcelInformationType.ALUNOS.getSheetName(), cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );

		this.cellStyles = new CellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 6;
	}

	@Override
	public String getFileName() {
		return "Alunos";
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
		return "Alunos";
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel( Workbook workbook ) {
		List<Aluno> alunos = Aluno.findByCenario(this.instituicaoEnsino,getCenario());

		if (this.removeUnusedSheets) {
			removeUnusedSheets(this.getSheetName(),workbook);
		}
		if (!alunos.isEmpty()) {
			Sheet sheet = workbook.getSheet(this.getSheetName());
			fillInCellStyles(sheet);
			int nextRow = this.initialRow;
			for (Aluno aluno : alunos) {
				nextRow = writeData(aluno,nextRow,sheet);
			}

			return true;
		}

		return false;
	}

	private int writeData(Aluno aluno, int row, Sheet sheet) {
		// Matrícula
		setCell(row,2,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],aluno.getMatricula());
		// Nome
		setCell(row,3,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],aluno.getNome());
		// Formando?
		setCell(row,4,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],(aluno.getFormando() ? getI18nConstants().sim() : HtmlUtils.htmlUnescape(getI18nConstants().nao())));
		// Virtual?
		setCell(row,5,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],(aluno.getVirtual() ? getI18nConstants().sim() : HtmlUtils.htmlUnescape(getI18nConstants().nao())));
		// Criado por Trieda?
		setCell(row,6,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],(aluno.getCriadoTrieda() ? getI18nConstants().sim() : HtmlUtils.htmlUnescape(getI18nConstants().nao())));

		row++;
		return row;
	}

	private void fillInCellStyles(Sheet sheet) {
		for (ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values()) {
			cellStyles[cellStyleReference.ordinal()] = getCell(cellStyleReference.getRow(),cellStyleReference.getCol(),sheet).getCellStyle();
		}
	}
}