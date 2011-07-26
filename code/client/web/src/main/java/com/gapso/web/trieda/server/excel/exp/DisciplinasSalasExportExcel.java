package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;

import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.Sala;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

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

	private HSSFCellStyle[] cellStyles;
	private boolean removeUnusedSheets;
	private String sheetName;
	private int initialRow;

	public DisciplinasSalasExportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages )
	{
		this( true, cenario, i18nConstants, i18nMessages );
	}

	public DisciplinasSalasExportExcel( boolean removeUnusedSheets,
		Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages )
	{
		super( cenario, i18nConstants, i18nMessages );
		this.cellStyles = new HSSFCellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.sheetName = ExcelInformationType.DISCIPLINAS_SALAS.getSheetName();
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
		return "/templateExport.xls";
	}

	@Override
	protected String getReportName()
	{
		return getI18nConstants().disciplinasSalas();
	}

	@Override
	protected boolean fillInExcel( HSSFWorkbook workbook )
	{
		List< Sala > salas = Sala.findByCenario( getCenario() );

		if ( !salas.isEmpty() )
		{
			if ( this.removeUnusedSheets )
			{
				removeUnusedSheets( this.sheetName, workbook );
			}

			HSSFSheet sheet = workbook.getSheet( this.sheetName );
			fillInCellStyles( sheet );

			int nextRow = this.initialRow;
			for ( Sala sala : salas )
			{
				nextRow = writeData( sala, nextRow, sheet );
			}

			return true;
		}

		return false;
	}

	private int writeData( Sala sala, int row, HSSFSheet sheet )
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

	private void fillInCellStyles( HSSFSheet sheet )
	{
		for ( ExcelCellStyleReference cellStyleReference :
				ExcelCellStyleReference.values() )
		{
			cellStyles[ cellStyleReference.ordinal() ] = getCell(
				cellStyleReference.getRow(), cellStyleReference.getCol(), sheet ).getCellStyle();
		}
	}
}
