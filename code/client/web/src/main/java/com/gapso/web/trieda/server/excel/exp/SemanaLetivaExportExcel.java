package com.gapso.web.trieda.server.excel.exp;

import java.util.List;

import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
public class SemanaLetivaExportExcel
	extends AbstractExportExcel
{
	enum ExcelCellStyleReference
	{
		TEXT( 6, 2 );

		private int row;
		private int col;

		private ExcelCellStyleReference( int row, int col )
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

	public SemanaLetivaExportExcel(Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( true, cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );
	}

	public SemanaLetivaExportExcel( boolean removeUnusedSheets,
		Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino,
		String fileExtension)
	{
		super( true, ExcelInformationType.SEMANA_LETIVA.getSheetName(), cenario,i18nConstants,i18nMessages, instituicaoEnsino, fileExtension );

		this.cellStyles = new CellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 6;
	}

	@Override
	public String getFileName() {
		return getI18nConstants().semanaLetiva();
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
		return getI18nConstants().turnos();
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel( Workbook workbook )
	{
		List< SemanaLetiva > semanas = SemanaLetiva.findByCenario(
		 	this.instituicaoEnsino, getCenario() );

		if (this.removeUnusedSheets) {
			removeUnusedSheets(this.getSheetName(),workbook);
		}
		
		if (!semanas.isEmpty()) {
			Sheet sheet = workbook.getSheet(this.getSheetName());
			fillInCellStyles(sheet);
			int nextRow = this.initialRow;
			for ( SemanaLetiva t : semanas )
			{
				nextRow = writeData( t, nextRow, sheet );
			}

			return true;
		}
		
		return false;
	}

	private int writeData( SemanaLetiva semanas, int row, Sheet sheet )
	{
		// Codigo
		setCell(row,2,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],semanas.getCodigo());
		// Descricao
		setCell(row,3,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],semanas.getDescricao());
		// Duracao
		setCell(row,4,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],semanas.getTempo());
		// Permite Intervalo
		setCell(row,5,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],semanas.getPermiteIntervaloAula() ? getI18nConstants().sim() : HtmlUtils.htmlUnescape(getI18nConstants().nao()));
		
		row++;
		return row;
	}
	
	private void fillInCellStyles(Sheet sheet) {
		for (ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values()) {
			cellStyles[cellStyleReference.ordinal()] = getCell(cellStyleReference.getRow(),cellStyleReference.getCol(),sheet).getCellStyle();
		}
	}
}
