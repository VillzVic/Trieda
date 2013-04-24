package com.gapso.web.trieda.server.excel.exp;

import java.util.List;

import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.AreaTitulacao;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
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

	private CellStyle [] cellStyles;
	private boolean removeUnusedSheets;
	private int initialRow;

	public CursoAreasTitulacaoExportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( true, cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );
	}

	public CursoAreasTitulacaoExportExcel( boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		super( true, ExcelInformationType.CURSO_AREAS_TITULACAO.getSheetName(), cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );

		this.cellStyles = new CellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 6;
	}

	@Override
	public String getFileName()
	{
		return HtmlUtils.htmlUnescape( getI18nConstants().cursosAreasTitulacao() );
	}

	@Override
	protected String getPathExcelTemplate()
	{
		if ( fileExtension.equals("xlsx") )
		{
			return "/templateExport.xlsx";
		}
		else
			return "/templateExport.xls";
	}

	@Override
	protected String getReportName()
	{
		return HtmlUtils.htmlUnescape( getI18nConstants().cursosAreasTitulacao() );
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel( Workbook workbook, Workbook templateWorkbook )
	{
		List< Curso > cursos = Curso.findByCenario(
			this.instituicaoEnsino, getCenario() );

		boolean empty = true;

		for ( Curso curso : cursos )
		{
			if ( !curso.getAreasTitulacao().isEmpty() )
			{
				empty = false;
				break;
			}
		}

		if ( empty )
		{
			return false;
		}

		if ( this.removeUnusedSheets )
		{
			removeUnusedSheets( this.getSheetName(), workbook );
		}
	
		Sheet sheet = workbook.getSheet( this.getSheetName() );
		if (isXls()) {
			fillInCellStyles(sheet);
		}
		else {
			Sheet templateSheet = templateWorkbook.getSheet(this.getSheetName());
			fillInCellStyles(templateSheet);
		}

		int nextRow = this.initialRow;
		for ( Curso curso : cursos )
		{
			for ( AreaTitulacao area : curso.getAreasTitulacao() )
			{
				nextRow = writeData( curso, area, nextRow, sheet );
			}
		}

		return true;
	}

	private int writeData(Curso curso, AreaTitulacao areaTitulacao, int row, Sheet sheet) {
		// Curso
		setCell(row,2,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],curso.getCodigo());
		// Area de Titulacao
		setCell(row,3,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],areaTitulacao.getCodigo());
		
		row++;
		return row;
	}
	
	private void fillInCellStyles(Sheet sheet) {
		for (ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values()) {
			cellStyles[cellStyleReference.ordinal()] = getCell(cellStyleReference.getRow(),cellStyleReference.getCol(),sheet).getCellStyle();
		}
	}
}
