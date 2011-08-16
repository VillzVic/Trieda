package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

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
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.AtendimentosServiceImpl;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoTaticoDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;

public class RelatorioVisaoCursoExportExcel
	extends AbstractExportExcel
{
	enum ExcelCellStyleReference
	{
		HEADER_LEFT_TEXT( 5, 3 ),
		HEADER_CENTER_VALUE( 5, 4 ),
		HEADER_CENTER_TEXT( 7, 2 ),
		TEXT( 8, 2 );

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
	private RelatorioVisaoCursoFiltroExcel relatorioFiltro;

	public RelatorioVisaoCursoExportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages )
	{
		this( true, cenario, i18nConstants, i18nMessages, null );
	}

	public RelatorioVisaoCursoExportExcel( boolean removeUnusedSheets,
		Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages )
	{
		this( removeUnusedSheets, cenario, i18nConstants, i18nMessages, null );
	}

	public RelatorioVisaoCursoExportExcel( Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages, ExportExcelFilter filter )
	{
		this( true, cenario, i18nConstants, i18nMessages, filter );
	}

	public RelatorioVisaoCursoExportExcel( boolean removeUnusedSheets,
			Cenario cenario, TriedaI18nConstants i18nConstants,
			TriedaI18nMessages i18nMessages, ExportExcelFilter filter )
	{
		super( cenario, i18nConstants, i18nMessages );

		this.cellStyles = new HSSFCellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.sheetName = ExcelInformationType.RELATORIO_VISAO_CURSO.getSheetName();
		this.initialRow = 5;
		this.setFilter( filter );
	}

	public ExportExcelFilter getFilter()
	{
		return relatorioFiltro;
	}

	public void setFilter( ExportExcelFilter filter )
	{
		if ( filter instanceof RelatorioVisaoCursoFiltroExcel )
		{
			this.relatorioFiltro = (RelatorioVisaoCursoFiltroExcel)filter;
		}
		else
		{
			this.relatorioFiltro = null;
		}
	}

	@Override
	public String getFileName()
	{
		return getI18nConstants().relatorioVisaoCurso();
	}

	@Override
	protected String getPathExcelTemplate()
	{
		return "/templateExport.xls";
	}

	@Override
	protected String getReportName()
	{
		return getI18nConstants().relatorioVisaoCurso();
	}

	public Set< Map< String, Object > > opcoesBuscaOperacional( Cenario cenario )
	{
		List< AtendimentoTatico > atdTaticoList
			= AtendimentoTatico.findByCenario( cenario );

		List< AtendimentoOperacional > atdOperacionalList
			= AtendimentoOperacional.findByCenario( cenario );

		List< AtendimentoRelatorioDTO > atdRelatorioList
			= new ArrayList< AtendimentoRelatorioDTO >(
				atdTaticoList.size() + atdOperacionalList.size() );

		for ( AtendimentoTatico atdTatico : atdTaticoList )
		{
			atdRelatorioList.add(
				ConvertBeans.toAtendimentoTaticoDTO( atdTatico ) );
		}

		for ( AtendimentoOperacional atdOperacional : atdOperacionalList )
		{
			atdRelatorioList.add(
				ConvertBeans.toAtendimentoOperacionalDTO( atdOperacional ) );
		}

		Set< Map< String, Object > > opcoes = new HashSet< Map< String, Object > >();

		for ( AtendimentoRelatorioDTO atendimentoRelatorio : atdRelatorioList )
		{
			Map< String, Object > opcao = new HashMap< String, Object >();

			opcao.put( "CurriculoDTO", atendimentoRelatorio.getCurriculoId() );
			opcao.put( "Periodo", Integer.valueOf( atendimentoRelatorio.getPeriodoString() ) );
			opcao.put( "TurnoDTO", atendimentoRelatorio.getTurnoId() );
			opcao.put( "CampusDTO", atendimentoRelatorio.getCampusId() );
			opcao.put( "CursoDTO", atendimentoRelatorio.getCursoId() );

			opcoes.add( opcao );
		}

		return opcoes;
	}

	private Set< Map< String, Object > > filtraAtendimentos( Set< Map< String, Object > > opcoes )
	{
		Set< Map< String, Object > > result = new HashSet< Map< String, Object > >();

		for ( Map< String, Object > opcao : opcoes )
		{
			Long curriculoId = (Long) opcao.get( "CurriculoDTO" );
			Integer periodo = (Integer) opcao.get( "Periodo" );
			Long turnoId = (Long) opcao.get( "TurnoDTO" );
			Long campusId = (Long) opcao.get( "CampusDTO" );
			Long cursoId = (Long) opcao.get( "CursoDTO" );

			if ( curriculoId == this.relatorioFiltro.getCurriculoDTO().getId()
				&& periodo == this.relatorioFiltro.getPeriodo()
				&& turnoId == this.relatorioFiltro.getTurnoDTO().getId()
				&& campusId == this.relatorioFiltro.getCampusDTO().getId()
				&& cursoId == this.relatorioFiltro.getCursoDTO().getId() )
			{
				result.add( opcao );
			}
		}

		return result;
	}

	private ParDTO< List< List< AtendimentoRelatorioDTO > >,
		List< List< Integer > > > getAtendimentoRelatorioDTOList( Cenario cenario )
	{
		List< List< AtendimentoRelatorioDTO > > atdRelatoriosList
			= new ArrayList< List< AtendimentoRelatorioDTO > >();
		List< List< Integer > > tamanhosSemanaList = new ArrayList< List< Integer > >();

		AtendimentosServiceImpl atendimentosServiceImpl = new AtendimentosServiceImpl();
		Set< Map< String, Object > > opcoes = opcoesBuscaOperacional( cenario );

		if ( this.relatorioFiltro != null )
		{
			opcoes = this.filtraAtendimentos( opcoes );
		}

		for ( Map< String, Object > opcao : opcoes )
		{
			Long curriculoId = (Long) opcao.get( "CurriculoDTO" );
			Curriculo curriculo = Curriculo.find( curriculoId );
			CurriculoDTO curriculoDTO = ConvertBeans.toCurriculoDTO( curriculo );

			Integer periodo = (Integer) opcao.get( "Periodo" );

			Long turnoId = (Long) opcao.get( "TurnoDTO" );
			Turno turno = Turno.find( turnoId );
			TurnoDTO turnoDTO = ConvertBeans.toTurnoDTO( turno );

			Long campusId = (Long) opcao.get( "CampusDTO" );
			Campus campus = Campus.find( campusId );
			CampusDTO campusDTO = ConvertBeans.toCampusDTO( campus );

			Long cursoId = (Long) opcao.get( "CursoDTO" );
			Curso curso = Curso.find( cursoId );
			CursoDTO cursoDTO = ConvertBeans.toCursoDTO( curso );

			ParDTO< List< AtendimentoRelatorioDTO >, List< Integer > > parAtendimentos
				= atendimentosServiceImpl.getBusca( curriculoDTO, periodo, turnoDTO, campusDTO, cursoDTO );

			List< AtendimentoRelatorioDTO > atdRelatorioList
				= new ArrayList< AtendimentoRelatorioDTO >( parAtendimentos.getPrimeiro() );
			atdRelatoriosList.add( atdRelatorioList );

			List< Integer > tamanhoSemanaList
				= new ArrayList< Integer >( parAtendimentos.getSegundo() );
			tamanhosSemanaList.add( tamanhoSemanaList );
		}

		ParDTO< List< List< AtendimentoRelatorioDTO > >, List< List< Integer > > > par
			= new ParDTO< List< List< AtendimentoRelatorioDTO > >, List< List< Integer > > >();

		par.setPrimeiro( atdRelatoriosList );
		par.setSegundo( tamanhosSemanaList );

		return par;
	}

	@Override
	protected boolean fillInExcel( HSSFWorkbook workbook )
	{
		boolean result = false;

		Cenario cenario = getCenario();
		ParDTO< List< List< AtendimentoRelatorioDTO > >, List< List< Integer > > > par
			= getAtendimentoRelatorioDTOList( cenario );

		List< List< AtendimentoRelatorioDTO > > atdRelatoriosList = par.getPrimeiro();
		List< List< Integer > > tamanhosSemanaList = par.getSegundo();

		if ( !atdRelatoriosList.isEmpty() )
		{
			HSSFSheet sheet = workbook.getSheet( this.sheetName );
			fillInCellStyles( sheet );

			List< HSSFComment > excelCommentsPool = buildExcelCommentsPool( workbook );
			Iterator< HSSFComment > itExcelCommentsPool = excelCommentsPool.iterator();
			List< HSSFCellStyle > excelColorsPool = buildColorPaletteCellStyles( workbook );

			Map< String, HSSFCellStyle > codigoDisciplinaToColorMap
				= new HashMap< String, HSSFCellStyle >();

			for ( List< AtendimentoRelatorioDTO > atdRelatorioList : atdRelatoriosList )
			{
				for ( AtendimentoRelatorioDTO atendimento : atdRelatorioList )
				{
					HSSFCellStyle style = codigoDisciplinaToColorMap.get(
						atendimento.getDisciplinaString() );

					if ( style == null )
					{
						int index = ( codigoDisciplinaToColorMap.size() % excelColorsPool.size() );

						codigoDisciplinaToColorMap.put(
							atendimento.getDisciplinaString(),
							excelColorsPool.get( index ) );
					}
				}
			}

			int nextRow = this.initialRow;

			for ( int i = 0; i < atdRelatoriosList.size(); i++ )
			{
				List< AtendimentoRelatorioDTO > atdRelatorioList
					= atdRelatoriosList.get( i );

				List< Integer > tamanhoSemanaList = tamanhosSemanaList.get( i );

				if ( atdRelatorioList.isEmpty() )
				{
					continue;
				}

				Oferta oferta = Oferta.find(
					atdRelatorioList.get( 0 ).getOfertaId() );

				Integer periodo = Integer.valueOf(
					atdRelatorioList.get( 0 ).getPeriodoString() );

				nextRow = this.writeSala( oferta, periodo, atdRelatorioList,
					tamanhoSemanaList, nextRow, sheet, itExcelCommentsPool,
					codigoDisciplinaToColorMap );
			}

			result = true;
		}

		if ( this.removeUnusedSheets )
		{
			removeUnusedSheets( this.sheetName, workbook );
		}

		return result;
	}

	@SuppressWarnings("unused")
	private int writeSala( Oferta oferta, Integer periodo,
		List< AtendimentoRelatorioDTO > atendimentos, List< Integer > tamanhoSemanaList,
		int row, HSSFSheet sheet, Iterator< HSSFComment > itExcelCommentsPool,
		Map< String, HSSFCellStyle > codigoDisciplinaToColorMap )
	{
		row = writeHeader( oferta, periodo, tamanhoSemanaList, row, sheet );

		int initialRow = row;
		int col = 2;

		// Preenche grade com créditos e células vazias
		int maxCreditos = oferta.getTurno().calculaMaxCreditos();
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

		// Agrupa os atendimentos por dia da semana
		Map< Integer, List< AtendimentoRelatorioDTO > > diaSemanaToAtendimentosMap
			= new HashMap< Integer, List< AtendimentoRelatorioDTO > >();

		for ( AtendimentoRelatorioDTO atendimento : atendimentos )
		{
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
			
			// Obs.: O valor do dia da semana aqui NÃO é interpretado exatamente
			// como o dia em que a aula é ministrada, mas sim a coluna da planilha
			// na qual o atendimento será impresso. Quando há mais de 4 créditos
			// alocados no mesmo dia da semana, a planilha irá utilizar mais de uma
			// coluna para esse dia da semana. Logo, teremos colunas com valor superior
			// a 8 (o que não é consistente com o valor de dia da semana.
			col = diaSemanaInt + 1;

			for ( AtendimentoRelatorioDTO atendimento
				: diaSemanaToAtendimentosMap.get( diaSemanaInt ) )
			{
				HSSFCellStyle style = codigoDisciplinaToColorMap.get(
					atendimento.getDisciplinaString() );

				// Escreve célula principal
				setCell( row, col, sheet, style, itExcelCommentsPool,
					atendimento.getExcelContentVisaoCurso(),
					this.getExcelCommentVisaoCurso( atendimento ) );

				// Une células de acordo com a quantidade de créditos
				mergeCells( row, ( row + atendimento.getTotalCreditos() - 1 ),
					col, col, sheet, style );

				row += atendimento.getTotalCreditos();
			}
		}

		return ( initialRow + maxCreditos + 1 );
	}

	private String getExcelCommentVisaoCurso( AtendimentoRelatorioDTO atendimento )
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
				+ ": " + atendimento.getTotalCreditos() + " de " + atendimento.getTotalCreditoDisciplina() + "\n"
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
				+ ": " + atOp.getTotalCreditos() + " de " + atOp.getTotalCreditos() + "\n"
				+ "Curso: " + atOp.getCursoNome() + "\n"
				+ "Matriz Curricular: " + atOp.getCurriculoString() + "\n"
				+ periodo + atOp.getPeriodoString() + "\n"
				+ "Quantidade: " + atOp.getQuantidadeAlunosString();
		}
		
		return "";
	}
	
	private int writeHeader( Oferta oferta, Integer periodo,
		List< Integer > tamanhoSemanaList, int row, HSSFSheet sheet )
	{
		int col = 3;

		// Curso
		setCell( row, col++, sheet,
			cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ],
			HtmlUtils.htmlUnescape( getI18nConstants().curso() ) );

		setCell( row, col++, sheet,
			cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ],
			oferta.getCurriculo().getCurso().getCodigo() );

		// Campus
		setCell(row, col++, sheet,
			cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ],
			HtmlUtils.htmlUnescape( getI18nConstants().campus() ) );

		setCell( row, col++, sheet,
			cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ],
			oferta.getCampus().getCodigo() );

		// Turno
		setCell( row, col++, sheet,
			cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ],
			HtmlUtils.htmlUnescape( getI18nConstants().turno() ) );

		setCell( row, col++, sheet,
			cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ],
			oferta.getTurno().getNome() );

		row++;
		col = 3;

		// Curriculo
		setCell( row, col++, sheet,
			cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ],
			HtmlUtils.htmlUnescape( getI18nConstants().matrizCurricular() ) );

		setCell( row, col++, sheet,
			cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ],
			oferta.getCurriculo().getCodigo() );

		// Periodo
		setCell( row, col++, sheet,
			cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ],
			HtmlUtils.htmlUnescape( getI18nConstants().periodo() ) );

		setCell( row, col++, sheet,
			cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ], periodo );

		row++;
		col = 2;

		// Créditos
		setCell( row, col++, sheet,
			cellStyles[ ExcelCellStyleReference.HEADER_CENTER_TEXT.ordinal() ],
			HtmlUtils.htmlUnescape( getI18nConstants().creditos() ) );

		// Dias Semana
		for ( Semanas semanas : Semanas.values() )
		{
			int qtd = tamanhoSemanaList.get( Semanas.toInt( semanas ) );
			setCell( row, col, sheet, semanas.name() );
			HSSFCellStyle style = cellStyles[ ExcelCellStyleReference.HEADER_CENTER_TEXT.ordinal() ];
			mergeCells( row, row, col, col + qtd - 1, sheet, style );
			col = col + qtd;
		}

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

	private List< HSSFCellStyle > buildColorPaletteCellStyles( HSSFWorkbook workbook )
	{
		List< HSSFCellStyle > colorPalleteCellStylesList = new ArrayList< HSSFCellStyle >();
		HSSFSheet sheet = workbook.getSheet(
			ExcelInformationType.PALETA_CORES.getSheetName() );

		if ( sheet != null )
		{ 
			for ( int rowIndex = sheet.getFirstRowNum();
				rowIndex <= sheet.getLastRowNum(); rowIndex++ )
			{
				HSSFRow row = sheet.getRow( rowIndex );

				if ( row != null )
				{
					HSSFCell cell = row.getCell( (int) row.getFirstCellNum() );

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
		List< HSSFComment > excelCommentsPool = new ArrayList< HSSFComment >();

		HSSFSheet sheet = workbook.getSheet(
			ExcelInformationType.RELATORIO_VISAO_CURSO.getSheetName() );

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
