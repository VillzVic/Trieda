package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.services.TurnosService;
import com.gapso.web.trieda.shared.util.view.TriedaException;

public class TurnosServiceImpl
	extends RemoteService
	implements TurnosService
{
	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public TurnoDTO getTurno( Long id )
	{
		Turno turno  = Turno.find(
			id, getInstituicaoEnsinoUser() );
		
		if ( turno == null )
		{
			return null;
		}

		return ConvertBeans.toTurnoDTO( turno );
	}

	@Override
	public PagingLoadResult< TurnoDTO > getBuscaList(
		String nome, CenarioDTO cenarioDTO, PagingLoadConfig config )
	{
		List< TurnoDTO > list = new ArrayList< TurnoDTO >();
		String orderBy = config.getSortField();
		Cenario cenario = null;
		
		if ( cenarioDTO != null )
		{
			cenario = ConvertBeans.toCenario(cenarioDTO);
		}

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

		List< Turno > listTurnos = Turno.findBy( getInstituicaoEnsinoUser(),
			nome, cenario, config.getOffset(), config.getLimit(), orderBy );

		for ( Turno turno : listTurnos )
		{
			list.add( ConvertBeans.toTurnoDTO( turno ) );
		}

		BasePagingLoadResult< TurnoDTO > result
			= new BasePagingLoadResult< TurnoDTO >( list );

		result.setOffset( config.getOffset() );
		result.setTotalLength( Turno.count(
			nome, getInstituicaoEnsinoUser(), cenario ) );

		return result;
	}
	
	@Override
	public ListLoadResult< TurnoDTO > getListOtimizedOnly()
	{
		onlyProfessor();

		List< Campus > campi = Campus.findAllOtimized(
			this.getInstituicaoEnsinoUser() );

		List< Turno > turnos = null;

		if ( campi.isEmpty() )
		{
			turnos = new ArrayList< Turno >( 0 );
		}
		else
		{
			turnos = AtendimentoOperacional.findAllTurnosByCursos(
				getInstituicaoEnsinoUser(), campi );
		}

		List< TurnoDTO > turnosDTO
			= new ArrayList< TurnoDTO >( turnos.size() );

		for ( Turno turno : turnos )
		{
			turnosDTO.add( ConvertBeans.toTurnoDTO( turno ) );
		}

		return new BaseListLoadResult< TurnoDTO >( turnosDTO );
	}

	@Override
	public ListLoadResult< TurnoDTO > getListByCampus( CenarioDTO cenarioDTO, CampusDTO campusDTO )
	{
		List< TurnoDTO > list = new ArrayList< TurnoDTO >();
		Campus campus = Campus.find(
			campusDTO.getId(), this.getInstituicaoEnsinoUser() );
		
		Cenario cenario = null;
		
		if ( cenarioDTO != null )
		{
			cenario = ConvertBeans.toCenario(cenarioDTO);
		}

		List< Turno > turnos = Turno.findBy(
			getInstituicaoEnsinoUser(), cenario, campus );

		for ( Turno turno : turnos )
		{
			list.add( ConvertBeans.toTurnoDTO( turno ) );
		}

		return new BaseListLoadResult< TurnoDTO >( list );
	}

	@Override
	public ListLoadResult< TurnoDTO > getList( CenarioDTO cenarioDTO )
	{
		Cenario cenario = null;
		
		if ( cenarioDTO != null )
		{
			cenario = ConvertBeans.toCenario(cenarioDTO);
		}

		List< Turno > list
			= Turno.findAll( getInstituicaoEnsinoUser(), cenario );

		List< TurnoDTO > listDTO
			= new ArrayList< TurnoDTO >();

		for ( Turno turno : list )
		{
			listDTO.add( ConvertBeans.toTurnoDTO( turno ) );
		}

		return new BaseListLoadResult< TurnoDTO >( listDTO );
	}

	@Override
	public ListLoadResult< TurnoDTO > getList( CenarioDTO cenarioDTO, BasePagingLoadConfig loadConfig )
	{
		CampusDTO campusDTO = loadConfig.get( "campusDTO" );
		ListLoadResult< TurnoDTO > list = null;
		
		Cenario cenario = null;
		
		if ( cenarioDTO != null )
		{
			cenario = ConvertBeans.toCenario(cenarioDTO);
		}

		if ( campusDTO == null )
		{
			list = getBuscaList( loadConfig.get(
				"query" ).toString(), cenarioDTO, loadConfig );
		}
		else
		{
			Campus campus = Campus.find(
				campusDTO.getId(), this.getInstituicaoEnsinoUser() );

			List< Turno > turnos = Turno.findBy(
				getInstituicaoEnsinoUser(), cenario, campus );

			List< TurnoDTO > turnosDTO = new ArrayList< TurnoDTO >();

			for ( Turno turno : turnos )
			{
				turnosDTO.add( ConvertBeans.toTurnoDTO( turno ) );
			}

			list = new BasePagingLoadResult< TurnoDTO >( turnosDTO );
		}

		return list;
	}

	@Override
	public void save( TurnoDTO turnoDTO ) throws TriedaException
	{
		try {
			Turno turno = ConvertBeans.toTurno( turnoDTO );
	
			if ( turno.getId() != null
				&& turno.getId() > 0 )
			{
				turno.merge();
			}
			else
			{
				turno.persist();
			}
		} catch (Exception e) {
			e.printStackTrace();
			throw new TriedaException(e);
		}
	}

	@Override
	public void remove( List< TurnoDTO > turnoDTOList ) throws TriedaException
	{
		try {
			for ( TurnoDTO turnoDTO : turnoDTOList )
			{
				ConvertBeans.toTurno( turnoDTO ).remove();
			}
		} catch (Exception e) {
			e.printStackTrace();
			throw new TriedaException(e);
		}
	}
	
	@Override
	public ListLoadResult<TurnoDTO> getTurnosNaoSelecionadosParaOtimizacao(CenarioDTO cenarioDTO, List<TurnoDTO> turnosSelecionados) {
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		List<TurnoDTO> turnosDTOsNaoSelecionados = new ArrayList<TurnoDTO>();
		
		Set<Long> turnosIDsSelecionados = new HashSet<Long>();
		for (TurnoDTO turnoDTO : turnosSelecionados) {
			turnosIDsSelecionados.add(turnoDTO.getId());
		}
		
		List<Turno> todosTurnos = Turno.findByCenario(this.getInstituicaoEnsinoUser(), cenario);
		for (Turno turno : todosTurnos) {
			if (!turnosIDsSelecionados.contains(turno.getId())) {
				turnosDTOsNaoSelecionados.add(ConvertBeans.toTurnoDTO(turno));
			}
		}

		return new BaseListLoadResult<TurnoDTO>(turnosDTOsNaoSelecionados);
	}
}
