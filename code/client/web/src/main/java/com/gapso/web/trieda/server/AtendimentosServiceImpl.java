package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.TreeMap;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.ProfessorVirtual;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoTaticoDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.services.AtendimentosService;
import com.gapso.web.trieda.shared.util.TriedaUtil;

public class AtendimentosServiceImpl
	extends RemoteService
	implements AtendimentosService
{
	private static final long serialVersionUID = -1505176338607927637L;

	@Override
	public PagingLoadResult< AtendimentoTaticoDTO > getList()
	{
		List< AtendimentoTaticoDTO > list
			= new ArrayList< AtendimentoTaticoDTO >();

		List< AtendimentoTatico > atendimentosTatico
			= AtendimentoTatico.findAll( getInstituicaoEnsinoUser() );

		for ( AtendimentoTatico atendimentoTatico : atendimentosTatico )
		{
			list.add( ConvertBeans.toAtendimentoTaticoDTO( atendimentoTatico ) );
		}

		BasePagingLoadResult< AtendimentoTaticoDTO > result
			= new BasePagingLoadResult< AtendimentoTaticoDTO >( list );

		result.setOffset( 0 );
		result.setTotalLength( 100 );

		return result;
	}

	@Override
	public List< AtendimentoRelatorioDTO > getBusca(
		SalaDTO salaDTO, TurnoDTO turnoDTO, SemanaLetivaDTO semanaLetivaDTO )
	{
		List< AtendimentoRelatorioDTO > arDTOList
			= new ArrayList< AtendimentoRelatorioDTO >();

		List< AtendimentoTaticoDTO > taticoList
			= getBuscaTatico( salaDTO, turnoDTO, semanaLetivaDTO );

		if ( !taticoList.isEmpty() )
		{
			for ( AtendimentoTaticoDTO atdto : taticoList )
			{
				arDTOList.add( atdto );
			}
		}
		else
		{
			List< AtendimentoOperacionalDTO > operacionalList
				= getBuscaOperacional( salaDTO, turnoDTO, semanaLetivaDTO );

			for ( AtendimentoOperacionalDTO atdto : operacionalList )
			{
				arDTOList.add( atdto );
			}
		}

		return montaListaParaVisaoSala( arDTOList );
	}

	public List< AtendimentoOperacionalDTO > getBuscaOperacional(
		SalaDTO salaDTO, TurnoDTO turnoDTO, SemanaLetivaDTO semanaLetivaDTO )
	{
		Sala sala = Sala.find( salaDTO.getId(), getInstituicaoEnsinoUser() );
		Turno turno = Turno.find( turnoDTO.getId(), getInstituicaoEnsinoUser() );
		SemanaLetiva semanaLetiva = SemanaLetiva.find( semanaLetivaDTO.getId(), getInstituicaoEnsinoUser() );

		List< AtendimentoOperacionalDTO > list
			= new ArrayList< AtendimentoOperacionalDTO >();

		List< AtendimentoOperacional > atendimentosOperacional
			= AtendimentoOperacional.findBySalaAndTurno(
				sala, turno, semanaLetiva, getInstituicaoEnsinoUser() );

		for ( AtendimentoOperacional atendimentoOperacional : atendimentosOperacional )
		{
			list.add( ConvertBeans.toAtendimentoOperacionalDTO( atendimentoOperacional ) );
		}

		return list;
	}

	public List< AtendimentoTaticoDTO > getBuscaTatico(
		SalaDTO salaDTO, TurnoDTO turnoDTO, SemanaLetivaDTO semanaLetivaDTO )
	{
		Sala sala = Sala.find( salaDTO.getId(), getInstituicaoEnsinoUser() );
		Turno turno = Turno.find( turnoDTO.getId(), getInstituicaoEnsinoUser() );
		SemanaLetiva semanaLetiva = SemanaLetiva.find( semanaLetivaDTO.getId(), getInstituicaoEnsinoUser() );

		List< AtendimentoTaticoDTO > list = new ArrayList< AtendimentoTaticoDTO >();
		List< AtendimentoTatico > atendimentosTatico = AtendimentoTatico.findBySalaAndTurno(
			getInstituicaoEnsinoUser(), sala, turno, semanaLetiva );

		for ( AtendimentoTatico atendimentoTatico : atendimentosTatico )
		{
			list.add( ConvertBeans.toAtendimentoTaticoDTO( atendimentoTatico ) );
		}

		return list;
	}

	/*
	 * Esse método tem como objetivo agrupar DTO's que
	 * correspondam a créditos de uma mesma aula, que
	 * foram armazenadas como linhas distintas no banco de dados.
	 * */
	public List< AtendimentoRelatorioDTO > transformaAtendimentosPorHorarioEmAtendimentosPorAula(
		List< AtendimentoOperacionalDTO > list )
	{
		List< AtendimentoRelatorioDTO > ret = new ArrayList< AtendimentoRelatorioDTO >();

		// Agrupa os DTOS pela chave [ Curso - Disciplina - Turma - DiaSemana - Sala ]
		Map< String, List<AtendimentoRelatorioDTO > > atendimentosDTOMap = new HashMap< String, List< AtendimentoRelatorioDTO > >();

		for ( AtendimentoRelatorioDTO dto : list )
		{
			String key = dto.getCursoNome() + "-" + dto.getDisciplinaString()
				+ "-" + dto.getTurma() + "-" + dto.getSemana() + "-" + dto.getSalaId();

			List< AtendimentoRelatorioDTO > dtoList = atendimentosDTOMap.get( key );

			if ( dtoList == null )
			{
				dtoList = new ArrayList< AtendimentoRelatorioDTO >();
				atendimentosDTOMap.put( key, dtoList );
			}

			dtoList.add( dto );
		}

		for ( Entry< String, List< AtendimentoRelatorioDTO > > entry
			: atendimentosDTOMap.entrySet() )
		{
			List< AtendimentoOperacionalDTO > ordenadoPorHorario
				= new ArrayList< AtendimentoOperacionalDTO >();

			for ( AtendimentoRelatorioDTO ar : entry.getValue() )
			{
				ordenadoPorHorario.add( (AtendimentoOperacionalDTO) ar );
			}

			ordenadoPorHorario = this.ordenaPorHorarioAula( ordenadoPorHorario );

			AtendimentoOperacionalDTO dtoMain = ordenadoPorHorario.get( 0 );

			int count = 1;
			for ( int i = 1; i < ordenadoPorHorario.size(); i++ )
			{
				AtendimentoOperacionalDTO h0 = ordenadoPorHorario.get( i - 1 );
				AtendimentoOperacionalDTO h1 = ordenadoPorHorario.get( i );

				if ( !h0.getHorarioId().equals( h1.getHorarioId() ) )
				{
					count++;
				}
			}

			dtoMain.setTotalCreditos( count );
			ret.add( dtoMain );
		}

		return ret;
	}

	public List< AtendimentoRelatorioDTO > montaListaParaVisaoSala(
		List< AtendimentoRelatorioDTO > list )
	{
		if ( !list.isEmpty()
			&& ( list.get( 0 ) instanceof AtendimentoOperacionalDTO ) )
		{
			List< AtendimentoOperacionalDTO > operacionalList
				= new ArrayList< AtendimentoOperacionalDTO >( list.size() );

			for ( AtendimentoRelatorioDTO arDTO : list )
			{
				operacionalList.add( ( AtendimentoOperacionalDTO ) arDTO );
			}

			list = transformaAtendimentosPorHorarioEmAtendimentosPorAula( operacionalList );
		}

		// Agrupa os DTOS pela chave [ Disciplina - Turma - DiaSemana ]
		Map< String, List< AtendimentoRelatorioDTO > > atendimentoTaticoDTOMap
			= new HashMap< String, List< AtendimentoRelatorioDTO > >();

		for ( AtendimentoRelatorioDTO dto : list )
		{
			String key = dto.getDisciplinaString()
				+ "-" + dto.getTurma() + "-" + dto.getSemana();

			List< AtendimentoRelatorioDTO > dtoList
				= atendimentoTaticoDTOMap.get( key );

			if ( dtoList == null )
			{
				dtoList = new ArrayList< AtendimentoRelatorioDTO >();
				atendimentoTaticoDTOMap.put( key, dtoList );
			}

			dtoList.add( dto );
		}

		// Quando há mais de um DTO por chave [Disciplina-Turma-DiaSemana],
		// concatena as informações de todos em um único DTO.
		List< AtendimentoRelatorioDTO > processedList
			= new ArrayList< AtendimentoRelatorioDTO >();

		for ( Entry< String, List< AtendimentoRelatorioDTO > > entry
			: atendimentoTaticoDTOMap.entrySet() )
		{
			if ( entry.getValue().size() == 1 )
			{
				processedList.addAll( entry.getValue() );
			}
			else
			{
				AtendimentoRelatorioDTO dtoMain = entry.getValue().get( 0 );

				for ( int i = 1; i < entry.getValue().size(); i++ )
				{
					AtendimentoRelatorioDTO dtoCurrent = entry.getValue().get( i );
					dtoMain.concatenateVisaoSala( dtoCurrent );
				}

				processedList.add( dtoMain );
			}
		}

		return processedList;
	}

	@Override
	public ParDTO< List< AtendimentoRelatorioDTO >, List< Integer > > getBusca(
		CurriculoDTO curriculoDTO, Integer periodo, TurnoDTO turnoDTO, CampusDTO campusDTO )
	{
		return this.getBusca( curriculoDTO, periodo, turnoDTO, campusDTO, null );
	}

	@Override
	public ParDTO< List< AtendimentoRelatorioDTO >, List< Integer > > getBusca(
		CurriculoDTO curriculoDTO, Integer periodo, TurnoDTO turnoDTO,
		CampusDTO campusDTO, CursoDTO cursoDTO )
	{
		ParDTO< List< AtendimentoRelatorioDTO >, List< Integer > > parDTOTempl
			= new ParDTO< List< AtendimentoRelatorioDTO >, List< Integer > >();

		parDTOTempl.setPrimeiro( new ArrayList< AtendimentoRelatorioDTO >() );

		List< AtendimentoTaticoDTO > taticoList = getBuscaTatico(
			curriculoDTO, periodo, turnoDTO, campusDTO, cursoDTO );

		if ( !taticoList.isEmpty() )
		{
			ParDTO< List< AtendimentoTaticoDTO >, List< Integer > > parDTO
				= montaListaParaVisaoCursoTatico( turnoDTO, taticoList );

			parDTOTempl.setSegundo( parDTO.getSegundo() );

			for ( AtendimentoTaticoDTO atdto : parDTO.getPrimeiro() )
			{
				parDTOTempl.getPrimeiro().add( atdto );
			}
		}
		else
		{
			List< AtendimentoOperacionalDTO > operacionalList
				= getBuscaOperacional( curriculoDTO, periodo, turnoDTO, campusDTO, cursoDTO );

			ParDTO< List< AtendimentoOperacionalDTO >, List< Integer > > parDTO
				= montaListaParaVisaoCursoOperacional( turnoDTO, operacionalList );

			parDTOTempl.setSegundo( parDTO.getSegundo() );

			List< AtendimentoOperacionalDTO > horariosOrdenados
				= this.ordenaPorHorarioAula( parDTO.getPrimeiro() );

			for ( AtendimentoOperacionalDTO atdto : horariosOrdenados )
			{
				parDTOTempl.getPrimeiro().add( atdto );
			}
		}

		return parDTOTempl;
	}

	public List< AtendimentoTaticoDTO > getBuscaTatico(
		CurriculoDTO curriculoDTO, Integer periodo,
		TurnoDTO turnoDTO, CampusDTO campusDTO, CursoDTO cursoDTO )
	{
		Curriculo curriculo = Curriculo.find( curriculoDTO.getId(), getInstituicaoEnsinoUser() );
		Turno turno = Turno.find( turnoDTO.getId(), getInstituicaoEnsinoUser() );
		Campus campus = Campus.find( campusDTO.getId(), this.getInstituicaoEnsinoUser() );

		Curso curso = null;

		if ( cursoDTO != null )
		{
			curso = Curso.find(
				cursoDTO.getId(), getInstituicaoEnsinoUser() );
		}

		List< AtendimentoTaticoDTO > list
			= new ArrayList< AtendimentoTaticoDTO >();

		List< AtendimentoTatico > atendimentosTatico = AtendimentoTatico.findBy(
			getInstituicaoEnsinoUser(), campus, curriculo, periodo, turno, curso );

		for ( AtendimentoTatico atendimentoTatico : atendimentosTatico )
		{
			list.add( ConvertBeans.toAtendimentoTaticoDTO( atendimentoTatico ) );
		}

		return list;
	}

	public List< AtendimentoOperacionalDTO > getBuscaOperacional(
		CurriculoDTO curriculoDTO, Integer periodo,
		TurnoDTO turnoDTO, CampusDTO campusDTO, CursoDTO cursoDTO )
	{
		Curriculo curriculo = Curriculo.find( curriculoDTO.getId(), getInstituicaoEnsinoUser() );
		Turno turno = Turno.find( turnoDTO.getId(), getInstituicaoEnsinoUser() );
		Campus campus = Campus.find( campusDTO.getId(), this.getInstituicaoEnsinoUser() );

		Curso curso = null;

		if ( cursoDTO != null )
		{
			curso = Curso.find(
				cursoDTO.getId(), getInstituicaoEnsinoUser() );
		}

		List< AtendimentoOperacionalDTO > list
			= new ArrayList< AtendimentoOperacionalDTO >();

		List< AtendimentoOperacional > atendimentosOperacional = AtendimentoOperacional.findBy(
			getInstituicaoEnsinoUser(), campus, curriculo, periodo, turno, curso );

		for ( AtendimentoOperacional atendimentoOperacional : atendimentosOperacional )
		{
			list.add( ConvertBeans.toAtendimentoOperacionalDTO( atendimentoOperacional ) );
		}

		list = agrupaAtendimentosOperacionalMesmaAula( list, curriculoDTO );

		return list;
	}

	private List< AtendimentoOperacionalDTO > agrupaAtendimentosOperacionalMesmaAula(
		List< AtendimentoOperacionalDTO > list, CurriculoDTO curriculoDTO  )
	{
		// Agrupa os DTOS pela chave [ Curso - Disciplina - Turma - DiaSemana - Sala ]
		Map< String, List< AtendimentoOperacionalDTO > > atendimentoOperacionalDTOMap
			= new HashMap< String, List< AtendimentoOperacionalDTO > >();

		for ( AtendimentoOperacionalDTO dto : list )
		{
			String key = dto.getCursoString() + "-" + dto.getDisciplinaString()
				+ "-" + dto.getTurma() + "-" + dto.getSemana() + "-" + dto.getSalaId();

			List< AtendimentoOperacionalDTO > dtoList
				= atendimentoOperacionalDTOMap.get( key );

			if ( dtoList == null )
			{
				dtoList = new ArrayList< AtendimentoOperacionalDTO >();
				atendimentoOperacionalDTOMap.put( key, dtoList );
			}

			dtoList.add( dto );
		}

		// Quando há mais de um DTO por chave
		// [ Curso - Disciplina - Turma - DiaSemana - Sala ],
		// concatena as informações de todos em um único DTO.
		List< AtendimentoOperacionalDTO > processedList
			= new ArrayList< AtendimentoOperacionalDTO >();

		for ( Entry< String, List< AtendimentoOperacionalDTO > > entry
			: atendimentoOperacionalDTOMap.entrySet() )
		{
			List< AtendimentoOperacionalDTO > ordenadoPorHorario = null;
			
			if ( entry.getValue().size() == 1 )
			{
				ordenadoPorHorario = new ArrayList< AtendimentoOperacionalDTO >( entry.getValue() );
			}
			else
			{
				ordenadoPorHorario = this.ordenaPorHorarioAula( entry.getValue() );
			}

			if ( ordenadoPorHorario.size() == 1 )
			{
				AtendimentoOperacionalDTO dto = ordenadoPorHorario.get( 0 );
				dto.setTotalCreditos( 1 );
				processedList.add( dto );
			}
			else
			{
				List< List< AtendimentoOperacionalDTO > > listAtendimentos
					= this.separaAtendimentosNaoConsecutivos( ordenadoPorHorario, curriculoDTO );

				for ( List< AtendimentoOperacionalDTO > atendimentos : listAtendimentos )
				{
					AtendimentoOperacionalDTO dtoMain = atendimentos.get( 0 ) ;
	
					// Procura pelo horário correspondente ao início da aula
					HorarioAula menorHorario = AtendimentoOperacional.retornaAtendimentoMenorHorarioAula(
					    ConvertBeans.toListAtendimentoOperacional( atendimentos ) );
	
					dtoMain.setHorarioId( menorHorario.getId() );
					dtoMain.setHorarioString( TriedaUtil.shortTimeString( menorHorario.getHorario() ) );
					dtoMain.setTotalCreditos( atendimentos.size() );
	
					processedList.add( dtoMain );
				}
			}
		}

		return processedList;
	}

	private List< List< AtendimentoOperacionalDTO > > separaAtendimentosNaoConsecutivos(
		List< AtendimentoOperacionalDTO > atendimentos, CurriculoDTO curriculoDTO )
	{
		SemanaLetiva semanaLetiva = SemanaLetiva.find(
			curriculoDTO.getSemanaLetivaId(), getInstituicaoEnsinoUser() );

		List< HorarioAula > horariosAulas
			= HorarioAula.findBySemanaLetiva( getInstituicaoEnsinoUser(), semanaLetiva );

		Map< Long, HorarioAula > mapIdHorario
			= HorarioAula.buildHorarioAulaIdToHorarioAulaMap( horariosAulas );

		Collections.sort( horariosAulas );
		this.ordenaPorHorarioAula( atendimentos );

		Map< AtendimentoOperacionalDTO, Integer > mapPositionHorarioAula
			= new HashMap< AtendimentoOperacionalDTO, Integer >();

		for ( AtendimentoOperacionalDTO dto : atendimentos )
		{
			HorarioAula horario = mapIdHorario.get( dto.getHorarioId() );
			Integer position = horariosAulas.indexOf( horario ); 
			mapPositionHorarioAula.put( dto, position );
		}

		List< List< AtendimentoOperacionalDTO > > result
			= new ArrayList< List< AtendimentoOperacionalDTO > >();

		result.add( new ArrayList< AtendimentoOperacionalDTO >() );
		result.get( 0 ).add( atendimentos.get( 0 ) );
		int currentPosition = mapPositionHorarioAula.get( atendimentos.get( 0 ) );

		for ( int i = 1; i < atendimentos.size(); i++ )
		{
			AtendimentoOperacionalDTO dto = atendimentos.get( i );
			Integer position = mapPositionHorarioAula.get( dto );

			if ( position != currentPosition + 1 )
			{
				result.add( new ArrayList< AtendimentoOperacionalDTO >() );
			}

			result.get( result.size() - 1 ).add( dto );

			currentPosition = position;
		}

		return result;
	}

	private ParDTO< List< AtendimentoTaticoDTO >, List< Integer > > montaListaParaVisaoCursoTatico(
		TurnoDTO turnoDTO, List< AtendimentoTaticoDTO > list )
	{
		// Agrupa os DTOS pelo dia da semana
		Map< Integer, List< AtendimentoTaticoDTO > > diaSemanaToAtendimentoTaticoDTOMap
			= new TreeMap< Integer, List< AtendimentoTaticoDTO > >();

		for ( AtendimentoTaticoDTO dto : list )
		{
			List< AtendimentoTaticoDTO > dtoList
				= diaSemanaToAtendimentoTaticoDTOMap.get( dto.getSemana() );

			if ( dtoList == null )
			{
				dtoList = new ArrayList< AtendimentoTaticoDTO >();
				diaSemanaToAtendimentoTaticoDTOMap.put(
					dto.getSemana(), dtoList );
			}

			dtoList.add( dto );
		}

		// Preenche entradas nulas do mapa
		// diaSemanaToAtendimentoTaticoDTOMap com uma lista vazia.
		for ( int i = 2; i <= 7; i++ )
		{
			if ( diaSemanaToAtendimentoTaticoDTOMap.get( i ) == null )
			{
				diaSemanaToAtendimentoTaticoDTOMap.put( i,
					Collections.< AtendimentoTaticoDTO > emptyList() );
			}
		}

		List< Integer > diaSemanaTamanhoList = new ArrayList< Integer >( 8 );
		Collections.addAll( diaSemanaTamanhoList, 1, 1, 1, 1, 1, 1, 1, 1, 1 );

		List< AtendimentoTaticoDTO > finalProcessedList
			= new ArrayList< AtendimentoTaticoDTO >();

		// Para cada dia da semana
		int countSemanaSize = 2;
		for ( Entry< Integer, List< AtendimentoTaticoDTO > > entry
			: diaSemanaToAtendimentoTaticoDTOMap.entrySet() )
		{
			List< List< AtendimentoTaticoDTO > > listListDTO
				= new ArrayList< List< AtendimentoTaticoDTO > >();

			// Verifica se o dia da semana
			// extrapola a quantidade máxima de créditos
			if ( AtendimentoTaticoDTO.countListDTOsCreditos( entry.getValue() )
					> turnoDTO.getMaxCreditos( entry.getKey() ) )
			{
				// Executa abordagem 1
				listListDTO = agrupaAtendimentosAbordagem1( entry );

				// Verifica se o dia da semana continua extrapolando a
				// quantidade máxima de créditos após a execução da abordagem 1
				if ( AtendimentoTaticoDTO.countListListDTOsCreditos( listListDTO )
					> turnoDTO.getMaxCreditos( entry.getKey() ) )
				{
					// Executa abordagem 2
					listListDTO.clear();
					listListDTO = agrupaAtendimentosAbordagem2( entry );
				}
			}
			else
			{
				for ( AtendimentoTaticoDTO dto : entry.getValue() )
				{
					dto.setSemana( countSemanaSize );
				}

				finalProcessedList.addAll( entry.getValue() );
			}

			int size = 1;
			for ( List< AtendimentoTaticoDTO > listDTOs : listListDTO )
			{
				if ( listDTOs.size() > size )
				{
					size = listDTOs.size();
				}

				AtendimentoTaticoDTO dtoMain = listDTOs.get( 0 );
				dtoMain.setSemana( countSemanaSize );

				for ( int i = 1; i < listDTOs.size(); i++ )
				{
					AtendimentoTaticoDTO dtoCurrent = listDTOs.get( i );
					dtoCurrent.setSemana( dtoMain.getSemana() + i );
				}

				finalProcessedList.addAll( listDTOs );
			}

			diaSemanaTamanhoList.add( entry.getKey(), size );
			countSemanaSize += size;
		}

		adicionaDadosCompartilhamentoSalaCursoTatico( finalProcessedList );

		ParDTO< List< AtendimentoTaticoDTO >, List< Integer > > entry
			= new ParDTO< List< AtendimentoTaticoDTO >, List< Integer > >(
				finalProcessedList, diaSemanaTamanhoList );

		return entry;
	}

	private ParDTO< List< AtendimentoOperacionalDTO >, List< Integer > > montaListaParaVisaoCursoOperacional(
		TurnoDTO turnoDTO, List< AtendimentoOperacionalDTO > list )
	{
		// Agrupa os DTOS pelo dia da semana
		Map< Integer, List< AtendimentoOperacionalDTO > > diaSemanaToAtendimentoOperacionalDTOMap
			= new TreeMap< Integer, List< AtendimentoOperacionalDTO > >();

		for ( AtendimentoRelatorioDTO dto : list )
		{
			List< AtendimentoOperacionalDTO > dtoList
				= diaSemanaToAtendimentoOperacionalDTOMap.get( dto.getSemana() );

			if ( dtoList == null )
			{
				dtoList = new ArrayList< AtendimentoOperacionalDTO >();
				diaSemanaToAtendimentoOperacionalDTOMap.put(
					dto.getSemana(), dtoList );
			}

			dtoList.add( (AtendimentoOperacionalDTO) dto );
		}

		// Preenche entradas nulas do mapa
		// diaSemanaToAtendimentoOperacionalDTOMap com uma lista vazia.
		for ( int i = 2; i <= 7; i++ )
		{
			if ( diaSemanaToAtendimentoOperacionalDTOMap.get( i ) == null )
			{
				diaSemanaToAtendimentoOperacionalDTOMap.put(
					i, Collections.< AtendimentoOperacionalDTO > emptyList() );
			}
		}

		List< Integer > diaSemanaTamanhoList = new ArrayList< Integer >( 8 );
		Collections.addAll( diaSemanaTamanhoList, 1, 1, 1, 1, 1, 1, 1, 1, 1 );

		List< AtendimentoOperacionalDTO > finalProcessedList
			= new ArrayList< AtendimentoOperacionalDTO >();

		// Para cada dia da semana
		int countSemanaSize = 2;

		for ( Entry< Integer, List< AtendimentoOperacionalDTO > > entry :
				diaSemanaToAtendimentoOperacionalDTOMap.entrySet() )
		{
			List< List< AtendimentoOperacionalDTO > > listListDTO
				= new ArrayList< List< AtendimentoOperacionalDTO > >();

			int size = 1;
			listListDTO = agrupaMesmoHorario( entry );

			for ( List< AtendimentoOperacionalDTO > listDTOs : listListDTO )
			{
				if ( listDTOs.size() > size )
				{
					size = listDTOs.size();
				}

				AtendimentoOperacionalDTO dtoMain = listDTOs.get( 0 );
				dtoMain.setSemana( countSemanaSize );

				for ( int i = 1; i < listDTOs.size(); i++ )
				{
					AtendimentoOperacionalDTO dtoCurrent = listDTOs.get( i );
					dtoCurrent.setSemana( dtoMain.getSemana() + i );
				}

				// Ordenando por horário de início da aula
				listDTOs = this.ordenaPorHorarioAula( listDTOs );

				finalProcessedList.addAll( listDTOs );
			}

			diaSemanaTamanhoList.add( entry.getKey(), size );
			countSemanaSize += size;
		}

		//adicionaDadosCompartilhamentoSalaCursoOperacional( finalProcessedList );

		ParDTO< List< AtendimentoOperacionalDTO >, List< Integer > > entry
			= new ParDTO< List< AtendimentoOperacionalDTO >, List< Integer > >(
				finalProcessedList, diaSemanaTamanhoList );

		return entry;
	}

	// Implementaçao da verifição relacionada com a issue
	// http://jira.gapso.com.br/browse/TRIEDA-979
	private void adicionaDadosCompartilhamentoSalaCursoOperacional(
		List< AtendimentoOperacionalDTO > atendimentos )
	{
		Map< ParDTO< Sala, ParDTO< Integer, Long > >, List< AtendimentoOperacionalDTO > > mapSalaAtendimentos
			= new HashMap< ParDTO< Sala, ParDTO< Integer, Long > >, List< AtendimentoOperacionalDTO > >();

		for ( AtendimentoOperacionalDTO atendimento : atendimentos )
		{
			Sala sala = Sala.find( atendimento.getSalaId(), getInstituicaoEnsinoUser() );
			Integer dia = atendimento.getSemana();
			Long horario = atendimento.getHorarioId();

			ParDTO< Sala, ParDTO< Integer, Long > > key
				= new ParDTO< Sala, ParDTO< Integer, Long > >(
					sala, new ParDTO< Integer, Long >( dia, horario ) );

			List< AtendimentoOperacionalDTO > list = mapSalaAtendimentos.get( key );

			if ( list == null )
			{
				list = new ArrayList< AtendimentoOperacionalDTO >();
				mapSalaAtendimentos.put( key, list );
			}

			list.add( atendimento );
		}

		// Informa que essa aula é compartilhada por mais de um curso
		for ( Entry< ParDTO< Sala, ParDTO< Integer, Long > >,
				List< AtendimentoOperacionalDTO > > entry
				: mapSalaAtendimentos.entrySet() )
		{
			List< AtendimentoOperacionalDTO > list = entry.getValue();
			montaStringCompartilhamentoSalaCursosOperacional( list );
		}
	}

	private void montaStringCompartilhamentoSalaCursosOperacional(
		List< AtendimentoOperacionalDTO > list )
	{
		if ( list != null && list.size() > 0 )
		{
			Set< Long > idsCursos = new HashSet< Long >();
			for ( AtendimentoOperacionalDTO atendimento : list )
			{
				idsCursos.add( atendimento.getCursoId() );
			}

			List< Long > listIds = new ArrayList< Long >( idsCursos );

			Curso curso = Curso.find( listIds.get( 0 ),
				getInstituicaoEnsinoUser() );

			String nomeCursos = "";

			if ( curso != null )
			{
				nomeCursos = curso.getNome();
			}

			for ( int i = 1; i < listIds.size(); i++ )
			{
				curso = Curso.find( listIds.get( i ),
					getInstituicaoEnsinoUser() );

				if ( curso != null )
				{			
					nomeCursos += ( ", " + curso.getNome() );
				}
			}

			for ( AtendimentoOperacionalDTO atendimento : list )
			{
				atendimento.setCompartilhamentoCursosString( nomeCursos );
			}
		}
	}

	// Implementaçao da verifição relacionada com a issue
	// http://jira.gapso.com.br/browse/TRIEDA-979
	private void adicionaDadosCompartilhamentoSalaCursoTatico(
		List< AtendimentoTaticoDTO > atendimentos )
	{
		Map< ParDTO< Sala, Integer >, List< AtendimentoTaticoDTO > > mapSalaAtendimentos
			= new HashMap< ParDTO< Sala, Integer >, List< AtendimentoTaticoDTO > >();

		for ( AtendimentoTaticoDTO atendimento : atendimentos )
		{
			Sala sala = Sala.find( atendimento.getSalaId(), getInstituicaoEnsinoUser() );
			Integer dia = atendimento.getSemana();

			// No modelo tático, consideramos apenas a
			// sala e o dia da aula, para agrupar os atendimentos
			// de acordo com os cursos que compartilham essa sala
			// Long horario = atendimento.getHorario();

			ParDTO< Sala, Integer > key = new ParDTO< Sala, Integer >( sala, dia );
			List< AtendimentoTaticoDTO > list = mapSalaAtendimentos.get( key );

			if ( list == null )
			{
				list = new ArrayList< AtendimentoTaticoDTO >();
				mapSalaAtendimentos.put( key, list );
			}

			list.add( atendimento );
		}

		// Informa que essa aula é compartilhada por mais de um curso
		for ( Entry< ParDTO< Sala, Integer >, List< AtendimentoTaticoDTO > > entry
			: mapSalaAtendimentos.entrySet() )
		{
			List< AtendimentoTaticoDTO > list = entry.getValue();
			montaStringCompartilhamentoSalaCursosTatico( list );
		}
	}

	private void montaStringCompartilhamentoSalaCursosTatico(
		List< AtendimentoTaticoDTO > list )
	{
		if ( list != null && list.size() > 0 )
		{
			Set< Long > idsCursos = new HashSet< Long >();
			for ( AtendimentoTaticoDTO atendimento : list )
			{
				idsCursos.add( atendimento.getCursoId() );
			}

			List< Long > listIds = new ArrayList< Long >( idsCursos );

			Curso curso = Curso.find( listIds.get( 0 ),
				getInstituicaoEnsinoUser() );

			String nomeCursos = "";
			
			if ( curso != null )
			{
				nomeCursos = curso.getNome();
			}

			for ( int i = 1; i < listIds.size(); i++ )
			{
				curso = Curso.find( listIds.get( i ),
					this.getInstituicaoEnsinoUser() );

				if ( curso != null )
				{
					nomeCursos += ( ", " + curso.getNome() );
				}
			}

			for ( AtendimentoTaticoDTO atendimento : list )
			{
				atendimento.setCompartilhamentoCursosString( nomeCursos );
			}
		}
	}

	private List< List< AtendimentoTaticoDTO > > agrupaAtendimentosAbordagem2(
		Entry< Integer, List< AtendimentoTaticoDTO > > entry )
	{
		List< List<AtendimentoTaticoDTO > > listListDTO
			= new ArrayList< List< AtendimentoTaticoDTO > >();

		List< AtendimentoTaticoDTO > sortedDTOs
			= new ArrayList< AtendimentoTaticoDTO >( entry.getValue() );

		Collections.sort( sortedDTOs, new Comparator< AtendimentoTaticoDTO >()
		{
			@Override
			public int compare( AtendimentoTaticoDTO o1, AtendimentoTaticoDTO o2 )
			{
				return o1.getTurma().compareTo( o2.getTurma() );
			}
		});

		for ( AtendimentoTaticoDTO currentDTO : sortedDTOs )
		{
			if ( listListDTO.isEmpty() )
			{
				listListDTO.add( new ArrayList< AtendimentoTaticoDTO >() );
				listListDTO.get( 0 ).add( currentDTO );
			}
			else
			{
				boolean wasDTOProcessed = false;
				for ( List< AtendimentoTaticoDTO > listDTO : listListDTO )
				{
					boolean wasDTORejected = false;
					for ( AtendimentoTaticoDTO dto : listDTO )
					{
						if ( !AtendimentoTaticoDTO.compatibleByApproach2( currentDTO, dto ) )
						{
							wasDTORejected = true;
							break;
						}
					}

					if ( !wasDTORejected )
					{
						listDTO.add( currentDTO );
						wasDTOProcessed = true;
						break;
					}
				}

				if ( !wasDTOProcessed )
				{
					listListDTO.add( new ArrayList< AtendimentoTaticoDTO >() );
					listListDTO.get( listListDTO.size() - 1 ).add( currentDTO );
				}
			}
		}

		return listListDTO;
	}

	private List< List< AtendimentoTaticoDTO > > agrupaAtendimentosAbordagem1(
		Entry< Integer, List< AtendimentoTaticoDTO > > entry )
	{
		List< List< AtendimentoTaticoDTO > > listListDTO
			= new ArrayList< List< AtendimentoTaticoDTO > >();

		for ( AtendimentoTaticoDTO currentDTO : entry.getValue() )
		{
			if ( listListDTO.isEmpty() )
			{
				listListDTO.add( new ArrayList< AtendimentoTaticoDTO >() );
				listListDTO.get( 0 ).add( currentDTO );
			}
			else
			{
				boolean wasDTOProcessed = false;

				for ( List< AtendimentoTaticoDTO > listDTO : listListDTO )
				{
					boolean wasDTORejected = false;
					for ( AtendimentoTaticoDTO dto : listDTO )
					{
						if ( !AtendimentoTaticoDTO.compatibleByApproach1( currentDTO, dto ) )
						{
							wasDTORejected = true;
							break;
						}
					}

					if ( !wasDTORejected )
					{
						listDTO.add( currentDTO );
						wasDTOProcessed = true;
						break;
					}
				}

				if ( !wasDTOProcessed )
				{
					listListDTO.add( new ArrayList< AtendimentoTaticoDTO >() );
					listListDTO.get( listListDTO.size() - 1 ).add( currentDTO );
				}
			}
		}

		return listListDTO;
	}

	private List< List< AtendimentoOperacionalDTO > > agrupaMesmoHorario(
		Entry< Integer, List< AtendimentoOperacionalDTO > > entry )
	{
		List< List< AtendimentoOperacionalDTO > > listListDTO
			= new ArrayList< List< AtendimentoOperacionalDTO > >();

		for ( AtendimentoOperacionalDTO currentDTO : entry.getValue() )
		{
			if ( listListDTO.isEmpty() )
			{
				listListDTO.add( new ArrayList< AtendimentoOperacionalDTO >() );

				listListDTO.get( 0 ).add( currentDTO );
			}
			else
			{
				boolean wasDTOProcessed = false;

				for ( List< AtendimentoOperacionalDTO > listDTO : listListDTO )
				{
					boolean wasDTORejected = false;

					for ( AtendimentoOperacionalDTO dto : listDTO )
					{
						if ( !temIntersecao(currentDTO, dto ) )
						{
							wasDTORejected = true;
							break;
						}
					}

					if ( !wasDTORejected )
					{
						listDTO.add( currentDTO );
						wasDTOProcessed = true;
					}
				}

				if ( !wasDTOProcessed )
				{
					listListDTO.add( new ArrayList< AtendimentoOperacionalDTO >() );
					listListDTO.get( listListDTO.size() - 1 ).add( currentDTO );
				}
			}
		}

		return listListDTO;
	}
	
	private boolean temIntersecao(AtendimentoOperacionalDTO dto1, AtendimentoOperacionalDTO dto2) {
		if (dto1.getSemana().equals(dto2.getSemana())) {
			if (dto1.getHorarioId().equals(dto2.getHorarioId())) {
				return true;
			} else {
				Calendar horarioInicial1 = Calendar.getInstance();
				Calendar horarioInicial2 = Calendar.getInstance();
				
				String[] horarioInicialArray1 = dto1.getHorarioString().split(":");
				String[] horarioInicialArray2 = dto2.getHorarioString().split(":");
				
				int horarioInicial1Horas = Integer.parseInt(horarioInicialArray1[0]);
				int horarioInicial1Minutos = Integer.parseInt(horarioInicialArray1[1]);
				
				int horarioInicial2Horas = Integer.parseInt(horarioInicialArray2[0]);
				int horarioInicial2Minutos = Integer.parseInt(horarioInicialArray2[1]);
				
				horarioInicial1.clear();
				horarioInicial1.set(2012,Calendar.FEBRUARY,1,horarioInicial1Horas,horarioInicial1Minutos,0);
				horarioInicial1.set(Calendar.MILLISECOND,0);
				
				horarioInicial2.clear();
				horarioInicial2.set(2012,Calendar.FEBRUARY,1,horarioInicial2Horas,horarioInicial2Minutos,0);
				horarioInicial2.set(Calendar.MILLISECOND,0);
				
				Calendar horarioFinal1 = Calendar.getInstance();
				Calendar horarioFinal2 = Calendar.getInstance();
				
				horarioFinal1.setTime(horarioInicial1.getTime());
				horarioFinal1.add(Calendar.MINUTE,dto1.getTotalCreditos()*dto1.getSemanaLetivaTempoAula());
				
				horarioFinal2.setTime(horarioInicial2.getTime());
				horarioFinal2.add(Calendar.MINUTE,dto2.getTotalCreditos()*dto2.getSemanaLetivaTempoAula());
				
				return (horarioInicial1.before(horarioInicial2) && horarioInicial2.before(horarioFinal1)) || // hi1 < hi2 < hf1
				       (horarioInicial1.before(horarioFinal2)   && horarioFinal2.before(horarioFinal1)) || // hi1 < hf2 < hf1
				       (horarioInicial2.before(horarioInicial1) && horarioInicial1.before(horarioFinal2)) || // hi2 < hi1 < hf2
				       (horarioInicial2.before(horarioFinal1)   && horarioFinal1.before(horarioFinal2)); // hi2 < hf1 < hf2
			}
		}
		return false;
	}

	@Override
	public List< AtendimentoOperacionalDTO > getAtendimentosOperacional(
		ProfessorDTO professorDTO, ProfessorVirtualDTO professorVirtualDTO,
		TurnoDTO turnoDTO, boolean isVisaoProfessor, SemanaLetivaDTO semanaLetivaDTO )
	{
		boolean isAdmin = isAdministrador();
		Turno turno = Turno.find( turnoDTO.getId(), getInstituicaoEnsinoUser() );

		Professor professor = ( professorDTO == null ? null : Professor.find( professorDTO.getId(), getInstituicaoEnsinoUser() ) );
		ProfessorVirtual professorVirtual = ( professorVirtualDTO == null ? null : ProfessorVirtual.find( professorVirtualDTO.getId(), getInstituicaoEnsinoUser() ) );
		SemanaLetiva semanaLetiva = SemanaLetiva.find( semanaLetivaDTO.getId(), getInstituicaoEnsinoUser() );
		List< AtendimentoOperacional > atendimentosOperacional = AtendimentoOperacional.getAtendimentosOperacional( getInstituicaoEnsinoUser(), isAdmin, professor, professorVirtual, turno, isVisaoProfessor, semanaLetiva );
		
		List< AtendimentoRelatorioDTO > arDTOList = new ArrayList< AtendimentoRelatorioDTO >();
		for ( AtendimentoOperacional atendimentoOperacional : atendimentosOperacional ) {
			arDTOList.add( ConvertBeans.toAtendimentoOperacionalDTO( atendimentoOperacional ) );
		}
		
		List< AtendimentoRelatorioDTO > tempDTOList = montaListaParaVisaoSala( arDTOList );
		List<AtendimentoOperacionalDTO> resultDTOList = new ArrayList<AtendimentoOperacionalDTO>();
		for (AtendimentoRelatorioDTO ar : tempDTOList) {
			resultDTOList.add((AtendimentoOperacionalDTO)ar);
		}
		
		return resultDTOList;

//		List< AtendimentoOperacional > atendimentosOperacionalDistinct
//			= new ArrayList< AtendimentoOperacional >();
//
//		for ( AtendimentoOperacional at : atendimentosOperacional )
//		{
//			boolean encontrou = false;
//
//			for ( AtendimentoOperacional at2 : atendimentosOperacionalDistinct )
//			{
//				if ( at.getHorarioDisponivelCenario().getHorarioAula()
//						== at2.getHorarioDisponivelCenario().getHorarioAula()
//					&& at.getHorarioDisponivelCenario().getDiaSemana()
//						== at2.getHorarioDisponivelCenario().getDiaSemana() )
//				{
//					encontrou = true;
//					break;
//				}
//			}
//
//			if ( !encontrou )
//			{
//				atendimentosOperacionalDistinct.add( at );
//			}
//		}
//		
//
//		List< AtendimentoOperacionalDTO > listDTO
//			= new ArrayList< AtendimentoOperacionalDTO >( atendimentosOperacionalDistinct.size() );
//
//		for ( AtendimentoOperacional atendimentoOperacional : atendimentosOperacionalDistinct )
//		{
//			listDTO.add( ConvertBeans.toAtendimentoOperacionalDTO( atendimentoOperacional ) );
//		}
//
//		return montaListaParaVisaoProfessor( montaListaParaVisaoProfessor1( listDTO ) );
	}

	private List< AtendimentoOperacionalDTO > montaListaParaVisaoProfessor1(
		List< AtendimentoOperacionalDTO > list )
	{
		// Agrupa os DTOS pela chave [ Curso - Disciplina - Turma - DiaSemana - Sala ]
		Map< String, List< AtendimentoOperacionalDTO > > atendimentoOperacionalDTOMap
			= new HashMap< String, List< AtendimentoOperacionalDTO > >();

		for ( AtendimentoOperacionalDTO dto : list )
		{
			String key = dto.getCursoString()
				+ "-" + dto.getDisciplinaString() + "-" + dto.getTurma()
				+ "-" + dto.getSemana() + "-" + dto.getSalaId();

			List< AtendimentoOperacionalDTO > dtoList
				= atendimentoOperacionalDTOMap.get( key );

			if ( dtoList == null )
			{
				dtoList = new ArrayList< AtendimentoOperacionalDTO >();
				atendimentoOperacionalDTOMap.put( key, dtoList );
			}

			dtoList.add( dto );
		}

		// Quando há mais de um DTO por chave
		// [ Curso - Disciplina - Turma - DiaSemana - Sala ],
		// concatena as informações de todos em um único DTO.
		List< AtendimentoOperacionalDTO > processedList
			= new ArrayList< AtendimentoOperacionalDTO >();

		for ( Entry< String, List< AtendimentoOperacionalDTO > > entry
			: atendimentoOperacionalDTOMap.entrySet() )
		{
			List< AtendimentoOperacionalDTO > ordenadoPorHorario = null;
			
			if ( entry.getValue().size() == 1 )
			{
				ordenadoPorHorario = new ArrayList< AtendimentoOperacionalDTO >( entry.getValue() );
			}
			else
			{
				ordenadoPorHorario = this.ordenaPorHorarioAula( entry.getValue() );
			}

			if ( ordenadoPorHorario.size() == 1 )
			{
				AtendimentoOperacionalDTO dto = ordenadoPorHorario.get( 0 );
				dto.setTotalCreditos( 1 );
				processedList.add( dto );
			}
			else
			{
				AtendimentoOperacionalDTO dtoMain = ordenadoPorHorario.get( 0 ) ;

				// Procura pelo horário correspondente ao início da aula
				HorarioAula menorHorario = AtendimentoOperacional.retornaAtendimentoMenorHorarioAula(
				    ConvertBeans.toListAtendimentoOperacional( ordenadoPorHorario ) );

				for ( int i = 1; i < ordenadoPorHorario.size(); i++ )
				{
					AtendimentoOperacionalDTO dtoCurrent = ordenadoPorHorario.get( i );
					dtoMain.concatenateVisaoProfessor( dtoCurrent );
				}

				dtoMain.setHorarioId( menorHorario.getId() );
				dtoMain.setHorarioString( TriedaUtil.shortTimeString( menorHorario.getHorario() ) );
				dtoMain.setTotalCreditos( entry.getValue().size() );

				processedList.add( dtoMain );
			}
		}

		return processedList;
	}

	public List< AtendimentoOperacionalDTO > montaListaParaVisaoProfessor(
		List< AtendimentoOperacionalDTO > list )
	{
		// Agrupa os DTOS pela chave [ Disciplina - Turma - DiaSemana - Sala ]
		Map< String, List< AtendimentoOperacionalDTO > > atendimentoOperacionalDTOMap
			= new HashMap< String, List< AtendimentoOperacionalDTO > >();

		for ( AtendimentoOperacionalDTO dto : list )
		{
			String key = dto.getDisciplinaString() + "-" + dto.getTurma()
				+ "-" + dto.getSemana() + "-" + dto.getSalaId();

			List< AtendimentoOperacionalDTO > dtoList
				= atendimentoOperacionalDTOMap.get( key );

			if ( dtoList == null )
			{
				dtoList = new ArrayList< AtendimentoOperacionalDTO >();
				atendimentoOperacionalDTOMap.put( key, dtoList );
			}

			dtoList.add( dto );
		}

		// Quando há mais de um DTO por chave
		// [ Disciplina - Turma - DiaSemana - Sala ],
		// concatena as informações de todos em um único DTO.
		List< AtendimentoOperacionalDTO > processedList
			= new ArrayList< AtendimentoOperacionalDTO >();

		for ( Entry< String, List< AtendimentoOperacionalDTO > > entry
			: atendimentoOperacionalDTOMap.entrySet() )
		{
			List< AtendimentoOperacionalDTO > ordenadoPorHorario = null;
			
			if ( entry.getValue().size() == 1 )
			{
				ordenadoPorHorario = new ArrayList< AtendimentoOperacionalDTO >( entry.getValue() );
			}
			else
			{
				ordenadoPorHorario = this.ordenaPorHorarioAula( entry.getValue() );
			}

			if ( ordenadoPorHorario.size() == 1 )
			{
				AtendimentoOperacionalDTO dto = ordenadoPorHorario.get( 0 );
				dto.setTotalCreditos( 1 );
				processedList.add( dto );
			}
			else
			{
				AtendimentoOperacionalDTO dtoMain = ordenadoPorHorario.get( 0 );

				// Procura pelo horário correspondente ao início da aula
				HorarioAula menorHorario = AtendimentoOperacional.retornaAtendimentoMenorHorarioAula(
					ConvertBeans.toListAtendimentoOperacional( ordenadoPorHorario ) );

				for ( int i = 1; i < ordenadoPorHorario.size(); i++ )
				{
					AtendimentoOperacionalDTO dtoCurrent = ordenadoPorHorario.get( i );
					dtoMain.concatenateVisaoProfessor( dtoCurrent );
				}

				dtoMain.setHorarioId( menorHorario.getId() );
				dtoMain.setHorarioString( TriedaUtil.shortTimeString( menorHorario.getHorario() ) );
				dtoMain.setTotalCreditos( ordenadoPorHorario.size() );

				processedList.add( dtoMain );
			}
		}

		return processedList;
	}

	@Override
	public ListLoadResult< ProfessorVirtualDTO > getProfessoresVirtuais( CampusDTO campusDTO )
	{
		Campus campus = Campus.find(
			campusDTO.getId(), this.getInstituicaoEnsinoUser() );

		List< ProfessorVirtual > professoresVirtuais
			= ProfessorVirtual.findBy( getInstituicaoEnsinoUser(), campus );

		List< ProfessorVirtualDTO > professoresVirtuaisDTO
			= new ArrayList< ProfessorVirtualDTO >();

		for ( ProfessorVirtual professorVirtual : professoresVirtuais )
		{
			professoresVirtuaisDTO.add(
				ConvertBeans.toProfessorVirtualDTO( professorVirtual ) );
		}

		return new BaseListLoadResult< ProfessorVirtualDTO >( professoresVirtuaisDTO );
	}

	public List< AtendimentoOperacionalDTO > ordenaPorHorarioAula(
		List< AtendimentoOperacionalDTO > listAtendimentos )
	{
		if ( listAtendimentos == null || listAtendimentos.size() == 0 )
		{
			return Collections.< AtendimentoOperacionalDTO > emptyList();
		}

		if ( listAtendimentos.size() == 1 )
		{
			return new ArrayList< AtendimentoOperacionalDTO >( listAtendimentos );
		}

		List< AtendimentoOperacionalDTO > result
			= new ArrayList< AtendimentoOperacionalDTO >( listAtendimentos );

		final Map< Long, HorarioAula > mapHorarios
			= HorarioAula.buildHorarioAulaIdToHorarioAulaMap(
				HorarioAula.findAll( getInstituicaoEnsinoUser() ) );

		Collections.sort( result,
			new Comparator< AtendimentoOperacionalDTO >()
		{
			@Override
			public int compare( AtendimentoOperacionalDTO arg1,
								AtendimentoOperacionalDTO arg2 )
			{
				HorarioAula h1 = mapHorarios.get( arg1.getHorarioId() );
				HorarioAula h2 = mapHorarios.get( arg2.getHorarioId() );

				return h1.getHorario().compareTo( h2.getHorario() );
			}
		});

		return result;
	}

	public int deslocarLinhasExportExcel(
		InstituicaoEnsino instituicaoEnsino,
		List< AtendimentoOperacionalDTO > atendimentosDia )
	{
		if ( atendimentosDia == null || atendimentosDia.size() == 0 )
		{
			return 0;
		}

		Turno turno = Turno.find(
			atendimentosDia.get( 0 ).getTurnoId() , instituicaoEnsino );

		final List< HorarioAula > listAll
			= HorarioAula.findByTurno( instituicaoEnsino, turno );

		final Map< Long, HorarioAula > mapHorarios
			= HorarioAula.buildHorarioAulaIdToHorarioAulaMap( listAll );

		// Procura pelo menor horário de
		// início de aula entre os atendimentos do dia
		HorarioAula menorHorario = mapHorarios.get(
			atendimentosDia.get( 0 ).getHorarioId() );

		for ( int i = 1; i < atendimentosDia.size(); i++ )
		{
			AtendimentoOperacionalDTO atDTO = atendimentosDia.get( i );
			HorarioAula ha = mapHorarios.get( atDTO.getHorarioId() );

			if ( ha.getHorario().compareTo( menorHorario.getHorario() ) < 0 )
			{
				menorHorario = ha;
			}
		}
		////

		// Ordenamos os atendimentos por horário de início da aula
		List< HorarioAula > horariosOrdenados
			= new ArrayList< HorarioAula >( listAll );

		Collections.sort( horariosOrdenados,
			new Comparator< HorarioAula >()
		{
			@Override
			public int compare( HorarioAula arg1,
								HorarioAula arg2 )
			{
				return arg1.getHorario().compareTo( arg2.getHorario() );
			}
		});
		////

		// Procuramos a posição do primeiro horário de aula do dia
		// em relação a todos os horários de aula cadastrados na base de dados.
		// Com isso, sabemos quantas colunas da planilha excel deverão ser
		// deixadas em branco antes de se preencher o primeiro atendimento do dia.
		for ( int index = 0; index < horariosOrdenados.size(); index++ )
		{
			HorarioAula ha = horariosOrdenados.get( index );

			if ( ha.getId() == menorHorario.getId() )
			{
				return index;
			}
		}

		return 0;
	}
}
