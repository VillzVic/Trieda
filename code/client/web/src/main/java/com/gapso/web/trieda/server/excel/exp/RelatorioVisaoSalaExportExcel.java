package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.Collections;
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
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.domain.Unidade;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.AtendimentosServiceImpl;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoTaticoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class RelatorioVisaoSalaExportExcel
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

	private HSSFCellStyle [] cellStyles;
	private boolean removeUnusedSheets;
	private String sheetName;
	private int initialRow;
	private RelatorioVisaoSalaFiltroExcel relatorioFiltro;

	public RelatorioVisaoSalaExportExcel(
		Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, InstituicaoEnsino instituicaoEnsino )
	{
		this( true, cenario, i18nConstants, i18nMessages, null, instituicaoEnsino );
	}

	public RelatorioVisaoSalaExportExcel(
		Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, ExportExcelFilter filter,
		InstituicaoEnsino instituicaoEnsino )
	{
		this( true, cenario, i18nConstants, i18nMessages, filter, instituicaoEnsino );
	}

	public RelatorioVisaoSalaExportExcel(
		boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		InstituicaoEnsino instituicaoEnsino )
	{
		this( removeUnusedSheets, cenario, i18nConstants, i18nMessages, null, instituicaoEnsino );
	}

	public RelatorioVisaoSalaExportExcel(
		boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		ExportExcelFilter filter, InstituicaoEnsino instituicaoEnsino )
	{
		super( cenario, i18nConstants, i18nMessages, instituicaoEnsino );

		this.cellStyles = new HSSFCellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.sheetName = ExcelInformationType.RELATORIO_VISAO_SALA.getSheetName();
		this.initialRow = 5;

		this.setFilter( filter );
	}

	@Override
	public String getFileName()
	{
		return this.getI18nConstants().relatorioVisaoSala();
	}

	@Override
	protected String getPathExcelTemplate()
	{
		return "/templateExport.xls";
	}

	@Override
	protected String getReportName()
	{
		return this.getI18nConstants().relatorioVisaoSala();
	}

	public RelatorioVisaoSalaFiltroExcel getFilter()
	{
		return this.relatorioFiltro;
	}

	public void setFilter( ExportExcelFilter filter )
	{
		if ( filter instanceof RelatorioVisaoSalaFiltroExcel )
		{
			this.relatorioFiltro = (RelatorioVisaoSalaFiltroExcel)filter;
		}
		else
		{
			this.relatorioFiltro = null;
		}
	}

	private List< AtendimentoRelatorioDTO > getAtendimentoRelatorioDTOList( Cenario cenario )
	{
		List< AtendimentoTatico > atdTaticoList = null;
		List< AtendimentoOperacional > atdOperacionalList =  null;

		if ( this.getFilter() == null )
		{
			atdTaticoList = AtendimentoTatico.findByCenario(
				this.instituicaoEnsino, cenario );

			atdOperacionalList = AtendimentoOperacional.findByCenario(
				this.instituicaoEnsino, cenario );
		}
		else
		{
			Campus campus = Campus.find(
				this.getFilter().getCampusDTO().getId(), this.instituicaoEnsino );

			Unidade unidade = Unidade.find(
				this.getFilter().getUnidadeDTO().getId(), this.instituicaoEnsino );

			Sala sala = Sala.find(
				this.getFilter().getSalaDTO().getId(), this.instituicaoEnsino );

			Turno turno = Turno.find(
				this.getFilter().getTurnoDTO().getId(), this.instituicaoEnsino );

			SemanaLetiva semanaLetiva = SemanaLetiva.find(
					this.getFilter().getSemanaLetivaDTO().getId(), this.instituicaoEnsino );

			atdTaticoList = AtendimentoTatico.findByCenario(
				this.instituicaoEnsino, cenario, campus, unidade, sala, turno, semanaLetiva );

			atdOperacionalList = AtendimentoOperacional.findByCenario(
				this.instituicaoEnsino, cenario, campus, unidade, sala, turno, semanaLetiva );
		}

		List< AtendimentoRelatorioDTO > atdRelatorioList
			= new ArrayList< AtendimentoRelatorioDTO >(
				atdTaticoList.size() + atdOperacionalList.size() );

		List< AtendimentoTaticoDTO > listAtendTaticoDTO
			= ConvertBeans.toListAtendimentoTaticoDTO( atdTaticoList );

		List< AtendimentoOperacionalDTO > listAtendOpDTO
			= ConvertBeans.toListAtendimentoOperacionalDTO( atdOperacionalList );

		AtendimentosServiceImpl service = new AtendimentosServiceImpl();
		listAtendOpDTO = service.ordenaPorHorarioAula( listAtendOpDTO );

		atdRelatorioList.addAll( listAtendTaticoDTO );
		atdRelatorioList.addAll( listAtendOpDTO );

		return atdRelatorioList;
	}
	
	@Override
	protected boolean fillInExcel( HSSFWorkbook workbook )
	{
		boolean result = false;

		List< AtendimentoRelatorioDTO > atdRelatorioList
			= getAtendimentoRelatorioDTOList( getCenario() );

		if ( !atdRelatorioList.isEmpty() )
		{
			HSSFSheet sheet = workbook.getSheet( this.sheetName );
			fillInCellStyles( sheet );

			List< HSSFComment> excelCommentsPool = buildExcelCommentsPool( workbook );
			Iterator< HSSFComment> itExcelCommentsPool = excelCommentsPool.iterator();

			List< HSSFCellStyle > excelColorsPool = buildColorPaletteCellStyles( workbook );

			Map< String, HSSFCellStyle > codigoDisciplinaToColorMap
				= new HashMap< String, HSSFCellStyle >();

			// FIXME -- Diferenciar atendimentos pela semana letiva da disciplina atendida
			Map< Sala, Map< Turno, Map< SemanaLetiva, List< AtendimentoRelatorioDTO > > > > mapNivel1
				= new TreeMap< Sala, Map< Turno, Map< SemanaLetiva, List< AtendimentoRelatorioDTO > > > >();

			for ( AtendimentoRelatorioDTO atendimento : atdRelatorioList )
			{
				Sala sala = Sala.find( atendimento.getSalaId(), this.instituicaoEnsino );
				Turno turno = Turno.find( atendimento.getTurnoId(), this.instituicaoEnsino );
				SemanaLetiva semanaLetiva = SemanaLetiva.find( atendimento.getSemanaLetivaId(), instituicaoEnsino );

				Map< Turno, Map< SemanaLetiva, List< AtendimentoRelatorioDTO > > > mapNivel2 = mapNivel1.get( sala );

				if ( mapNivel2 == null )
				{
					mapNivel2 = new HashMap< Turno, Map< SemanaLetiva, List< AtendimentoRelatorioDTO > > >();
					mapNivel1.put( sala, mapNivel2 );
				}

				Map< SemanaLetiva, List< AtendimentoRelatorioDTO > > mapNivel3 = mapNivel2.get( turno );

				if ( mapNivel3 == null )
				{
					mapNivel3 = new HashMap< SemanaLetiva, List< AtendimentoRelatorioDTO > >();
					mapNivel2.put( turno, mapNivel3 );
				}

				List< AtendimentoRelatorioDTO > list = mapNivel3.get( semanaLetiva );
				
				if ( list == null )
				{
					list = new ArrayList< AtendimentoRelatorioDTO >();
					mapNivel3.put( semanaLetiva, list );
				}

				list.add( atendimento );

				HSSFCellStyle style = codigoDisciplinaToColorMap.get(
					atendimento.getDisciplinaString() );

				if ( style == null )
				{
					int index = ( codigoDisciplinaToColorMap.size() % excelColorsPool.size() );

					codigoDisciplinaToColorMap.put(
						atendimento.getDisciplinaString(), excelColorsPool.get( index ) );
				}
			}

			int nextRow = this.initialRow;

			for ( Sala sala : mapNivel1.keySet() )
			{
				Map< Turno, Map< SemanaLetiva, List< AtendimentoRelatorioDTO > > > mapNivel2 = mapNivel1.get( sala );

				for ( Turno turno : mapNivel2.keySet() )
				{
					Map< SemanaLetiva, List< AtendimentoRelatorioDTO > > mapNivel3 = mapNivel2.get( turno );

					for ( SemanaLetiva semanaLetiva : mapNivel3.keySet() )
					{
						List< AtendimentoRelatorioDTO > listAtendimentos = mapNivel3.get( semanaLetiva );

						nextRow = writeSala( sala, turno, semanaLetiva, listAtendimentos,
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

	private int writeSala(
		Sala sala, Turno turno, SemanaLetiva semanaLetiva,
		List< AtendimentoRelatorioDTO > atendimentos, int row, HSSFSheet sheet,
		Iterator< HSSFComment > itExcelCommentsPool,
		Map< String,HSSFCellStyle > codigoDisciplinaToColorMap )
	{
		row = writeHeader( sala, turno, row, sheet );

		int initialRow = row;
		int col = 2;

		// Preenche grade com créditos e células vazias
		int maxCreditos = semanaLetiva.calculaMaxCreditos();

		for ( int indexCredito = 1; indexCredito <= maxCreditos; indexCredito++ )
		{
			// Créditos
			setCell( row, col++, sheet,
				this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], indexCredito );

			// Dias Semana
			for ( int i = 0; i < Semanas.values().length; i++ )
			{
				setCell( row, col++, sheet,
					this.cellStyles[ ExcelCellStyleReference.TEXT.ordinal() ], "" );
			}

			row++;
			col = 2;
		}

		// Processa os atendimentos lidos do BD para
		// que os mesmos sejam visualizados na visão sala
		AtendimentosServiceImpl atendimentosService = new AtendimentosServiceImpl();
		List< AtendimentoRelatorioDTO > atendimentosParaVisaoSala
			= atendimentosService.montaListaParaVisaoSala( atendimentos );

		atendimentosParaVisaoSala = ordenaHorarioAula( atendimentosParaVisaoSala );

		// Agrupa os atendimentos por dia da semana
		Map< Integer, List< AtendimentoRelatorioDTO > > diaSemanaToAtendimentosMap
			= new HashMap< Integer, List< AtendimentoRelatorioDTO > >();

		for ( AtendimentoRelatorioDTO atendimento : atendimentosParaVisaoSala )
		{
			// TODO -- Considerar apenas os horários da semana letiva
			if ( atendimento.getSemanaLetivaId() != semanaLetiva.getId() )
			{
				continue;
			}

			List< AtendimentoRelatorioDTO > list
				= diaSemanaToAtendimentosMap.get( atendimento.getSemana() );

			if ( list == null )
			{
				list = new ArrayList< AtendimentoRelatorioDTO >();
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

			List< AtendimentoRelatorioDTO > atedimentosDiaSemana
				= diaSemanaToAtendimentosMap.get( diaSemanaInt );

			if ( atedimentosDiaSemana == null || atedimentosDiaSemana.size() == 0 )
			{
				continue;
			}

			// Calcular quantas colunas deverão ser
			// 'saltadas' para escrever o primeiro atendimento do dia
			if ( atedimentosDiaSemana.get( 0 ) instanceof AtendimentoOperacionalDTO )
			{
				List< AtendimentoOperacionalDTO > listOp
					= new ArrayList< AtendimentoOperacionalDTO >();

				for ( AtendimentoRelatorioDTO ar : atedimentosDiaSemana )
				{
					listOp.add( (AtendimentoOperacionalDTO) ar );
				}

				AtendimentosServiceImpl service = new AtendimentosServiceImpl();
				row += service.deslocarLinhasExportExcel( this.instituicaoEnsino, listOp );

				atedimentosDiaSemana.clear();

				for ( AtendimentoOperacionalDTO atOp : listOp )
				{
					atedimentosDiaSemana.add( atOp );
				}
			}

			for ( AtendimentoRelatorioDTO atendimento : atedimentosDiaSemana )
			{
				HSSFCellStyle style = codigoDisciplinaToColorMap.get(
					atendimento.getDisciplinaString() );

				// Escreve célula principal
				setCell( row, col, sheet, style, itExcelCommentsPool,
					atendimento.getExcelContentVisaoSala(),
					this.getExcelCommentVisaoSala( atendimento ) );

				// Une células de acordo com a quantidade de créditos
				mergeCells( row, ( row + atendimento.getTotalCreditos() - 1 ),
					col, col, sheet, style );

				row += atendimento.getTotalCreditos();
			}
		}
				
		return ( initialRow + maxCreditos + 1 );
	}

	private String getExcelCommentVisaoSala( AtendimentoRelatorioDTO atendimento )
	{
		String creditos = HtmlUtils.htmlUnescape( "Cr&eacute;dito(s) " );
		String teorico = HtmlUtils.htmlUnescape( "Te&oacute;rico(s)" );
		String pratico = HtmlUtils.htmlUnescape( "Pr&aacute;tico(s)" );
		String periodo = HtmlUtils.htmlUnescape( "Per&iacute;odo: " );
		String horario = HtmlUtils.htmlUnescape( "Hor&aacute;rio: " );

		if ( atendimento instanceof AtendimentoTaticoDTO )
		{
			return atendimento.getDisciplinaNome() + "\n"
				+ "Turma: " + atendimento.getTurma() + "\n"
				+ creditos + ( ( atendimento.isTeorico() ) ? teorico : pratico )
				+ ": " + atendimento.getTotalCreditos() + " de "
				+ atendimento.getTotalCreditoDisciplina() + "\n"
				+ "Curso: " + atendimento.getCursoNome() + "\n"
				+ "Matriz Curricular: " + atendimento.getCurriculoString() + "\n"
				+ periodo + atendimento.getPeriodoString() + "\n" 
				+ "Quantidade: " + atendimento.getQuantidadeAlunosString();
		}
		else if ( atendimento instanceof AtendimentoOperacionalDTO )
		{
			AtendimentoOperacionalDTO atOp = (AtendimentoOperacionalDTO) atendimento;

			return atOp.getDisciplinaNome() + "\n"
				+ "Turma: " + atOp.getTurma() + "\n"
				+ horario + atOp.getHorarioString() + "\n"
				+ creditos + ( ( atOp.getCreditoTeoricoBoolean() ) ? teorico : pratico )
				+ ": " + atOp.getTotalCreditos() + " de " + atOp.getTotalCreditoDisciplina() + "\n"
				+ "Curso: " + atOp.getCursoNome() + "\n"
				+ "Matriz Curricular: " + atOp.getCurriculoString() + "\n"
				+ periodo + atOp.getPeriodoString() + "\n"
				+ "Quantidade: " + atOp.getQuantidadeAlunosString() + "\n"
				+ "Sala: " + atOp.getSalaString() + "\n"
				+ "Professor: " + ( atOp.getProfessorId() != null ?
					atOp.getProfessorString() : atOp.getProfessorVirtualString() );
		}

		return "";
	}

	private int writeHeader( Sala sala, Turno turno, int row, HSSFSheet sheet )
	{
		int col = 3;

		// Campus
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ],
			HtmlUtils.htmlUnescape( this.getI18nConstants().campus() ) );
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ],
			sala.getUnidade().getCampus().getCodigo() );

		// Sala
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ],
			HtmlUtils.htmlUnescape( this.getI18nConstants().sala() ) );
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ],
			sala.getCodigo() );

		// Capacidade
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ],
			HtmlUtils.htmlUnescape( this.getI18nConstants().capacidade() ) );
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ],
			sala.getCapacidade() );

		row++;
		col = 3;

		// Unidade
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ],
			HtmlUtils.htmlUnescape( this.getI18nConstants().unidade() ) );
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ],
			sala.getUnidade().getCodigo() );

		// Turno
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ],
			HtmlUtils.htmlUnescape( this.getI18nConstants().turno() ) );
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ],
			turno.getNome() );

		// Tipo
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ],
			HtmlUtils.htmlUnescape( this.getI18nConstants().tipo() ) );
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ],
			sala.getTipoSala().getNome() );

		row++;
		col = 2;

		// Créditos
		setCell( row, col++, sheet, this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_TEXT.ordinal() ],
			HtmlUtils.htmlUnescape( this.getI18nConstants().creditos() ) );

		// Dias Semana
		for ( Semanas semanas : Semanas.values() )
		{
			setCell( row, col++, sheet,
				this.cellStyles[ ExcelCellStyleReference.HEADER_CENTER_TEXT.ordinal() ],
				semanas.name() );
		}

		row++;
		return row;
	}

	private void fillInCellStyles( HSSFSheet sheet )
	{
		for ( ExcelCellStyleReference cellStyleReference : ExcelCellStyleReference.values() )
		{
			this.cellStyles[ cellStyleReference.ordinal() ] = getCell(
				cellStyleReference.getRow(), cellStyleReference.getCol(), sheet ).getCellStyle();
		}
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

		return colorPalleteCellStylesList;
	}

	private List< HSSFComment > buildExcelCommentsPool( HSSFWorkbook workbook )
	{
		List< HSSFComment > excelCommentsPool
			= new ArrayList< HSSFComment >();

		HSSFSheet sheet = workbook.getSheet(
			ExcelInformationType.RELATORIO_VISAO_SALA.getSheetName() );

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

	private List< AtendimentoRelatorioDTO > ordenaHorarioAula(
		List< AtendimentoRelatorioDTO > list )
	{
		if ( list == null || list.size() == 0 )
		{
			return Collections.< AtendimentoRelatorioDTO > emptyList();
		}
			
		if ( list.get( 0 ) instanceof AtendimentoTaticoDTO )
		{
			return list;
		}

		List< AtendimentoOperacionalDTO > opList
			= new ArrayList< AtendimentoOperacionalDTO >();

		for ( AtendimentoRelatorioDTO ar : list )
		{
			opList.add( (AtendimentoOperacionalDTO) ar );
		}

		AtendimentosServiceImpl service = new AtendimentosServiceImpl();
		opList = service.ordenaPorHorarioAula( opList );

		List< AtendimentoRelatorioDTO > result
			= new ArrayList< AtendimentoRelatorioDTO >( opList );

		return result;
	}
}
