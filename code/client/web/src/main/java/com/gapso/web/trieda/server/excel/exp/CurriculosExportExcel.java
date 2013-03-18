package com.gapso.web.trieda.server.excel.exp;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

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
public class CurriculosExportExcel
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
	private boolean removeUnusedSheets;
	private int initialRow;
	private Map< String, Boolean > mapCurriculosExportados = new HashMap< String, Boolean >();

	public CurriculosExportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino, String fileExtension )
	{
		this( true, cenario, i18nConstants, i18nMessages, instituicaoEnsino, fileExtension );
	}

	public CurriculosExportExcel( boolean removeUnusedSheets, Cenario cenario,
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
		List< Curriculo > curriculos
			= Curriculo.findByCenario(
				this.instituicaoEnsino, getCenario() );

		if ( curriculos != null && !curriculos.isEmpty() )
		{
			if ( this.removeUnusedSheets )
			{
				removeUnusedSheets( this.getSheetName(), workbook );
			}

			Sheet sheet = workbook.getSheet( this.getSheetName() );
			fillInCellStyles( sheet );
			int nextRow = this.initialRow;

			for ( Curriculo c : curriculos )
			{
				nextRow = writeData( c, nextRow, sheet );
			}

			return true;
		}

		return false;
	}
	
	private int writeData( Curriculo curriculo, int row, Sheet sheet )
	{
		List< Integer > listPeriodos
			= curriculo.getPeriodos();

		if ( listPeriodos == null || listPeriodos.size() == 0 )
		{
			return row;
		}

		for ( Integer periodo : listPeriodos )
		{
			List< CurriculoDisciplina > disciplinasDeUmPeriodo = curriculo.getCurriculoDisciplinasByPeriodo(periodo);

			if ( disciplinasDeUmPeriodo == null || disciplinasDeUmPeriodo.size() == 0 )
			{
				continue;
			}

			for ( CurriculoDisciplina disciplinaDeUmPeriodo : disciplinasDeUmPeriodo )
			{
				// Chegando nesse ponto, temos que mais uma linha será escrita
				// na planilha de exportação. Assim, registramos essa linha
				String key = "";

				key += curriculo.getCurso().getCodigo();
				key += "-" + curriculo.getCodigo();
				key += "-" + curriculo.getDescricao();
				key += "-" + periodo;
				key += "-" + disciplinaDeUmPeriodo.getDisciplina().getCodigo();
				key += "-" + disciplinaDeUmPeriodo.getCurriculo().getSemanaLetiva().getCodigo();

				if ( this.mapCurriculosExportados.containsKey( key ) )
				{
					continue;
				}
				else
				{
					this.mapCurriculosExportados.put( key, true );
				}

				// Curso
				setCell( row, 2, sheet, this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
					curriculo.getCurso().getCodigo() );

				// Código
				setCell( row, 3, sheet, this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
					curriculo.getCodigo() );

				// Descrição
				setCell( row, 4, sheet, this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
					curriculo.getDescricao() );

				// Período
				setCell( row, 5, sheet, this.cellStyles[ ExcelCellStyleReference.NUMBER.ordinal() ], periodo );

				// Disciplina
				setCell( row, 6, sheet, this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
					disciplinaDeUmPeriodo.getDisciplina().getCodigo() );

				// Semana Letiva
				setCell( row, 7, sheet, this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
					disciplinaDeUmPeriodo.getCurriculo().getSemanaLetiva().getCodigo() );

				row++;
			}
		}

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
