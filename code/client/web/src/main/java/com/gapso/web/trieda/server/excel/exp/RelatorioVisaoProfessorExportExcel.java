package com.gapso.web.trieda.server.excel.exp;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;

import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFComment;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.ProfessorVirtual;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.AtendimentosServiceImpl;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.excel.ExcelInformationType;
import com.gapso.web.trieda.shared.i18n.TriedaI18nConstants;
import com.gapso.web.trieda.shared.i18n.TriedaI18nMessages;


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

	private HSSFCellStyle [] cellStyles;
	private boolean removeUnusedSheets;
	private String sheetName;
	private int initialRow;
	private RelatorioVisaoProfessorFiltroExcel relatorioFiltro;
	private Campus campus;
	private boolean isVisaoProfessor;

	public RelatorioVisaoProfessorExportExcel(
		Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, boolean isVisaoProfessor,
		InstituicaoEnsino instituicaoEnsino )
	{
		this( true, cenario, i18nConstants, i18nMessages, null, isVisaoProfessor, instituicaoEnsino );
	}

	public RelatorioVisaoProfessorExportExcel(
		Cenario cenario, TriedaI18nConstants i18nConstants,
		TriedaI18nMessages i18nMessages, ExportExcelFilter filter,
		boolean isVisaoProfessor, InstituicaoEnsino instituicaoEnsino )
	{
		this( true, cenario, i18nConstants, i18nMessages, filter, isVisaoProfessor, instituicaoEnsino );
	}

	public RelatorioVisaoProfessorExportExcel(
		boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		boolean isVisaoProfessor, InstituicaoEnsino instituicaoEnsino )
	{
		this( removeUnusedSheets, cenario, i18nConstants,
			  i18nMessages, null, isVisaoProfessor, instituicaoEnsino );
	}

	public RelatorioVisaoProfessorExportExcel(
		boolean removeUnusedSheets, Cenario cenario,
		TriedaI18nConstants i18nConstants, TriedaI18nMessages i18nMessages,
		ExportExcelFilter filter, boolean isVisaoProfessor,
		InstituicaoEnsino instituicaoEnsino )
	{
		super( cenario, i18nConstants, i18nMessages, instituicaoEnsino );

		this.cellStyles = new HSSFCellStyle[ ExcelCellStyleReference.values().length ];
		this.removeUnusedSheets = removeUnusedSheets;
		this.sheetName = ExcelInformationType.RELATORIO_VISAO_PROFESSOR.getSheetName();
		this.initialRow = 5;
		this.isVisaoProfessor = isVisaoProfessor;
		this.setFilter( filter );

		if ( this.relatorioFiltro != null )
		{
			this.campus = Campus.find(
				this.relatorioFiltro.getCampusDTO().getId(),
				this.instituicaoEnsino );
		}
	}

	public boolean isVisaoProfessor()
	{
		return this.isVisaoProfessor;
	}

	public void setVisaoProfessor( boolean isVisaoProfessor )
	{
		this.isVisaoProfessor = isVisaoProfessor;
	}

	@Override
	public String getFileName()
	{
		return this.getI18nConstants().relatorioVisaoProfessor();
	}

	@Override
	protected String getPathExcelTemplate()
	{
		return "/templateExport.xls";
	}

	@Override
	protected String getReportName()
	{
		return this.getI18nConstants().relatorioVisaoProfessor();
	}

	public RelatorioVisaoProfessorFiltroExcel getFilter()
	{
		return this.relatorioFiltro;
	}

	public void setFilter( ExportExcelFilter filter )
	{
		if ( filter instanceof RelatorioVisaoProfessorFiltroExcel )
		{
			this.relatorioFiltro = (RelatorioVisaoProfessorFiltroExcel) filter;
		}
		else
		{
			this.relatorioFiltro = null;
		}
	}

	private List< AtendimentoOperacionalDTO > getAtendimentos()
	{
		boolean idAdmin = true;

		List< Professor > professores = null;
		List< ProfessorVirtual > professoresVirtuais = null;
		List< Turno > turnos = null;

		// Recupera os dados preenchidos nos filtros
		if ( relatorioFiltro != null )
		{
			Professor professor = ( relatorioFiltro.getProfessorDTO() == null ? null :
				Professor.find( relatorioFiltro.getProfessorDTO().getId(), this.instituicaoEnsino ) );

			ProfessorVirtual professorVirtual = ( relatorioFiltro.getProfessorVirtualDTO() == null ? null :
				ProfessorVirtual.find( relatorioFiltro.getProfessorVirtualDTO().getId(), this.instituicaoEnsino ) );

			if ( professor != null )
			{
				professores = new ArrayList< Professor >( 1 );
				professores.add( professor );
			}
			else if ( professorVirtual != null )
			{
				professoresVirtuais = new ArrayList< ProfessorVirtual >( 1 );
				professoresVirtuais.add( professorVirtual );
			}

			Turno turno = ( relatorioFiltro.getTurnoDTO() == null ? null :
				Turno.find( relatorioFiltro.getTurnoDTO().getId(), this.instituicaoEnsino ) );

			if ( turno == null )
			{
				turnos = Turno.findAll( this.instituicaoEnsino );
			}
			else
			{
				turnos = new ArrayList< Turno >( 1 );
				turnos.add( turno );
			}
		}
		else
		{
			// Relatorio solicitado a partir da opção de 'exportar tudo'
			professores = Professor.findAll( this.instituicaoEnsino );
			professoresVirtuais = ProfessorVirtual.findAll( this.instituicaoEnsino );
			turnos = Turno.findAll( this.instituicaoEnsino );
		}

		Set< AtendimentoOperacional > atendimentosOperacional
			= new HashSet< AtendimentoOperacional >();

		// Atendimentos operacionais dos professores
		if ( professores != null && professores.size() > 0 )
		{
			for ( Professor professor : professores )
			{
				for ( Turno turno : turnos )
				{
					List< AtendimentoOperacional > atendimentos
						= AtendimentoOperacional.getAtendimentosOperacional( instituicaoEnsino,
							idAdmin, professor, null, turno, this.isVisaoProfessor() );

					atendimentosOperacional.addAll( atendimentos );
				}
			}
		}

		// Atendimentos operacionais dos professores virtuais
		if ( professoresVirtuais != null && professoresVirtuais.size() > 0 )
		{
			for ( ProfessorVirtual professorVirtual : professoresVirtuais )
			{
				for ( Turno turno : turnos )
				{
					List< AtendimentoOperacional > atendimentos
						= AtendimentoOperacional.getAtendimentosOperacional( instituicaoEnsino,
							idAdmin, null, professorVirtual, turno, this.isVisaoProfessor() );

					atendimentosOperacional.addAll( atendimentos );
				}
			}
		}

		List< AtendimentoOperacionalDTO > atendimentosOperacionalDTO
			= new ArrayList< AtendimentoOperacionalDTO >();

		for ( AtendimentoOperacional domain : atendimentosOperacional )
		{
			atendimentosOperacionalDTO.add(
				ConvertBeans.toAtendimentoOperacionalDTO( domain ) );
		}

		AtendimentosServiceImpl service = new AtendimentosServiceImpl();
		atendimentosOperacionalDTO = service.ordenaPorHorarioAula( atendimentosOperacionalDTO );

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

			// Dado o id de um professor ( ou professor virtual ),
			// temos os seus atendimentos, organizados por turno
			Map< Long, Map< Turno, Map< SemanaLetiva, List< AtendimentoOperacionalDTO > > > > mapNivel1
				= new TreeMap< Long, Map< Turno, Map< SemanaLetiva, List< AtendimentoOperacionalDTO > > > >();

			for ( AtendimentoOperacionalDTO atendimento : atendimentos )
			{
				Professor professor = null;
				ProfessorVirtual professorVirtual = null;

				if ( this.campus == null )
				{
					this.campus = Campus.find(
						atendimento.getCampusId(), this.instituicaoEnsino );
				}

				if ( atendimento.getProfessorId() != null )
				{
					professor = Professor.find(
						atendimento.getProfessorId(), this.instituicaoEnsino );
				}
				else
				{
					professorVirtual = ProfessorVirtual.find(
						atendimento.getProfessorVirtualId(), this.instituicaoEnsino );
				}

				Long professorId = ( professor == null ? professorVirtual.getId() : professor.getId() );
				Turno turno = Turno.find( atendimento.getTurnoId(), this.instituicaoEnsino );
				SemanaLetiva semanaLetiva = SemanaLetiva.find( atendimento.getSemanaLetivaId(), this.instituicaoEnsino );

				Map< Turno, Map< SemanaLetiva, List< AtendimentoOperacionalDTO > > > mapNivel2 = mapNivel1.get( professorId );

				if ( mapNivel2 == null )
				{
					mapNivel2 = new HashMap< Turno, Map< SemanaLetiva, List< AtendimentoOperacionalDTO > > >();
					mapNivel1.put( professorId, mapNivel2 );
				}

				Map< SemanaLetiva, List< AtendimentoOperacionalDTO > > mapNivel3 = mapNivel2.get( turno );

				if ( mapNivel3 == null )
				{
					mapNivel3 = new HashMap< SemanaLetiva, List< AtendimentoOperacionalDTO > >();
					mapNivel2.put( turno, mapNivel3 );
				}

				List< AtendimentoOperacionalDTO > list = mapNivel3.get( semanaLetiva );
				
				if ( list == null )
				{
					list = new ArrayList< AtendimentoOperacionalDTO >();
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

			for ( Long profId : mapNivel1.keySet() )
			{
				Map< Turno, Map< SemanaLetiva, List< AtendimentoOperacionalDTO > > > mapNivel2 = mapNivel1.get( profId );

				for ( Turno turno : mapNivel2.keySet() )
				{
					Map< SemanaLetiva, List< AtendimentoOperacionalDTO > > mapNivel3 = mapNivel2.get( turno );

					for ( SemanaLetiva semanaLetiva : mapNivel3.keySet() )
					{
						List< AtendimentoOperacionalDTO > listAtendimentos = mapNivel3.get( semanaLetiva ); 

						Professor professor = null;
						ProfessorVirtual professorVirtual = null;
	
						professor = Professor.find( profId, this.instituicaoEnsino );
	
						if ( professor == null )
						{
							professorVirtual = ProfessorVirtual.find(
								profId, this.instituicaoEnsino );
						}
	
						if ( professor != null )
						{
							nextRow = writeProfessor( this.campus, professor, turno, semanaLetiva, listAtendimentos,
								nextRow, sheet, itExcelCommentsPool, codigoDisciplinaToColorMap );
						}
						else if ( professorVirtual != null )
						{
							nextRow = writeProfessorVirtual(
								this.campus, professorVirtual, turno, semanaLetiva, listAtendimentos,
								nextRow, sheet, itExcelCommentsPool, codigoDisciplinaToColorMap );
						}
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

	private int writeProfessor(
		Campus campus, Professor professor,
		Turno turno, SemanaLetiva semanaLetiva,
		List< AtendimentoOperacionalDTO > atendimentos,
		int row, HSSFSheet sheet, Iterator< HSSFComment > itExcelCommentsPool,
		Map< String, HSSFCellStyle > codigoDisciplinaToColorMap )
	{
		row = writeHeaderProfessor(
			campus, professor, turno, row, sheet );

		int initialRow = row;
		int col = 2;

		// Preenche grade com créditos e células vazias
		int maxCreditos = semanaLetiva.calculaMaxCreditos();

		for ( int indexCredito = 1;
			  indexCredito <= maxCreditos; indexCredito++ )
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

		// Processa os atendimentos lidos do BD para que
		// os mesmos sejam visualizados na visão professor
		AtendimentosServiceImpl atendimentosService = new AtendimentosServiceImpl();
		List< AtendimentoOperacionalDTO > atendimentosParaVisaoProfessor
			= atendimentosService.montaListaParaVisaoProfessor( atendimentos );

		atendimentosParaVisaoProfessor
			= this.ordenaHorarioAula( atendimentosParaVisaoProfessor );

		// Agrupa os atendimentos por dia da semana
		Map< Integer, List< AtendimentoOperacionalDTO > > diaSemanaToAtendimentosMap
			= new HashMap< Integer, List< AtendimentoOperacionalDTO > >();

		for ( AtendimentoOperacionalDTO atendimento
			: atendimentosParaVisaoProfessor )
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

			row += atendimentosService.deslocarLinhasExportExcel(
				this.instituicaoEnsino, atedimentosDiaSemana );

			for ( AtendimentoOperacionalDTO atendimento : atedimentosDiaSemana )
			{
				HSSFCellStyle style = codigoDisciplinaToColorMap.get(
					atendimento.getDisciplinaString() );

				// Escreve célula principal
				setCell( row, col, sheet, style, itExcelCommentsPool,
					atendimento.getExcelContentVisaoProfessor(),
					this.getExcelCommentVisaoProfessor( atendimento ) );

				// Une células de acordo com a quantidade de créditos
				mergeCells( row, ( row + atendimento.getTotalLinhas() - 1 ),
					col, col, sheet, style );

				row += atendimento.getTotalLinhas();
			}
		}

		return ( initialRow + maxCreditos + 1 );
	}
	
	private String getExcelCommentVisaoProfessor( AtendimentoOperacionalDTO atendimento )
	{
		String horario = HtmlUtils.htmlUnescape( "Hor&aacute;rio: " );
		String creditos = HtmlUtils.htmlUnescape( "Cr&eacute;dito(s) " );
		String teorico = HtmlUtils.htmlUnescape( "Te&oacute;rico(s)" );
		String pratico = HtmlUtils.htmlUnescape( "Pr&aacute;tico(s)" );
		String periodo = HtmlUtils.htmlUnescape( "Per&iacute;odo: " );
	
		return atendimento.getDisciplinaNome() + "\n"
			+ "Turma: " + atendimento.getTurma() + "\n"
			+ horario + atendimento.getHorarioString() + "\n"
			+ creditos + ( ( atendimento.getCreditoTeoricoBoolean() ) ? teorico : pratico )
			+ ": " + atendimento.getTotalCreditos() + " de " + atendimento.getTotalCreditoDisciplina() + "\n"
			+ "Curso: " + atendimento.getCursoNome() + "\n"
			+ "Matriz Curricular: " + atendimento.getCurriculoString() + "\n"
			+ periodo + atendimento.getPeriodoString() + "\n"
			+ "Quantidade: " + atendimento.getQuantidadeAlunosString() + "\n"
			+ "Sala: " + atendimento.getSalaString() + "\n"
			+ "Professor: " + ( atendimento.getProfessorId() != null ?
				atendimento.getProfessorString() : atendimento.getProfessorVirtualString() );
	}

	private int writeProfessorVirtual(
		Campus campus, ProfessorVirtual professorVirtual,
		Turno turno, SemanaLetiva semanaLetiva,
		List< AtendimentoOperacionalDTO > atendimentos, int row,
		HSSFSheet sheet, Iterator< HSSFComment > itExcelCommentsPool,
		Map< String,HSSFCellStyle > codigoDisciplinaToColorMap )
	{
		row = writeHeaderProfessorVirtual(
			campus, professorVirtual, turno, row, sheet );

		int initialRow = row;
		int col = 2;

		// Preenche grade com créditos e células vazias
		int maxCreditos = semanaLetiva.calculaMaxCreditos();

		for ( int indexCredito = 1;
			  indexCredito <= maxCreditos; indexCredito++ )
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
		// que os mesmos sejam visualizados na visão professor
		AtendimentosServiceImpl atendimentosService = new AtendimentosServiceImpl();
		List< AtendimentoOperacionalDTO > atendimentosParaVisaoProfessor
			= atendimentosService.montaListaParaVisaoProfessor( atendimentos );

		atendimentosParaVisaoProfessor
			= this.ordenaHorarioAula( atendimentosParaVisaoProfessor );

		// Agrupa os atendimentos por dia da semana
		Map< Integer, List< AtendimentoOperacionalDTO > > diaSemanaToAtendimentosMap
			= new HashMap< Integer, List< AtendimentoOperacionalDTO > >();

		for ( AtendimentoOperacionalDTO atendimento
			: atendimentosParaVisaoProfessor )
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

			row += atendimentosService.deslocarLinhasExportExcel(
				this.instituicaoEnsino, atedimentosDiaSemana );

			for ( AtendimentoOperacionalDTO atendimento : atedimentosDiaSemana )
			{
				HSSFCellStyle style = codigoDisciplinaToColorMap.get(
					atendimento.getDisciplinaString() );

				// Escreve célula principal
				setCell( row, col, sheet, style, itExcelCommentsPool,
					atendimento.getExcelContentVisaoProfessor(),
					this.getExcelCommentVisaoProfessor( atendimento ) );

				// Une células de acordo com a quantidade de créditos
				mergeCells( row, ( row + atendimento.getTotalCreditos() - 1 ),
					col, col, sheet, style );

				row += atendimento.getTotalCreditos();
			}
		}

		return ( initialRow + maxCreditos + 1 );
	}

	private int writeHeaderProfessor(
		Campus campus, Professor professor,
		Turno turno, int row, HSSFSheet sheet )
	{
		int col = 3;

		// Campus
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ], this.getI18nConstants().campus() );
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ], campus.getCodigo() );

		// Professor
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ], this.getI18nConstants().professor() );
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ], professor.getNome() );

		row++;
		col = 3;

		// Turno
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ], this.getI18nConstants().turno() );
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ], turno.getNome() );

		// Professor Virtual
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ],
			this.getI18nConstants().professorVirtual() );
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ], "" );

		row++;
		col = 2;

		// Créditos
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_CENTER_TEXT.ordinal() ], this.getI18nConstants().creditos() );

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
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ], this.getI18nConstants().campus() );
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ], campus.getCodigo() );

		// Professor
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ], this.getI18nConstants().professor() );
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ], "" );

		row++;
		col = 3;

		// Turno
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ], this.getI18nConstants().turno() );
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ], turno.getNome() );

		// Professor Virtual
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_LEFT_TEXT.ordinal() ], this.getI18nConstants().professorVirtual() );
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_CENTER_VALUE.ordinal() ], professorVirtual.getNome() );

		row++;
		col = 2;

		// Créditos
		setCell( row, col++, sheet, cellStyles[ ExcelCellStyleReference.HEADER_CENTER_TEXT.ordinal() ], this.getI18nConstants().creditos() );

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

	private List< AtendimentoOperacionalDTO > ordenaHorarioAula(
		List< AtendimentoOperacionalDTO > list )
	{
		if ( list == null || list.size() == 0 )
		{
			return Collections.< AtendimentoOperacionalDTO > emptyList();
		}
		
		List< AtendimentoOperacionalDTO > opList
			= new ArrayList< AtendimentoOperacionalDTO >( list );

		AtendimentosServiceImpl service = new AtendimentosServiceImpl();
		opList = service.ordenaPorHorarioAula( opList );

		return opList;
	}
}
