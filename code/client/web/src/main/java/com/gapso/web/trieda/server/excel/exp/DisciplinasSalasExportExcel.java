package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.CurriculoDisciplina;
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
	protected boolean fillInExcel( Workbook workbook, Workbook templateWorkbook )
	{
		List< Sala > salas = Sala.findByCenario(
			this.instituicaoEnsino, getCenario() );

		if ( !salas.isEmpty() )
		{
			if ( this.removeUnusedSheets )
			{
				removeUnusedSheets( this.getSheetName(), workbook );
			}

			sheet = workbook.getSheet( this.getSheetName() );
			if (isXls()) {
				fillInCellStyles(sheet);
			}
			else {
				Sheet templateSheet = templateWorkbook.getSheet(this.getSheetName());
				fillInCellStyles(templateSheet);
			}

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
		// Agrupa as informações por período
		Map< Integer, List< CurriculoDisciplina > > periodoToCurriculosDisciplinasMap
			= new TreeMap< Integer, List< CurriculoDisciplina > >();

		for ( CurriculoDisciplina disciplinaDeUmPeriodo :
				sala.getCurriculoDisciplinas() )
		{
			List<CurriculoDisciplina> list = periodoToCurriculosDisciplinasMap
					.get( disciplinaDeUmPeriodo.getPeriodo() );

			if ( list == null )
			{
				list = new ArrayList< CurriculoDisciplina >();
				periodoToCurriculosDisciplinasMap.put(
					disciplinaDeUmPeriodo.getPeriodo(), list );
			}

			list.add( disciplinaDeUmPeriodo );
		}

		for ( Integer periodo : periodoToCurriculosDisciplinasMap.keySet() )
		{
			for ( CurriculoDisciplina disciplinaDeUmPeriodo :
					periodoToCurriculosDisciplinasMap.get( periodo ) )
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

				// Curso
				setCell( row, 3, sheet,
					cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
					disciplinaDeUmPeriodo.getCurriculo().getCurso().getCodigo() );

				// Matriz Curricular
				setCell( row, 4, sheet,
					cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
					disciplinaDeUmPeriodo.getCurriculo().getCodigo() );

				// Período
				setCell( row, 5, sheet,
					cellStyles[ ExcelCellStyleReference.NUMBER.ordinal() ], periodo );

				// Disciplina
				setCell( row, 6, sheet,
					cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ],
					disciplinaDeUmPeriodo.getDisciplina().getCodigo() );

				row++;
			}
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
