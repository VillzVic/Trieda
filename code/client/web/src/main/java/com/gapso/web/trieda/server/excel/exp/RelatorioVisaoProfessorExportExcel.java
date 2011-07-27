package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFComment;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;

import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.ProfessorVirtual;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.AtendimentosServiceImpl;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

@SuppressWarnings("unused")
public class RelatorioVisaoProfessorExportExcel
	extends AbstractExportExcel
{
	enum ExcelCellStyleReference
	{
		HEADER_LEFT_TEXT( 5,3 ),
		HEADER_CENTER_VALUE( 5,4 ),
		HEADER_CENTER_TEXT( 7,2 ),
		TEXT( 8,2 );

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
	private RelatorioVisaoProfessorFiltroExcel relatorioFiltro;
	private Campus campus;	

	public RelatorioVisaoProfessorExportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages )
	{
		this( true, cenario, i18nConstants, i18nMessages, null );
	}

	public RelatorioVisaoProfessorExportExcel( Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, ExportExcelFilter filter )
	{
		this( true, cenario, i18nConstants, i18nMessages, filter );
	}

	public RelatorioVisaoProfessorExportExcel(boolean removeUnusedSheets,
		Cenario cenario, TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages )
	{
		this( removeUnusedSheets, cenario, i18nConstants, i18nMessages, null );
	}

	public RelatorioVisaoProfessorExportExcel(boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, ExportExcelFilter filter )
	{
		super( cenario, i18nConstants, i18nMessages );

		this.cellStyles = new HSSFCellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.sheetName = ExcelInformationType.RELATORIO_VISAO_PROFESSOR.getSheetName();
		this.initialRow = 5;
		this.setFilter( filter );

		if ( this.relatorioFiltro != null )
		{
			this.campus = Campus.find( this.relatorioFiltro.getCampusDTO().getId() );
		}
	}

	@Override
	public String getFileName()
	{
		return getI18nConstants().relatorioVisaoProfessor();
	}

	@Override
	protected String getPathExcelTemplate()
	{
		return "/templateExport.xls";
	}

	@Override
	protected String getReportName()
	{
		return getI18nConstants().relatorioVisaoProfessor();
	}

	public RelatorioVisaoProfessorFiltroExcel getFilter()
	{
		return relatorioFiltro;
	}

	public void setFilter( ExportExcelFilter filter )
	{
		if ( filter instanceof RelatorioVisaoProfessorFiltroExcel )
		{
			this.relatorioFiltro = (RelatorioVisaoProfessorFiltroExcel)filter;
		}
		else
		{
			this.relatorioFiltro = null;
		}
	}

	private List< AtendimentoOperacionalDTO > getAtendimentos()
	{
		boolean idAdmin = true;

		Professor professor = ( relatorioFiltro.getProfessorDTO() == null ? null :
			Professor.find( relatorioFiltro.getProfessorDTO().getId() ) );

		ProfessorVirtual professorVirtual = ( relatorioFiltro.getProfessorVirtualDTO() == null ? null :
			ProfessorVirtual.find( relatorioFiltro.getProfessorVirtualDTO().getId() ) );

		Turno turno = Turno.find( relatorioFiltro.getTurnoDTO().getId() );

		List< AtendimentoOperacional > atendimentosOperacional
			= AtendimentoOperacional.getAtendimentosOperacional(
				idAdmin, professor, professorVirtual, turno );

		List< AtendimentoOperacionalDTO > atendimentosOperacionalDTO
			= new ArrayList< AtendimentoOperacionalDTO >();

		for ( AtendimentoOperacional domain : atendimentosOperacional )
		{
			atendimentosOperacionalDTO.add(
				ConvertBeans.toAtendimentoOperacionalDTO( domain ) );
		}

		return atendimentosOperacionalDTO;
	}
	
	@Override
	protected boolean fillInExcel( HSSFWorkbook workbook )
	{
		boolean result = false;

		List< AtendimentoOperacionalDTO > atendimentos = this.getAtendimentos();

		if ( !atendimentos.isEmpty() )
		{
			HSSFSheet sheet = workbook.getSheet( this.sheetName );
			fillInCellStyles( sheet );

			List< HSSFComment > excelCommentsPool = buildExcelCommentsPool( workbook );
			Iterator< HSSFComment > itExcelCommentsPool = excelCommentsPool.iterator();
			List< HSSFCellStyle > excelColorsPool = buildColorPaletteCellStyles( workbook );

			Map< String, HSSFCellStyle > codigoDisciplinaToColorMap
				= new HashMap< String, HSSFCellStyle >();

			// Dado o id de um professor (ou professor virtual),
			// temos os seus atendimentos, organizados por turno
			Map< Long, Map< Turno, List< AtendimentoOperacionalDTO > > > mapNivel1
				= new TreeMap< Long, Map< Turno, List< AtendimentoOperacionalDTO > > >();

			Professor professor = null;
			ProfessorVirtual professorVirtual = null;
			for ( AtendimentoOperacionalDTO atendimento : atendimentos )
			{
				if ( this.campus == null )
				{
					this.campus = Campus.find( atendimento.getCampusId() );
				}

				if ( atendimento.getProfessorId() != null )
				{
					professor = Professor.find( atendimento.getProfessorId() );
				}
				else
				{
					professorVirtual = ProfessorVirtual.find( atendimento.getProfessorVirtualId() );
				}

				Long professorId = ( professor == null ? professorVirtual.getId() : professor.getId() );
				Turno turno = Turno.find( atendimento.getTurnoId() );

				Map< Turno, List< AtendimentoOperacionalDTO > > mapNivel2 = mapNivel1.get( professorId );
				if ( mapNivel2 == null )
				{
					mapNivel2 = new HashMap< Turno, List< AtendimentoOperacionalDTO > >();
					mapNivel1.put( professorId, mapNivel2 );
				}

				List< AtendimentoOperacionalDTO > list = mapNivel2.get( turno );
				if ( list == null )
				{
					list = new ArrayList< AtendimentoOperacionalDTO >();
					mapNivel2.put( turno, list );
				}

				list.add( atendimento );
				HSSFCellStyle style = codigoDisciplinaToColorMap.get( atendimento.getDisciplinaString() );
				if ( style == null )
				{
					int index = ( codigoDisciplinaToColorMap.size() % excelColorsPool.size() );
					codigoDisciplinaToColorMap.put( atendimento.getDisciplinaString(), excelColorsPool.get( index ) );
				}
			}

			int nextRow = this.initialRow;
			for ( Long profId : mapNivel1.keySet() )
			{
				Map< Turno, List< AtendimentoOperacionalDTO > > mapNivel2 = mapNivel1.get( profId );
				for ( Turno turno : mapNivel2.keySet() )
				{
					if ( professor != null )
					{
						nextRow = writeProfessor( this.campus, professor, turno, mapNivel2.get( turno ),
							nextRow, sheet, itExcelCommentsPool, codigoDisciplinaToColorMap );
					}
					else if ( professorVirtual != null )
					{
						nextRow = writeProfessorVirtual( this.campus, professorVirtual, turno, mapNivel2.get( turno ),
							nextRow, sheet, itExcelCommentsPool, codigoDisciplinaToColorMap );
					}
				}				
			}			

			result = true;
		}

		if ( this.removeUnusedSheets )
		{
			removeUnusedSheets( this.sheetName, workbook );
		}

		return result;
	}

	private int writeProfessor( Campus campus, Professor professor, Turno turno,
		List< AtendimentoOperacionalDTO > atendimentos, int row, HSSFSheet sheet,
		Iterator< HSSFComment > itExcelCommentsPool,
		Map< String, HSSFCellStyle > codigoDisciplinaToColorMap )
	{
		row = writeHeaderProfessor( campus, professor, turno, row, sheet );

		int initialRow = row;
		int col = 2;

		// Preenche grade com créditos e células vazias
		int maxCreditos = turno.calculaMaxCreditos();
		for ( int indexCredito = 1; indexCredito <= maxCreditos; indexCredito++ )
		{
			// Créditos
			setCell( row, col++, sheet,
				cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], indexCredito );

			// Dias Semana
			for ( Semanas semanas : Semanas.values() )
			{
				setCell( row, col++, sheet,
					cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], "" );
			}

			row++;
			col = 2;
		}

		// Processa os atendimentos lidos do BD para que os mesmos sejam visualizados na visão professor
		AtendimentosServiceImpl atendimentosService = new AtendimentosServiceImpl();
		List< AtendimentoOperacionalDTO > atendimentosParaVisaoProfessor
			= atendimentosService.montaListaParaVisaoProfessor( atendimentos );

		// Agrupa os atendimentos por dia da semana
		Map< Integer, List< AtendimentoOperacionalDTO > > diaSemanaToAtendimentosMap
			= new HashMap< Integer, List< AtendimentoOperacionalDTO > >();

		for ( AtendimentoOperacionalDTO atendimento : atendimentosParaVisaoProfessor )
		{
			List< AtendimentoOperacionalDTO > list
				= diaSemanaToAtendimentosMap.get( atendimento.getSemana() );

			if ( list == null )
			{
				list = new ArrayList< AtendimentoOperacionalDTO >();
				diaSemanaToAtendimentosMap.put( atendimento.getSemana(), list );
			}

			list.add( atendimento );
		}

		// Para cada dia da semana, escreve os atendimentos no excel
		for ( Integer diaSemanaInt : diaSemanaToAtendimentosMap.keySet() )
		{
			row = initialRow;

			Semanas diaSemana = Semanas.get( diaSemanaInt );
			switch ( diaSemana )
			{
				case SEG: { col = 3; break; }
				case TER: { col = 4; break; }
				case QUA: { col = 5; break; }
				case QUI: { col = 6; break; }
				case SEX: { col = 7; break; }
				case SAB: { col = 8; break; }
				case DOM: { col = 9; break; }
			}

			List< AtendimentoOperacionalDTO > atedimentosDiaSemana
				= diaSemanaToAtendimentosMap.get( diaSemanaInt );

			for ( AtendimentoOperacionalDTO atendimento : atedimentosDiaSemana )
			{
				HSSFCellStyle style = codigoDisciplinaToColorMap.get( atendimento.getDisciplinaString() );

				// Escreve célula principal
				setCell( row, col, sheet, style, itExcelCommentsPool,
					atendimento.getExcelContentVisaoProfessor(), atendimento.getExcelCommentVisaoProfessor() );

				// Une células de acordo com a quantidade de créditos
				mergeCells( row, ( row + atendimento.getTotalLinhas() - 1 ), col, col, sheet, style );

				row += atendimento.getTotalLinhas();
			}
		}

		return ( initialRow + maxCreditos + 1 );
	}
	
	private int writeProfessorVirtual( Campus campus, ProfessorVirtual professorVirtual, Turno turno,
		List< AtendimentoOperacionalDTO > atendimentos, int row, HSSFSheet sheet,
		Iterator< HSSFComment > itExcelCommentsPool,
		Map< String,HSSFCellStyle > codigoDisciplinaToColorMap )
	{
		row = writeHeaderProfessorVirtual( campus, professorVirtual, turno, row, sheet );

		int initialRow = row;
		int col = 2;

		// Preenche grade com créditos e células vazias
		int maxCreditos = turno.calculaMaxCreditos();
		for ( int indexCredito = 1; indexCredito <= maxCreditos; indexCredito++ )
		{
			// Créditos
			setCell( row, col++, sheet,
				cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], indexCredito );

			// Dias Semana
			for ( Semanas semanas : Semanas.values() )
			{
				setCell( row, col++, sheet,
					cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], "" );
			}

			row++;
			col = 2;
		}

		// Processa os atendimentos lidos do BD para que os mesmos sejam visualizados na visão professor
		AtendimentosServiceImpl atendimentosService = new AtendimentosServiceImpl();
		List< AtendimentoOperacionalDTO > atendimentosParaVisaoProfessor
			= atendimentosService.montaListaParaVisaoProfessor( atendimentos );

		// Agrupa os atendimentos por dia da semana
		Map< Integer, List< AtendimentoOperacionalDTO > > diaSemanaToAtendimentosMap
			= new HashMap< Integer, List< AtendimentoOperacionalDTO > >();

		for ( AtendimentoOperacionalDTO atendimento : atendimentosParaVisaoProfessor )
		{
			List< AtendimentoOperacionalDTO > list
				= diaSemanaToAtendimentosMap.get( atendimento.getSemana() );

			if ( list == null )
			{
				list = new ArrayList< AtendimentoOperacionalDTO >();
				diaSemanaToAtendimentosMap.put( atendimento.getSemana(), list );
			}

			list.add( atendimento );
		}

		// Para cada dia da semana, escreve os atendimentos no excel
		for ( Integer diaSemanaInt : diaSemanaToAtendimentosMap.keySet() )
		{
			row = initialRow;

			Semanas diaSemana = Semanas.get( diaSemanaInt );
			switch ( diaSemana )
			{
				case SEG: { col = 3; break; }
				case TER: { col = 4; break; }
				case QUA: { col = 5; break; }
				case QUI: { col = 6; break; }
				case SEX: { col = 7; break; }
				case SAB: { col = 8; break; }
				case DOM: { col = 9; break; }
			}

			List< AtendimentoOperacionalDTO > atedimentosDiaSemana
					= diaSemanaToAtendimentosMap.get( diaSemanaInt );

			for ( AtendimentoOperacionalDTO atendimento : atedimentosDiaSemana )
			{
				HSSFCellStyle style = codigoDisciplinaToColorMap.get( atendimento.getDisciplinaString() );

				// Escreve célula principal
				setCell( row, col, sheet, style, itExcelCommentsPool,
					atendimento.getExcelContentVisaoProfessor(), atendimento.getExcelCommentVisaoProfessor() );

				// Une células de acordo com a quantidade de créditos
				mergeCells( row, ( row + atendimento.getTotalCreditos() - 1 ), col, col, sheet, style );

				row += atendimento.getTotalCreditos();
			}
		}
					
		return ( initialRow + maxCreditos + 1 );
	}

	private int writeHeaderProfessor( Campus campus, Professor professor,
		Turno turno, int row, HSSFSheet sheet )
	{
		int col = 3;

		// Campus
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ], "Campus" );
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ], campus.getCodigo() );

		// Professor
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ], "Professor" );
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ], professor.getNome() );

		row++;
		col = 3;

		// Turno
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ], "Turno" );
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ], turno.getNome() );

		// Professor Virtual
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ], "Professor Virtual" );
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ], "" );

		row++;
		col = 2;

		// Créditos
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_CENTER_TEXT.ordinal() ], "Creditos" );

		// Dias Semana
		for ( Semanas semanas : Semanas.values() )
		{
			setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_CENTER_TEXT.ordinal() ], semanas.name() );
		}

		row++;
		return row;
	}
	
	private int writeHeaderProfessorVirtual( Campus campus, ProfessorVirtual professorVirtual,
		Turno turno, int row, HSSFSheet sheet )
	{
		int col = 3;

		// Campus
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ], "Campus" );
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ], campus.getCodigo() );

		// Professor
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ], "Professor Virtual" );
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ], "" );

		row++;
		col = 3;

		// Turno
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ], "Turno" );
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ], turno.getNome() );

		// Professor Virtual
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ], "Professor Virtual" );
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ], professorVirtual.getId() );

		row++;
		col = 2;

		// Créditos
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_CENTER_TEXT.ordinal() ], "Creditos" );

		// Dias Semana
		for ( Semanas semanas : Semanas.values() )
		{
			setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_CENTER_TEXT.ordinal() ], semanas.name() );
		}

		row++;
		return row;
	}

	private List< HSSFCellStyle > buildColorPaletteCellStyles( HSSFWorkbook workbook )
	{
		List< HSSFCellStyle > colorPalleteCellStylesList = new ArrayList< HSSFCellStyle >();

		HSSFSheet sheet = workbook.getSheet(
			ExcelInformationType.PALETA_CORES.getSheetName() );

		if ( sheet != null )
		{
            for ( int rowIndex = sheet.getFirstRowNum(); rowIndex <= sheet.getLastRowNum(); rowIndex++ )
            {
            	HSSFRow row = sheet.getRow( rowIndex );
            	if ( row != null )
            	{
            		HSSFCell cell = row.getCell( (int)row.getFirstCellNum() );
            		if ( cell != null )
            		{
            			colorPalleteCellStylesList.add( cell.getCellStyle() );
            		}
            	}
            }
		}

		removeUnusedSheet( ExcelInformationType.PALETA_CORES.getSheetName(), workbook );
		return colorPalleteCellStylesList;
	}

	private void fillInCellStyles( HSSFSheet sheet )
	{
		for ( ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values() )
		{
			cellStyles[ cellStyleReference.ordinal() ]
				= getCell( cellStyleReference.getRow(), cellStyleReference.getCol(), sheet ).getCellStyle();
		}
	}

	private List< HSSFComment > buildExcelCommentsPool( HSSFWorkbook workbook )
	{
		List< HSSFComment > excelCommentsPool = new ArrayList< HSSFComment >();
		HSSFSheet sheet = workbook.getSheet(
			ExcelInformationType.RELATORIO_VISAO_PROFESSOR.getSheetName() );

		if ( sheet != null )
		{
            for ( int rowIndex = sheet.getFirstRowNum();
            	rowIndex <= sheet.getLastRowNum(); rowIndex++ )
            {
            	HSSFRow row = sheet.getRow( rowIndex );

            	if ( row != null )
            	{
            		HSSFCell cell = row.getCell( 25 );

            		if ( cell != null && cell.getCellComment() != null )
            		{
            			excelCommentsPool.add( cell.getCellComment() );
            		}
            	}
            }
		}

		return excelCommentsPool;
	}
}
