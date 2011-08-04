package com.gapso.web.trieda.server;

import java.util.ArrayList;
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
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.ProfessorVirtual;
import com.gapso.trieda.domain.Sala;
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
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.services.AtendimentosService;

/**
 * The server side implementation of the RPC service.
 */
public class AtendimentosServiceImpl extends RemoteService
	implements AtendimentosService
{
	private static final long serialVersionUID = -1505176338607927637L;

	@Override
	public PagingLoadResult< AtendimentoTaticoDTO > getList()
	{
		List< AtendimentoTaticoDTO > list = new ArrayList< AtendimentoTaticoDTO >();
		List< AtendimentoTatico > atendimentosTatico = AtendimentoTatico.findAll();

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
			SalaDTO salaDTO, TurnoDTO turnoDTO )
	{
		List< AtendimentoRelatorioDTO > arDTOList
			= new ArrayList< AtendimentoRelatorioDTO >();

		List< AtendimentoTaticoDTO > taticoList = getBuscaTatico( salaDTO, turnoDTO );

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
				= getBuscaOperacional( salaDTO, turnoDTO );

			for ( AtendimentoOperacionalDTO atdto : operacionalList )
			{
				arDTOList.add( atdto );
			}
		}

		return montaListaParaVisaoSala( arDTOList );
	}

	public List< AtendimentoOperacionalDTO > getBuscaOperacional(
			SalaDTO salaDTO, TurnoDTO turnoDTO )
	{
		Sala sala = Sala.find( salaDTO.getId() );
		Turno turno = Turno.find( turnoDTO.getId() );

		List< AtendimentoOperacionalDTO > list = new ArrayList< AtendimentoOperacionalDTO >();
		List< AtendimentoOperacional > atendimentosOperacional
			= AtendimentoOperacional.findBySalaAndTurno( sala, turno );

		for ( AtendimentoOperacional atendimentoOperacional : atendimentosOperacional )
		{
			list.add( ConvertBeans.toAtendimentoOperacionalDTO( atendimentoOperacional ) );
		}

		return list;
	}

	public List< AtendimentoTaticoDTO > getBuscaTatico(
			SalaDTO salaDTO, TurnoDTO turnoDTO )
	{
		Sala sala = Sala.find( salaDTO.getId() );
		Turno turno = Turno.find( turnoDTO.getId() );

		List< AtendimentoTaticoDTO > list = new ArrayList< AtendimentoTaticoDTO >();
		List< AtendimentoTatico > atendimentosTatico
			= AtendimentoTatico.findBySalaAndTurno( sala, turno );

		for ( AtendimentoTatico atendimentoTatico : atendimentosTatico )
		{
			list.add( ConvertBeans.toAtendimentoTaticoDTO( atendimentoTatico ) );
		}

		return list;
	}

	private List< AtendimentoRelatorioDTO > preMontaListaOperacional(
		List< AtendimentoOperacionalDTO > list )
	{
		List< AtendimentoRelatorioDTO > ret = new ArrayList< AtendimentoRelatorioDTO >();

		// Agrupa os DTOS pela chave [Curso-Disciplina-Turma-DiaSemana-Sala]
		Map< String, List<AtendimentoRelatorioDTO > > atendimentoTaticoDTOMap
			= new HashMap< String, List< AtendimentoRelatorioDTO > >();

		for ( AtendimentoRelatorioDTO dto : list )
		{
			String key = dto.getCursoNome() + "-" + dto.getDisciplinaString()
					+ "-" + dto.getTurma() + "-" + dto.getSemana() + "-" + dto.getSalaId();

			List< AtendimentoRelatorioDTO > dtoList = atendimentoTaticoDTOMap.get( key );

			if ( dtoList == null )
			{
				dtoList = new ArrayList< AtendimentoRelatorioDTO >();
				atendimentoTaticoDTOMap.put( key, dtoList );
			}

			dtoList.add( dto );
		}

		final Map< Long, HorarioAula > horarios
			= HorarioAula.buildHorarioAulaIdToHorarioAulaMap( HorarioAula.findAll() );

		for ( Entry< String, List< AtendimentoRelatorioDTO > > entry : atendimentoTaticoDTOMap.entrySet() )
		{
			List< AtendimentoRelatorioDTO > ordenadoPorHorario
				= new ArrayList< AtendimentoRelatorioDTO >( entry.getValue() );

			Collections.sort( ordenadoPorHorario,
					new Comparator< AtendimentoRelatorioDTO >()
					{
						@Override
						public int compare( AtendimentoRelatorioDTO a1,
								AtendimentoRelatorioDTO a2 )
						{
							AtendimentoOperacionalDTO arg1 = ( AtendimentoOperacionalDTO ) a1;
							AtendimentoOperacionalDTO arg2 = ( AtendimentoOperacionalDTO ) a2;

							HorarioAula h1 = horarios.get( arg1.getHorarioId() );
							HorarioAula h2 = horarios.get( arg2.getHorarioId() );

							return h1.getHorario().compareTo( h2.getHorario() );
						}
					});

			AtendimentoOperacionalDTO dtoMain
				= ( AtendimentoOperacionalDTO ) ordenadoPorHorario.get( 0 );

			int count = 1;
			for ( int i = 1; i < ordenadoPorHorario.size(); i++ )
			{
				AtendimentoOperacionalDTO h0 = ( AtendimentoOperacionalDTO ) ordenadoPorHorario.get( i - 1 );
				AtendimentoOperacionalDTO h1 = ( AtendimentoOperacionalDTO ) ordenadoPorHorario.get( i );

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

			list = preMontaListaOperacional( operacionalList );
		}

		// Agrupa os DTOS pela chave [Disciplina-Turma-DiaSemana]
		Map< String, List< AtendimentoRelatorioDTO > > atendimentoTaticoDTOMap
			= new HashMap< String, List< AtendimentoRelatorioDTO > >();

		for ( AtendimentoRelatorioDTO dto : list )
		{
			String key = dto.getDisciplinaString()
				+ "-" + dto.getTurma() + "-" + dto.getSemana();

			List< AtendimentoRelatorioDTO > dtoList
				= atendimentoTaticoDTOMap .get( key );

			if ( dtoList == null )
			{
				dtoList = new ArrayList< AtendimentoRelatorioDTO >();
				atendimentoTaticoDTOMap.put( key, dtoList );
			}

			dtoList.add( dto );
		}

		// Quando há mais de um DTO por chave [Disciplina-Turma-DiaSemana],
		// concatena as informações de todos em um único DTO.
		List< AtendimentoRelatorioDTO > processedList = new ArrayList< AtendimentoRelatorioDTO >();
		for ( Entry< String, List< AtendimentoRelatorioDTO > > entry : atendimentoTaticoDTOMap.entrySet() )
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

	public List< AtendimentoRelatorioDTO > montaListaParaVisaoCursoExcel(
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

			list = preMontaListaOperacional( operacionalList );
		}

		// Agrupa os DTOS pela chave [Disciplina-Turma-DiaSemana]
		Map< String, List< AtendimentoRelatorioDTO > > atendimentoTaticoDTOMap
			= new HashMap< String, List< AtendimentoRelatorioDTO > >();

		for ( AtendimentoRelatorioDTO dto : list )
		{
			String key = dto.getDisciplinaString()
				+ "-" + dto.getTurma() + "-" + dto.getSemana();

			List< AtendimentoRelatorioDTO > dtoList
				= atendimentoTaticoDTOMap .get( key );

			if ( dtoList == null )
			{
				dtoList = new ArrayList< AtendimentoRelatorioDTO >();
				atendimentoTaticoDTOMap.put( key, dtoList );
			}

			dtoList.add( dto );
		}

		// Quando há mais de um DTO por chave [Disciplina-Turma-DiaSemana],
		// concatena as informações de todos em um único DTO.
		List< AtendimentoRelatorioDTO > processedList = new ArrayList< AtendimentoRelatorioDTO >();
		for ( Entry< String, List< AtendimentoRelatorioDTO > > entry : atendimentoTaticoDTOMap.entrySet() )
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
			for ( AtendimentoOperacionalDTO atdto : parDTO.getPrimeiro() )
			{
				parDTOTempl.getPrimeiro().add( atdto );
			}
		}

		return parDTOTempl;
	}

	public List< AtendimentoTaticoDTO > getBuscaTatico( CurriculoDTO curriculoDTO,
		Integer periodo, TurnoDTO turnoDTO, CampusDTO campusDTO, CursoDTO cursoDTO )
	{
		Curriculo curriculo = Curriculo.find( curriculoDTO.getId() );
		Turno turno = Turno.find( turnoDTO.getId() );
		Campus campus = Campus.find( campusDTO.getId() );
		Curso curso = null;
		if ( cursoDTO != null )
		{
			curso = Curso.find( cursoDTO.getId() );
		}

		List< AtendimentoTaticoDTO > list = new ArrayList< AtendimentoTaticoDTO >();
		List< AtendimentoTatico > atendimentosTatico = AtendimentoTatico.findBy(
				campus, curriculo, periodo, turno, curso );

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
		Curriculo curriculo = Curriculo.find( curriculoDTO.getId() );
		Turno turno = Turno.find( turnoDTO.getId() );
		Campus campus = Campus.find( campusDTO.getId() );

		Curso curso = null;
		if ( cursoDTO != null )
		{
			curso = Curso.find( cursoDTO.getId() );
		}

		List< AtendimentoOperacional > atendimentosOperacional = AtendimentoOperacional.findBy(
				campus, curriculo, periodo, turno, curso );

		List< AtendimentoOperacionalDTO > list = new ArrayList< AtendimentoOperacionalDTO >();
		for ( AtendimentoOperacional atendimentoOperacional : atendimentosOperacional )
		{
			list.add( ConvertBeans.toAtendimentoOperacionalDTO( atendimentoOperacional ) );
		}

		return list;
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
		if ( !list.isEmpty()
			&& ( list.get( 0 ) instanceof AtendimentoOperacionalDTO ) )
		{
			List< AtendimentoRelatorioDTO > arList = preMontaListaOperacional( list );
			list.clear();

			for ( AtendimentoRelatorioDTO ar : arList )
			{
				list.add( (AtendimentoOperacionalDTO) ar );
			}
		}

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

				finalProcessedList.addAll( listDTOs );
			}

			diaSemanaTamanhoList.add( entry.getKey(), size );
			countSemanaSize += size;
		}

		adicionaDadosCompartilhamentoSalaCursoOperacional( finalProcessedList );

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
			Sala sala = Sala.find( atendimento.getSalaId() );
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

	private void montaStringCompartilhamentoSalaCursosOperacional( List< AtendimentoOperacionalDTO > list )
	{
		if ( list != null && list.size() > 0 )
		{
			Set< Long > idsCursos = new HashSet< Long >();
			for ( AtendimentoOperacionalDTO atendimento : list )
			{
				idsCursos.add( atendimento.getCursoId() );
			}
			List< Long > listIds = new ArrayList< Long >( idsCursos );

			Curso curso = Curso.find( listIds.get( 0 ) );
			String nomeCursos = curso.getNome();
			for ( int i = 1; i < listIds.size(); i++ )
			{
				curso = Curso.find( listIds.get( i ) );
				nomeCursos += ", " + curso.getNome();
			}

			for ( AtendimentoOperacionalDTO atendimento : list )
			{
				atendimento.setCompartilhamentoCursosString( nomeCursos );
			}
		}
	}

	// Implementaçao da verifição relacionada com a issue
	// http://jira.gapso.com.br/browse/TRIEDA-979
	private void adicionaDadosCompartilhamentoSalaCursoTatico( List< AtendimentoTaticoDTO > atendimentos )
	{
		Map< ParDTO< Sala, Integer >, List< AtendimentoTaticoDTO > > mapSalaAtendimentos
			= new HashMap< ParDTO< Sala, Integer >, List< AtendimentoTaticoDTO > >();

		for ( AtendimentoTaticoDTO atendimento : atendimentos )
		{
			Sala sala = Sala.find( atendimento.getSalaId() );
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

	private void montaStringCompartilhamentoSalaCursosTatico( List< AtendimentoTaticoDTO > list )
	{
		if ( list != null && list.size() > 0 )
		{
			Set< Long > idsCursos = new HashSet< Long >();
			for ( AtendimentoTaticoDTO atendimento : list )
			{
				idsCursos.add( atendimento.getCursoId() );
			}
			List< Long > listIds = new ArrayList< Long >( idsCursos );

			Curso curso = Curso.find( listIds.get( 0 ) );
			String nomeCursos = curso.getNome();
			for ( int i = 1; i < listIds.size(); i++ )
			{
				curso = Curso.find( listIds.get( i ) );
				nomeCursos += ", " + curso.getNome();
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
						if ( !AtendimentoOperacionalDTO.compatibleSameTime(
								currentDTO, dto ) )
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

	@Override
	public List< AtendimentoOperacionalDTO > getAtendimentosOperacional(
		ProfessorDTO professorDTO, ProfessorVirtualDTO professorVirtualDTO, TurnoDTO turnoDTO )
	{
		boolean isAdmin = !isAdministrador();
		Turno turno = Turno.find( turnoDTO.getId() );
		Professor professor = ( professorDTO == null ? null :
			Professor.find( professorDTO.getId() ) );
		ProfessorVirtual professorVirtual = ( professorVirtualDTO == null ? null :
			ProfessorVirtual.find( professorVirtualDTO.getId() ) );

		List< AtendimentoOperacional > atendimentosOperacional = AtendimentoOperacional
			.getAtendimentosOperacional( isAdmin, professor, professorVirtual, turno );

		List< AtendimentoOperacionalDTO > listDTO
			= new ArrayList< AtendimentoOperacionalDTO >( atendimentosOperacional.size() );

		for ( AtendimentoOperacional atendimentoOperacional : atendimentosOperacional )
		{
			listDTO.add( ConvertBeans.toAtendimentoOperacionalDTO( atendimentoOperacional ) );
		}

		return montaListaParaVisaoProfessor( montaListaParaVisaoProfessor1( listDTO ) );
	}

	private List< AtendimentoOperacionalDTO > montaListaParaVisaoProfessor1(
			List< AtendimentoOperacionalDTO > list )
	{
		// Agrupa os DTOS pela chave [Curso - Disciplina - Turma - DiaSemana - Sala]
		Map< String, List< AtendimentoOperacionalDTO > > atendimentoOperacionalDTOMap
			= new HashMap<String, List<AtendimentoOperacionalDTO > >();

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

		final Map< Long, HorarioAula > horarios
			= HorarioAula.buildHorarioAulaIdToHorarioAulaMap( HorarioAula.findAll() );

		// Quando há mais de um DTO por chave
		// [Curso - Disciplina - Turma - DiaSemana - Sala], concatena as informações
		// de todos em um único DTO.
		List< AtendimentoOperacionalDTO > processedList
			= new ArrayList< AtendimentoOperacionalDTO >();

		for ( Entry< String, List< AtendimentoOperacionalDTO > > entry : atendimentoOperacionalDTOMap.entrySet() )
		{
			List< AtendimentoOperacionalDTO > ordenadoPorHorario
				= new ArrayList< AtendimentoOperacionalDTO >( entry.getValue() );

			Collections.sort( ordenadoPorHorario,
					new Comparator< AtendimentoOperacionalDTO >()
					{
						@Override
						public int compare( AtendimentoOperacionalDTO arg1,
											AtendimentoOperacionalDTO arg2 )
						{
							HorarioAula h1 = horarios.get( arg1.getHorarioId() );
							HorarioAula h2 = horarios.get( arg2.getHorarioId() );

							return h1.getHorario().compareTo( h2.getHorario() );
						}
					});

			if ( ordenadoPorHorario.size() == 1 )
			{
				processedList.addAll( ordenadoPorHorario );
			}
			else
			{
				AtendimentoOperacionalDTO dtoMain = ordenadoPorHorario.get( 0 );

				for ( int i = 1; i < ordenadoPorHorario.size(); i++ )
				{
					dtoMain.setTotalLinhas( dtoMain.getTotalLinhas() + 1 );
				}

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

		// Quando há mais de um DTO por chave [ Disciplina - Turma - DiaSemana - Sala],
		// concatena as informações de todos em um único DTO.
		List< AtendimentoOperacionalDTO > processedList
			= new ArrayList< AtendimentoOperacionalDTO >();

		for ( Entry< String, List< AtendimentoOperacionalDTO > > entry
				: atendimentoOperacionalDTOMap.entrySet() )
		{
			if ( entry.getValue().size() == 1 )
			{
				processedList.addAll( entry.getValue() );
			}
			else
			{
				AtendimentoOperacionalDTO dtoMain = entry.getValue().get( 0 );

				for ( int i = 1; i < entry.getValue().size(); i++ )
				{
					AtendimentoOperacionalDTO dtoCurrent = entry.getValue().get( i );
					dtoMain.concatenateVisaoProfessor( dtoCurrent );
				}

				processedList.add( dtoMain );
			}
		}

		return processedList;
	}

	@Override
	public ListLoadResult< ProfessorVirtualDTO > getProfessoresVirtuais( CampusDTO campusDTO )
	{
		Campus campus = Campus.find( campusDTO.getId() );

		List< ProfessorVirtual > professoresVirtuais = ProfessorVirtual.findBy( campus );
		List< ProfessorVirtualDTO > professoresVirtuaisDTO = new ArrayList< ProfessorVirtualDTO >();

		for ( ProfessorVirtual professorVirtual : professoresVirtuais )
		{
			professoresVirtuaisDTO.add(
				ConvertBeans.toProfessorVirtualDTO( professorVirtual ) );
		}

		return new BaseListLoadResult< ProfessorVirtualDTO >( professoresVirtuaisDTO );
	}
}
