package com.gapso.web.trieda.server.excel.exp;

import java.util.List;

import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Sala;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
public class DisciplinasSalasExportExcel
	extends AbstractExportExcel
{
	enum ExcelCellStyleReference
	{
		TEXT( 6, 2 ),
		NUMBER( 6, 5 );

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
	private Sheet sheet;
	private boolean removeUnusedSheets;
	private int initialRow;

	public DisciplinasSalasExportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( true, cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );
	}

	public DisciplinasSalasExportExcel( boolean removeUnusedSheets,	Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, 
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		super( true, ExcelInformationType.DISCIPLINAS_SALAS.getSheetName(), cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );

		this.cellStyles = new CellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 6;
	}

	@Override
	public String getFileName()
	{
		return getI18nConstants().disciplinasSalas();
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
		return getI18nConstants().disciplinasSalas();
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel( Workbook workbook )
	{
		List< Sala > salas = Sala.findByCenario(
			this.instituicaoEnsino, getCenario() );
		
		if ( this.removeUnusedSheets )
		{
			removeUnusedSheets( this.getSheetName(), workbook );
		}
		if ( !salas.isEmpty() )
		{
			sheet = workbook.getSheet( this.getSheetName() );
			fillInCellStyles(sheet);

			int nextRow = this.initialRow;
			for ( Sala sala : salas )
			{
				nextRow = writeData( sala, nextRow );
			}

			return true;
		}

		return false;
	}

	private int writeData( Sala sala, int row )
	{
		for ( Disciplina disciplinas :
				sala.getDisciplinas() )
		{
			if (isXls()){
				Sheet newSheet = restructuringWorkbookIfRowLimitIsViolated(row,1,sheet);
				if (newSheet != null) {
					row = this.initialRow;
					sheet = newSheet;
				}
			}
			// Sala
			setCell( row, 2, sheet,
				cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], sala.getCodigo() );

			// Disciplina
			setCell( row, 3, sheet,
				cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
				disciplinas.getCodigo() );
			
			row++;
		}

		return row;
	}

	private void fillInCellStyles( Sheet sheet )
	{
		for ( ExcelCellStyleReference cellStyleReference :
				ExcelCellStyleReference.values() )
		{
			cellStyles[ cellStyleReference.ordinal() ] = getCell(
				cellStyleReference.getRow(), cellStyleReference.getCol(), sheet ).getCellStyle();
		}
	}
}
