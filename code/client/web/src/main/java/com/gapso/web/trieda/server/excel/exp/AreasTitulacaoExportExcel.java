package com.gapso.web.trieda.server.excel.exp;

import java.util.List;

import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;

import com.gapso.trieda.domain.AreaTitulacao;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class AreasTitulacaoExportExcel extends AbstractExportExcel {
	
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
	private HSSFCellStyle[] cellStyles;
	
	private boolean removeUnusedSheets;
	private String sheetName;
	private int initialRow;
	
	public AreasTitulacaoExportExcel( Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino )
	{
			this( true, cenario, i18nConstants, i18nMessages, instituicaoEnsino );
	}

	public AreasTitulacaoExportExcel( boolean removeUnusedSheets,
		Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino )
	{
		super( cenario, i18nConstants, i18nMessages, instituicaoEnsino );

		this.cellStyles = new HSSFCellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.sheetName = ExcelInformationType.AREAS_TITULACAO.getSheetName();
		this.initialRow = 6;
	}

	@Override
	public String getFileName() {
		return getI18nConstants().areasTitulacao();
	}
	
	@Override
	protected String getPathExcelTemplate() {
		return "/templateExport.xls";
	}

	@Override
	protected String getReportName() {
		return getI18nConstants().areasTitulacao();
	}

	@Override
	protected boolean fillInExcel( HSSFWorkbook workbook )
	{
		List< AreaTitulacao > areas
			= AreaTitulacao.findAll( this.instituicaoEnsino );

		if ( !areas.isEmpty() )
		{
			if ( this.removeUnusedSheets )
			{
				removeUnusedSheets( this.sheetName, workbook );
			}

			HSSFSheet sheet = workbook.getSheet( this.sheetName );
			fillInCellStyles( sheet );
			int nextRow = this.initialRow;

			for ( AreaTitulacao area : areas )
			{
				nextRow = writeData( area, nextRow, sheet );
			}

			return true;
		}

		return false;
	}
	
	private int writeData(AreaTitulacao area, int row, HSSFSheet sheet) {
		// Nome
		setCell(row,2,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],area.getCodigo());
		// Descrição
		setCell(row,3,sheet,cellStyles[ExcelCellStyleReference.TEXT.ordinal()],area.getDescricao());
		
		row++;
		return row;
	}
	
	private void fillInCellStyles(HSSFSheet sheet) {
		for (ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values()) {
			cellStyles[cellStyleReference.ordinal()] = getCell(cellStyleReference.getRow(),cellStyleReference.getCol(),sheet).getCellStyle();
		}
	}
}


