package com.gapso.web.trieda.server.excel.exp;

import java.util.List;

import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Unidade;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class UnidadesExportExcel
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

	private HSSFCellStyle [] cellStyles;
	private boolean removeUnusedSheets;
	private String sheetName;
	private int initialRow;

	public UnidadesExportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino )
	{
		this( true, cenario, i18nConstants, i18nMessages, instituicaoEnsino );
	}

	public UnidadesExportExcel( boolean removeUnusedSheets,
		Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino )
	{
		super( cenario,i18nConstants,i18nMessages, instituicaoEnsino );

		this.cellStyles = new HSSFCellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.sheetName = ExcelInformationType.UNIDADES.getSheetName();
		this.initialRow = 6;
	}

	@Override
	public String getFileName() {
		return getI18nConstants().unidades();
	}
	
	@Override
	protected String getPathExcelTemplate() {
		return "/templateExport.xls";
	}

	@Override
	protected String getReportName() {
		return getI18nConstants().unidades();
	}

	@Override
	protected boolean fillInExcel( HSSFWorkbook workbook )
	{
		List< Unidade > unidades = Unidade.findByCenario(
		 	this.instituicaoEnsino, getCenario() );

		if (!unidades.isEmpty()) {
			if (this.removeUnusedSheets) {
				removeUnusedSheets(this.sheetName,workbook);
			}
			HSSFSheet sheet = workbook.getSheet(this.sheetName);
			fillInCellStyles(sheet);
			int nextRow = this.initialRow;
			for ( Unidade u : unidades )
			{
				nextRow = writeData( u, nextRow, sheet );
			}

			return true;
		}

		return false;
	}

	private int writeData( Unidade unidade, int row, HSSFSheet sheet )
	{
		// Codigo
		setCell(row,2,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],unidade.getCodigo());
		// Nome
		setCell(row,3,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],unidade.getNome());
		// Campus
		setCell(row,4,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],unidade.getCampus().getCodigo());
		
		row++;
		return row;
	}
	
	private void fillInCellStyles(HSSFSheet sheet) {
		for (ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values()) {
			cellStyles[cellStyleReference.ordinal()] = getCell(cellStyleReference.getRow(),cellStyleReference.getCol(),sheet).getCellStyle();
		}
	}
}