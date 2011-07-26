package com.gapso.web.trieda.server.excel.exp;

import java.util.List;

import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;

import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class CampiExportExcel extends AbstractExportExcel
{
	enum ExcelCellStyleReference
	{
		TEXT( 6, 2 ),
		CURRENCY( 6, 7 );

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

	private HSSFCellStyle[] cellStyles;
	private boolean removeUnusedSheets;
	private String sheetName;
	private int initialRow;

	public CampiExportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages )
	{
		this(true, cenario, i18nConstants, i18nMessages);
	}

	public CampiExportExcel( boolean removeUnusedSheets, Cenario cenario,
			TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages )
	{
		super( cenario, i18nConstants, i18nMessages );

		this.cellStyles = new HSSFCellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.sheetName = ExcelInformationType.CAMPI.getSheetName();
		this.initialRow = 6;
	}

	@Override
	public String getFileName()
	{
		return getI18nConstants().campi();
	}

	@Override
	protected String getPathExcelTemplate()
	{
		return "/templateExport.xls";
	}

	@Override
	protected String getReportName()
	{
		return getI18nConstants().campi();
	}

	@Override
	protected boolean fillInExcel( HSSFWorkbook workbook )
	{
		List< Campus > campi = Campus.findByCenario( getCenario() );

		if ( !campi.isEmpty() )
		{
			if ( this.removeUnusedSheets )
			{
				removeUnusedSheets( this.sheetName, workbook );
			}

			HSSFSheet sheet = workbook.getSheet( this.sheetName );
			fillInCellStyles( sheet );

			int nextRow = this.initialRow;
			for ( Campus c : campi )
			{
				nextRow = writeData( c, nextRow, sheet );
			}

			return true;
		}

		return false;
	}

	private int writeData( Campus campus, int row, HSSFSheet sheet )
	{
		// Codigo
		setCell( row, 2, sheet,
			cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], campus.getCodigo() );

		// Nome
		setCell( row, 3, sheet,
			cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], campus.getNome() );
		// Estado
		setCell( row, 4, sheet,
			cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], campus.getEstado().toString() );

		// Municipio
		setCell( row, 5, sheet,
			cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], campus.getMunicipio() );

		// Bairro
		setCell( row, 6, sheet,
			cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], campus.getBairro() );

		// Custo Medio do Credito
		setCell( row, 7, sheet,
			cellStyles[ ExcelCellStyleReference.CURRENCY.ordinal() ], campus.getValorCredito() );

		row++;
		return row;
	}

	private void fillInCellStyles( HSSFSheet sheet )
	{
		for ( ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values() )
		{
			cellStyles[ cellStyleReference.ordinal() ] = getCell(
				cellStyleReference.getRow(), cellStyleReference.getCol(), sheet ).getCellStyle();
		}
	}
}
