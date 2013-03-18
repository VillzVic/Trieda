package com.gapso.web.trieda.server.excel.exp;

import java.util.List;

import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclarationAnnotation;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportMethodScan;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@ProgressDeclarationAnnotation
public class CurriculosExportExcelWithMergedCells
	extends AbstractExportExcel
{
	enum ExcelCellStyleReference
	{
		TEXT( 6, 2 ),
		NUMBER( 6, 5 );

		private int row;
		private int col;

		private ExcelCellStyleReference(
			int row, int col )
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

	public CurriculosExportExcelWithMergedCells( Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( true, cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );
	}

	public CurriculosExportExcelWithMergedCells( boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		super( true, ExcelInformationType.CURRICULOS.getSheetName(), cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );

		this.cellStyles = new CellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.initialRow = 6;
	}

	@Override
	public String getFileName()
	{
		return getI18nConstants().curriculos();
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
		return getI18nConstants().curriculos();
	}

	@Override
	@ProgressReportMethodScan(texto = "Processando conteúdo da planilha")
	protected boolean fillInExcel( Workbook workbook )
	{
		List< Curriculo > curriculo
			= Curriculo.findByCenario( this.instituicaoEnsino, getCenario() );

		if ( !curriculo.isEmpty() )
		{
			if ( this.removeUnusedSheets )
			{
				removeUnusedSheets( this.getSheetName(), workbook );
			}

			Sheet sheet = workbook.getSheet( this.getSheetName() );
			fillInCellStyles( sheet );
			int nextRow = this.initialRow;

			for ( Curriculo c : curriculo )
			{
				nextRow = writeData( c, nextRow, sheet );
			}

			return true;
		}

		return false;
	}
	
	private int writeData( Curriculo curriculo, int row, Sheet sheet )
	{
		int initialRowCurriculo = row;

		// Curso
		setCell( row, 2, sheet, this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
			curriculo.getCurso().getCodigo() );

		// Código
		setCell( row, 3, sheet, this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], curriculo.getCodigo() );

		// Descrição
		setCell( row, 4, sheet, this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], curriculo.getDescricao() );

		// Semana Letiva
		setCell( row, 7, sheet, this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], curriculo.getSemanaLetiva().getCodigo() );

		List< Integer > listPeriodos
			= curriculo.getPeriodos();

		for ( Integer periodo : listPeriodos )
		{
			int initialRowPeriodo = row;

			// Período
			setCell( row, 5, sheet, this.cellStyles[ ExcelCellStyleReference.NUMBER.ordinal() ], periodo );

			List< CurriculoDisciplina > disciplinasDeUmPeriodo
				= curriculo.getCurriculoDisciplinasByPeriodo(periodo);

			for ( CurriculoDisciplina disciplinaDeUmPeriodo : disciplinasDeUmPeriodo )
			{
				// Disciplina
				setCell( row, 6, sheet, this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
					disciplinaDeUmPeriodo.getDisciplina().getCodigo() );

				row++;
			}

			// Merge - Período
			mergeCells( initialRowPeriodo, row - 1 , 5, 5, sheet,
				this.cellStyles[ ExcelCellStyleReference.NUMBER.ordinal() ] );
		}

		// Merge - Curso
		mergeCells( initialRowCurriculo, ( row - 1 ), 2, 2, sheet,
			this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ] );

		// Merge - Código
		mergeCells( initialRowCurriculo, ( row - 1 ), 3, 3, sheet,
			this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ] );

		// Merge - Descrição
		mergeCells( initialRowCurriculo, ( row - 1 ), 4, 4, sheet,
			this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ] );

		// Merge - Semana Letiva
		mergeCells( initialRowCurriculo, ( row - 1 ), 7, 7, sheet,
			this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ] );

		return row;
	}

	private void fillInCellStyles( Sheet sheet )
	{
		for ( ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values() )
		{
			this.cellStyles[ cellStyleReference.ordinal() ] = getCell(
				cellStyleReference.getRow(), cellStyleReference.getCol(), sheet ).getCellStyle();
		}
	}
}
