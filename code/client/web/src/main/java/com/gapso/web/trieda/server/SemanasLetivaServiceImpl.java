package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.TreeMap;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Unidade;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.services.SemanasLetivaService;

public class SemanasLetivaServiceImpl
	extends RemoteService implements SemanasLetivaService
{
	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public SemanaLetivaDTO getSemanaLetiva( CenarioDTO cenario )
	{
		// Verifica se o cenário já tem uma semana letiva associada
		if ( cenario.getSemanaLetivaId() != null )
		{
			SemanaLetiva semanaLetivaCenario = SemanaLetiva.find(
				cenario.getSemanaLetivaId(), getInstituicaoEnsinoUser() );
			
			if ( semanaLetivaCenario != null )
			{
				return ConvertBeans.toSemanaLetivaDTO( semanaLetivaCenario );
			}
		}

		List< SemanaLetiva > semanasLetivas
			= SemanaLetiva.findAll( getInstituicaoEnsinoUser() );

		if ( semanasLetivas != null && semanasLetivas.size() > 0 )
		{
			ConvertBeans.toSemanaLetivaDTO( semanasLetivas.get( 0 ) );
		}

		return null;
	}

	@Override
	public ListLoadResult< SemanaLetivaDTO > getList( BasePagingLoadConfig loadConfig )
	{
		return getBuscaList( loadConfig.get( "query" ).toString(), null, loadConfig );
	}

	@Override
	public PagingLoadResult< SemanaLetivaDTO > getBuscaList( String codigo,
		String descricao, PagingLoadConfig config )
	{
		List< SemanaLetivaDTO > list = new ArrayList< SemanaLetivaDTO >();
		String orderBy = config.getSortField();

		if ( orderBy != null )
		{
			if ( config.getSortDir() != null
				&& config.getSortDir().equals( SortDir.DESC ) )
			{
				orderBy = ( orderBy + " asc" );
			}
			else
			{
				orderBy = ( orderBy + " desc" );
			}
		}

		List< SemanaLetiva > listDomains = SemanaLetiva.findBy(
			getInstituicaoEnsinoUser(), codigo, descricao,
			config.getOffset(), config.getLimit(), orderBy );

		for ( SemanaLetiva semanaLetiva : listDomains )
		{
			list.add( ConvertBeans.toSemanaLetivaDTO( semanaLetiva ) );
		}

		BasePagingLoadResult< SemanaLetivaDTO > result
			= new BasePagingLoadResult< SemanaLetivaDTO >( list );

		result.setOffset( config.getOffset() );
		result.setTotalLength( SemanaLetiva.count(
			getInstituicaoEnsinoUser(), codigo, descricao ) );

		return result;
	}
	
	@Override
	public ListLoadResult< SemanaLetivaDTO > getList()
	{
		List< SemanaLetivaDTO > list = new ArrayList< SemanaLetivaDTO >();
		List< SemanaLetiva > listDomains = SemanaLetiva.findAll( getInstituicaoEnsinoUser() );

		for ( SemanaLetiva semanaLetiva : listDomains )
		{
			list.add( ConvertBeans.toSemanaLetivaDTO( semanaLetiva ) );
		}

		return new BaseListLoadResult< SemanaLetivaDTO >( list );
	}

	@Override
	public void save( SemanaLetivaDTO semanaLetivaDTO )
	{
		SemanaLetiva semanaLetiva
			= ConvertBeans.toSemanaLetiva( semanaLetivaDTO );

		if ( semanaLetiva.getId() != null && semanaLetiva.getId() > 0 )
		{
			semanaLetiva.merge();
		}
		else
		{
			semanaLetiva.persist();
		}

		InstituicaoEnsino instituicaoEnsino = InstituicaoEnsino.find(
			semanaLetivaDTO.getInstituicaoEnsinoId() );

		if ( semanaLetiva.getOficial() )
		{
			semanaLetiva.markOficial( instituicaoEnsino );
		}
	}

	@Override
	public void remove( List< SemanaLetivaDTO > semanaLetivaDTOList )
	{
		for ( SemanaLetivaDTO semanaLetivaDTO : semanaLetivaDTOList )
		{
			ConvertBeans.toSemanaLetiva( semanaLetivaDTO ).remove();
		}
	}

	@Override
	public PagingLoadResult< HorarioDisponivelCenarioDTO > getHorariosDisponiveisCenario(
		SemanaLetivaDTO semanaLetivaDTO )
	{
		Set< HorarioAula > horariosAula = new HashSet< HorarioAula >();
		List< SemanaLetiva > semanasLetivas
			= SemanaLetiva.findAll( getInstituicaoEnsinoUser() );

		for ( SemanaLetiva semanaLetiva : semanasLetivas )
		{
			horariosAula.addAll( semanaLetiva.getHorariosAula() );
		}

		List< HorarioDisponivelCenarioDTO > list
			= new ArrayList< HorarioDisponivelCenarioDTO >();

		for ( HorarioAula o : horariosAula )
		{
			list.add( ConvertBeans.toHorarioDisponivelCenarioDTO( o ) );
		}

		Map< String, List< HorarioDisponivelCenarioDTO > > horariosTurnos
			= new HashMap< String, List< HorarioDisponivelCenarioDTO > >();

		for ( HorarioDisponivelCenarioDTO o : list )
		{
			List< HorarioDisponivelCenarioDTO > horarios
				= horariosTurnos.get( o.getTurnoString() );

			if ( horarios == null )
			{
				horarios = new ArrayList< HorarioDisponivelCenarioDTO >();
				horariosTurnos.put( o.getTurnoString(), horarios );
			}

			horarios.add( o );
		}

		for ( Entry< String, List< HorarioDisponivelCenarioDTO > > entry : horariosTurnos.entrySet() )
		{
			Collections.sort( entry.getValue() );
		}

		Map< Date, List< String > > horariosFinalTurnos
			= new TreeMap< Date, List< String > >();

		for ( Entry< String, List< HorarioDisponivelCenarioDTO > > entry : horariosTurnos.entrySet() )
		{
			Date ultimoHorario = entry.getValue().get(
				entry.getValue().size() - 1 ).getHorario();

			List< String > turnos = horariosFinalTurnos.get( ultimoHorario );

			if ( turnos == null )
			{
				turnos = new ArrayList< String >();
				horariosFinalTurnos.put( ultimoHorario, turnos );
			}

			turnos.add( entry.getKey() );
		}

		list.clear();
		for ( Entry< Date, List< String > > entry : horariosFinalTurnos.entrySet() )
		{
			for ( String turno : entry.getValue() )
			{
				list.addAll( horariosTurnos.get( turno ) );
			}
		}

		BasePagingLoadResult< HorarioDisponivelCenarioDTO > result
			= new BasePagingLoadResult< HorarioDisponivelCenarioDTO >( list );

		result.setOffset( 0 );
		result.setTotalLength( list.size() );

		return result;
	}

	@Override
	public void saveHorariosDisponiveisCenario(
		SemanaLetivaDTO semanaLetivaDTO, List< HorarioDisponivelCenarioDTO > listDTO )
	{
		List< HorarioDisponivelCenario > listSelecionados
			= ConvertBeans.toHorarioDisponivelCenario( listDTO );

		SemanaLetiva semanaLetiva = SemanaLetiva.find(
			semanaLetivaDTO.getId(), getInstituicaoEnsinoUser() );

		List< HorarioDisponivelCenario > todosQueJaTinhamList
			= new ArrayList< HorarioDisponivelCenario >();

		for ( HorarioAula horarioAula : semanaLetiva.getHorariosAula() )
		{
			todosQueJaTinhamList.addAll( horarioAula.getHorariosDisponiveisCenario() );
		}

		List< HorarioDisponivelCenario > removerList
			= new ArrayList< HorarioDisponivelCenario >( todosQueJaTinhamList );

		removerList.removeAll( listSelecionados );

		for ( HorarioAula horariosAula : semanaLetiva.getHorariosAula() )
		{
			horariosAula.getHorariosDisponiveisCenario().removeAll( removerList );
			horariosAula.merge();
		}

		List< HorarioDisponivelCenario > adicionarList
			= new ArrayList< HorarioDisponivelCenario >( listSelecionados );

		adicionarList.removeAll( todosQueJaTinhamList );

		if ( adicionarList != null && adicionarList.size() > 0 )
		{
			List< Campus > campi = Campus.findAll( this.getInstituicaoEnsinoUser() );
			List< Unidade > unidades = Unidade.findAll( getInstituicaoEnsinoUser() );
			List< Sala > salas = Sala.findAll( getInstituicaoEnsinoUser() );
			List< Disciplina > disciplinas = Disciplina.findAll( getInstituicaoEnsinoUser() );
			List< Professor > professores = Professor.findAll( getInstituicaoEnsinoUser() );
	
			for ( HorarioDisponivelCenario o : adicionarList )
			{
				o.getCampi().addAll( campi );
				o.getUnidades().addAll( unidades );
				o.getSalas().addAll( salas );
				o.getDisciplinas().addAll( disciplinas );
				o.getProfessores().addAll( professores );
	
				o.merge();
			}
		}
	}
}
