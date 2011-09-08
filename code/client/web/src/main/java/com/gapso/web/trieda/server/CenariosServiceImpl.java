package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.util.HtmlUtils;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.web.trieda.server.util.CenarioUtil;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.TreeNodeDTO;
import com.gapso.web.trieda.shared.services.CenariosService;

@Transactional
public class CenariosServiceImpl
	extends RemoteService
	implements CenariosService
{
	private static final long serialVersionUID = -5951529933566541220L;

	public InstituicaoEnsinoDTO getInstituicaoEnsinoDTO()
	{
		InstituicaoEnsino domain
			= this.getInstituicaoEnsinoUser();

		if ( domain == null )
		{
			return null;
		}

		InstituicaoEnsinoDTO instituicaoEnsinoDTO
			= ConvertBeans.toInstituicaoEnsinoDTO( domain );

		return instituicaoEnsinoDTO;
	}

	@Override
	public CenarioDTO getMasterData()
	{
		InstituicaoEnsino instituicaoEnsino = this.getInstituicaoEnsinoUser();
		Cenario cenario = Cenario.findMasterData( this.getInstituicaoEnsinoUser() );

		if ( cenario == null )
		{
			// Criamos um novo cenário para representar
			// o master data da instituição de ensino
			cenario = new Cenario();

			cenario.setOficial( false );
			cenario.setMasterData( true );
			cenario.setNome( "MASTER DATA" );
			cenario.setAno( 1 );
			cenario.setSemestre( 1 );
			cenario.setComentario( "MASTER DATA" );

			// Semana letiva padrão do novo cenário
			SemanaLetiva semanaLetivaCenario = new SemanaLetiva();

			semanaLetivaCenario.setCodigo( HtmlUtils.htmlUnescape( "Semana Padrão" ) );
			semanaLetivaCenario.setDescricao( HtmlUtils.htmlUnescape( "Semana Letiva Padrão" ) );
			semanaLetivaCenario.setInstituicaoEnsino( instituicaoEnsino );
			semanaLetivaCenario.setOficial( true );

			Set< Campus > campi = new HashSet< Campus >(
				Campus.findAll( instituicaoEnsino ) );

			CenarioUtil util = new CenarioUtil();
			util.criarCenario( cenario, semanaLetivaCenario, campi );

			cenario = Cenario.findMasterData( this.getInstituicaoEnsinoUser() );
		}

		return ConvertBeans.toCenarioDTO( cenario );
	}

	@Override
	public CenarioDTO getCenario( Long id )
	{
		Cenario cenario = Cenario.find(
			id, this.getInstituicaoEnsinoUser() );

		return ( cenario == null ? null : ConvertBeans.toCenarioDTO( cenario ) );
	}

	@Override
	public PagingLoadResult< CenarioDTO > getList( PagingLoadConfig config )
	{
		List< CenarioDTO > list = new ArrayList< CenarioDTO >();
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

		List< Cenario > listDomains = Cenario.find( this.getInstituicaoEnsinoUser(),
			config.getOffset(), config.getLimit(), orderBy );

		for ( Cenario cenario : listDomains )
		{
			list.add( ConvertBeans.toCenarioDTO( cenario ) );
		}

		BasePagingLoadResult< CenarioDTO > result
			= new BasePagingLoadResult< CenarioDTO >( list );

		result.setOffset( config.getOffset() );
		result.setTotalLength( Cenario.count(
			this.getInstituicaoEnsinoUser() ) );

		return result;
	}

	@Override
	public PagingLoadResult< CenarioDTO > getBuscaList(
		Integer ano, Integer semestre, PagingLoadConfig config )
	{
		List< CenarioDTO > list = new ArrayList< CenarioDTO >();
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

		List< Cenario > listCenarios = Cenario.findByAnoAndSemestre( this.getInstituicaoEnsinoUser(),
			ano, semestre, config.getOffset(), config.getLimit(), orderBy );

		for ( Cenario cenario : listCenarios )
		{
			list.add( ConvertBeans.toCenarioDTO( cenario ) );
		}

		BasePagingLoadResult< CenarioDTO > result
			= new BasePagingLoadResult< CenarioDTO >( list );

		result.setOffset( config.getOffset() );
		result.setTotalLength( Cenario.count(
			this.getInstituicaoEnsinoUser() ) );

		return result;
	}

	@Override
	@Transactional
	public void editar( CenarioDTO cenarioDTO )
	{
		Cenario cenario = ConvertBeans.toCenario( cenarioDTO );
		cenario.merge();
	}

	@Override
	@Transactional
	public void criar( CenarioDTO cenarioDTO,
		SemanaLetivaDTO semanaLetivaDTO, Set< CampusDTO > campiDTO )
	{
		Cenario cenario = ConvertBeans.toCenario( cenarioDTO );
		SemanaLetiva semanaLetiva
			= ConvertBeans.toSemanaLetiva( semanaLetivaDTO );

		Set< Campus > campi = new HashSet< Campus >();

		for ( CampusDTO dto : campiDTO )
		{
			campi.add( ConvertBeans.toCampus( dto ) );
		}

		CenarioUtil cenarioUtil = new CenarioUtil();
		cenarioUtil.criarCenario( cenario, semanaLetiva, campi );
	}

	@Override
	@Transactional
	public void clonar( CenarioDTO cenarioDTO )
	{
		Cenario cenario = ConvertBeans.toCenario( cenarioDTO );

		CenarioUtil cenarioUtil = new CenarioUtil();
		cenarioUtil.clonarCenario( cenario );
	}

	@Override
	public void remove( List< CenarioDTO > cenarioDTOList )
	{
		for ( CenarioDTO cenarioDTO : cenarioDTOList )
		{
			ConvertBeans.toCenario( cenarioDTO ).remove();
		}
	}

	@Override
	public List< TreeNodeDTO > getResumos( CenarioDTO cenarioDTO )
	{
		Cenario cenario = Cenario.find(
			cenarioDTO.getId(), this.getInstituicaoEnsinoUser() );

		List< TreeNodeDTO > list = new ArrayList< TreeNodeDTO >();

		list.add( new TreeNodeDTO( "Total de Campi: <b>" + Campus.count( getInstituicaoEnsinoUser(), cenario ) + "</b>" ) );
		list.add( new TreeNodeDTO( "Total de Cursos: <b>" + Curso.count( getInstituicaoEnsinoUser(), cenario ) + "</b>" ) );
		list.add( new TreeNodeDTO( "Total de Matrizes Curriculares: <b>" + Curriculo.count( getInstituicaoEnsinoUser(), cenario ) + "</b>" ) );
		list.add( new TreeNodeDTO( "Total de Disciplinas: <b>" + Disciplina.count( getInstituicaoEnsinoUser(), cenario ) + "</b>" ) );
		list.add( new TreeNodeDTO( "Demanda Total: <b>" + Demanda.sumDemanda( getInstituicaoEnsinoUser(), cenario ) + " Alunos</b>" ) );
		list.add( new TreeNodeDTO( "Total de Salas de Aula: <b>" +
			Sala.countSalaDeAula( getInstituicaoEnsinoUser(), cenario ) + "</b>" ) );
		list.add( new TreeNodeDTO( "Total de Salas de Laborat&oacute;rio: <b>" +
			Sala.countLaboratorio( getInstituicaoEnsinoUser(), cenario ) + "</b>" ) );

		return list;
	}
}
