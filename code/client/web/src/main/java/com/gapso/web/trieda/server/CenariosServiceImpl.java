package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Date;
import java.util.HashSet;
import java.util.List;
import java.util.Locale;
import java.util.Set;

import org.springframework.format.number.NumberFormatter;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.util.HtmlUtils;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Aluno;
import com.gapso.trieda.domain.AlunoDemanda;
import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.TipoContrato;
import com.gapso.trieda.domain.TipoDisciplina;
import com.gapso.trieda.domain.TipoSala;
import com.gapso.trieda.domain.Titulacao;
import com.gapso.trieda.domain.Unidade;
import com.gapso.web.trieda.server.util.CenarioUtil;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.dtos.ResumoDTO;
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
	public CenarioDTO getCurrentCenario()
	{
		Cenario cenario = getCenario();
				
		return cenario == null ? getMasterData() : ConvertBeans.toCenarioDTO(cenario);
	}
	
	@Override
	public void setCurrentCenario(long cenarioId)
	{
		setCenario( cenarioId );
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

			semanaLetivaCenario.setCodigo( HtmlUtils.htmlUnescape( "Semana Padr&atilde;o" ) );
			semanaLetivaCenario.setDescricao( HtmlUtils.htmlUnescape( "Semana Letiva Padr&atilde;o" ) );
			semanaLetivaCenario.setInstituicaoEnsino( instituicaoEnsino );

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

		List< Cenario > listCenarios = Cenario.findByAnoAndSemestre(
			this.getInstituicaoEnsinoUser(), ano, semestre,
			config.getOffset(), config.getLimit(), orderBy );

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

		try
		{
			if ( cenario.getId() != null
				&& cenario.getId() > 0 )
			{
				cenario.setAtualizadoPor(getUsuario());
				cenario.setDataAtualizacao(new Date());
				cenario.merge();
			}
			else
			{
				cenario.setCriadoPor(getUsuario());
				cenario.setDataCriacao(new Date());
				cenario.setAtualizadoPor(getUsuario());
				cenario.setDataAtualizacao(new Date());
				criarTiposContrato(cenario);
				criarTiposDisciplina(cenario);
				criarTiposSala(cenario);
				criarTitulacoes(cenario);
				cenario.persist();
			}			
		}
		catch( Exception e )
		{
			System.out.println( e.getCause() );
		}
	}

	private void criarTiposSala(Cenario cenario) {
		TipoSala tipo1 = new TipoSala();
		tipo1.setNome( TipoSala.TIPO_SALA_DE_AULA );
		tipo1.setDescricao( TipoSala.TIPO_SALA_DE_AULA );
		tipo1.setAceitaAulaPratica( false );
		tipo1.setInstituicaoEnsino( getInstituicaoEnsinoUser() );
		tipo1.setCenario(cenario);
		tipo1.persist();

		TipoSala tipo2 = new TipoSala();
		tipo2.setNome( "Laboratório"  );
		tipo2.setDescricao( "Laboratório"  );
		tipo2.setAceitaAulaPratica( true );
		tipo2.setInstituicaoEnsino( getInstituicaoEnsinoUser() );
		tipo2.setCenario(cenario);
		tipo2.persist();
	}

	private void criarTitulacoes(Cenario cenario) {
		Titulacao titulacao1 = new Titulacao();
		titulacao1.setCenario(cenario);
		titulacao1.setInstituicaoEnsino(getInstituicaoEnsinoUser());
		titulacao1.setNome("Licenciado");
		titulacao1.persist();
		
		Titulacao titulacao2 = new Titulacao();
		titulacao2.setCenario(cenario);
		titulacao2.setInstituicaoEnsino(getInstituicaoEnsinoUser());
		titulacao2.setNome("Bacharel");
		titulacao2.persist();
		
		Titulacao titulacao3 = new Titulacao();
		titulacao3.setCenario(cenario);
		titulacao3.setInstituicaoEnsino(getInstituicaoEnsinoUser());
		titulacao3.setNome("Especialista");
		titulacao3.persist();
		
		Titulacao titulacao4 = new Titulacao();
		titulacao4.setCenario(cenario);
		titulacao4.setInstituicaoEnsino(getInstituicaoEnsinoUser());
		titulacao4.setNome("Mestre");
		titulacao4.persist();
		
		Titulacao titulacao5 = new Titulacao();
		titulacao5.setCenario(cenario);
		titulacao5.setInstituicaoEnsino(getInstituicaoEnsinoUser());
		titulacao5.setNome("Doutor");
		titulacao5.persist();
	}

	private void criarTiposDisciplina(Cenario cenario) {
		TipoDisciplina tipo1 = new TipoDisciplina();
		tipo1.setCenario(cenario);
		tipo1.setInstituicaoEnsino(getInstituicaoEnsinoUser());
		tipo1.setNome("Presencial");
		tipo1.persist();
		
		TipoDisciplina tipo3 = new TipoDisciplina();
		tipo3.setCenario(cenario);
		tipo3.setInstituicaoEnsino(getInstituicaoEnsinoUser());
		tipo3.setNome("Online");
		tipo3.persist();
	}

	private void criarTiposContrato(Cenario cenario) {
		TipoContrato tipo1 = new TipoContrato();
		tipo1.setCenario(cenario);
		tipo1.setInstituicaoEnsino(getInstituicaoEnsinoUser());
		tipo1.setNome("Horista");
		tipo1.persist();
		
		TipoContrato tipo2 = new TipoContrato();
		tipo2.setCenario(cenario);
		tipo2.setInstituicaoEnsino(getInstituicaoEnsinoUser());
		tipo2.setNome("Tempo Parcial");
		tipo2.persist();
		
		TipoContrato tipo3 = new TipoContrato();
		tipo3.setCenario(cenario);
		tipo3.setInstituicaoEnsino(getInstituicaoEnsinoUser());
		tipo3.setNome("Tempo Integral");
		tipo3.persist();	
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
		
		Locale pt_BR = new Locale("pt","BR");
		NumberFormatter numberFormatter = new NumberFormatter();
		
		int totalSalas = Sala.countSalaDeAula( getInstituicaoEnsinoUser(), cenario );
		int totalLaboratorios = Sala.countLaboratorio( getInstituicaoEnsinoUser(), cenario );
		int totalAmbientes = totalSalas + totalLaboratorios;

		List< TreeNodeDTO > list = new ArrayList< TreeNodeDTO >();

		list.add( new TreeNodeDTO( new ResumoDTO( "Total de campi ", "<b>" + numberFormatter.print( Campus.count( getInstituicaoEnsinoUser(), cenario ),pt_BR ) + "</b>"), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "Total de unidades ", "<b>" + numberFormatter.print( Unidade.findByCenario( getInstituicaoEnsinoUser(), cenario ).size(),pt_BR ) + "</b>"), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "Total de cursos ", "<b>" +  numberFormatter.print( Curso.count( getInstituicaoEnsinoUser(), cenario ),pt_BR ) + "</b>"), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "Total de matrizes curriculares ", "<b>" +  numberFormatter.print( Curriculo.count( getInstituicaoEnsinoUser(), cenario ),pt_BR ) + "</b>"), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "Total de disciplinas ", "<b>" +  numberFormatter.print( Disciplina.count( getInstituicaoEnsinoUser(), cenario ),pt_BR ) + "</b>"), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "Total de alunos ", "<b>" +  numberFormatter.print( Aluno.count( getInstituicaoEnsinoUser(), cenario, null, null ),pt_BR ) + "</b>"), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "|--- Alunos com pelo menos uma demanda ", "<b>" +
				numberFormatter.print( AlunoDemanda.countAlunosUteis( getInstituicaoEnsinoUser(), cenario ),pt_BR ) + "</b>"), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "Total de professores ", "<b>" +  numberFormatter.print( Professor.count( getInstituicaoEnsinoUser(), cenario, null, null, null, null ),pt_BR ) + "</b>"), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "|--- Professores com pelo menos uma habilitação, disponibilidade e campus de trabalho ", "<b>" +
				numberFormatter.print( Professor.findProfessoresUteis(getInstituicaoEnsinoUser(), cenario, null).size(),pt_BR ) + "</b>"), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "|--- Professores da instituição utilizados ", "<b>" +
				numberFormatter.print( AtendimentoOperacional.countProfessores( getInstituicaoEnsinoUser(), cenario ),pt_BR ) + "</b>"), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "|--- Professores virtuais utilizados ", "<b>" +
				numberFormatter.print( AtendimentoOperacional.countProfessoresVirtuais( getInstituicaoEnsinoUser(), cenario ),pt_BR ) + "</b>"), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "Demanda total ", "<b>" +  numberFormatter.print( Demanda.sumDemanda( getInstituicaoEnsinoUser(), cenario ),pt_BR ) + "</b> pares (aluno, disciplina), onde:"), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "|--- ", "<b>" +  numberFormatter.print( AlunoDemanda.sumDemandaPorPrioridade(getInstituicaoEnsinoUser(),cenario,1),pt_BR ) + "</b> P1"), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "|---  ", "<b>" +  numberFormatter.print( AlunoDemanda.sumDemandaPorPrioridade(getInstituicaoEnsinoUser(),cenario,2),pt_BR ) + "</b> P2"), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "Total de ambientes ", "<b>" +  numberFormatter.print( totalAmbientes,pt_BR )), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "|--- Total de salas de aula ", "<b>" +
				 numberFormatter.print( totalSalas,pt_BR ) + "</b>"), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "|--- Total de laboratórios ", "<b>" +
				 numberFormatter.print( totalLaboratorios,pt_BR ) + "</b>"), null, true ) );
		
		return list;
	}
	
	@Override
	public void limpaSolucoesCenario(CenarioDTO cenarioDTO)
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		Cenario.limpaSolucoesCenario(cenario);
	}
	
	public Integer checkDBVersion()
	{
		//Cenario.executeSqlUpdate(13, 14);
		return Cenario.getDBVersion();
	}
}
