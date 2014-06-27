package com.gapso.web.trieda.server;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashSet;
import java.util.List;
import java.util.Locale;
import java.util.Set;

import org.springframework.format.number.NumberFormatter;
import org.springframework.transaction.annotation.Transactional;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Aluno;
import com.gapso.trieda.domain.AlunoDemanda;
import com.gapso.trieda.domain.AreaTitulacao;
import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Aula;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.CenarioClone;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.DeslocamentoCampus;
import com.gapso.trieda.domain.DeslocamentoUnidade;
import com.gapso.trieda.domain.DicaEliminacaoProfessorVirtual;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.DivisaoCredito;
import com.gapso.trieda.domain.Equivalencia;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.MotivoUsoProfessorVirtual;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.ProfessorVirtual;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.TipoContrato;
import com.gapso.trieda.domain.TipoCurso;
import com.gapso.trieda.domain.TipoDisciplina;
import com.gapso.trieda.domain.TipoSala;
import com.gapso.trieda.domain.Titulacao;
import com.gapso.trieda.domain.Turma;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.domain.Unidade;
import com.gapso.web.trieda.server.util.CenarioUtil;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.server.util.progressReport.ProgressDeclaration;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportFileWriter;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportListReader;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportListWriter;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportReader;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportWriter;
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
	implements CenariosService, ProgressDeclaration
{
	private static final long serialVersionUID = -5951529933566541220L;
	private ProgressReportWriter progressReport;
	
	public void initProgressReport(String chave) {
		try {
			List<String> feedbackList = new ArrayList<String>();
			
			setProgressReport(feedbackList);
			ProgressReportReader progressSource = new ProgressReportListReader(feedbackList);
			progressSource.start();
			ProgressReportServiceImpl.getProgressReportSession(getThreadLocalRequest()).put(chave, progressSource);
			getProgressReport().start();
		}
		catch(Exception e){
			System.out.println("Nao foi possivel realizar o acompanhamento da progressao.");
		}
	}

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
			cenario.setInstituicaoEnsino(instituicaoEnsino);
			
			cenario.persist();

/*			// Semana letiva padrão do novo cenário
			SemanaLetiva semanaLetivaCenario = new SemanaLetiva();

			semanaLetivaCenario.setCodigo( HtmlUtils.htmlUnescape( "Semana Padrão" ) );
			semanaLetivaCenario.setDescricao( HtmlUtils.htmlUnescape( "Semana Letiva Padrão" ) );
			semanaLetivaCenario.setInstituicaoEnsino( instituicaoEnsino );

			Set< Campus > campi = new HashSet< Campus >(
				Campus.findAll( instituicaoEnsino ) );

			CenarioUtil util = new CenarioUtil();
			util.criarCenario( cenario, semanaLetivaCenario, campi );*/

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
	public ListLoadResult< CenarioDTO > getCenarios()
	{
		List< CenarioDTO > list = new ArrayList< CenarioDTO >();

		List< Cenario > listCenarios = Cenario.findAll(getInstituicaoEnsinoUser());
		for ( Cenario cenario : listCenarios )
		{
			list.add( ConvertBeans.toCenarioDTO( cenario ) );
		}

		BaseListLoadResult< CenarioDTO > result
			= new BaseListLoadResult< CenarioDTO >( list );

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
				Cenario cenarioBD = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
				cenarioBD.setComentario(cenarioDTO.getComentario());
				cenarioBD.setAno(cenarioDTO.getAno());
				cenarioBD.setOficial(cenarioDTO.getOficial());
				cenarioBD.setSemestre(cenarioDTO.getSemestre());
				cenarioBD.setNome(cenarioDTO.getNome());
				cenarioBD.setAtualizadoPor(getUsuario());
				cenarioBD.setDataAtualizacao(new Date());
				cenarioBD.persist();
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
			e.printStackTrace();
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
	public void clonar( CenarioDTO cenarioDTO, CenarioDTO clone, boolean clonarSolucao )
	{
		//Inicializa o relatorio de progresso
		initProgressReport("chaveClonarCenario");
		getProgressReport().setInitNewPartial("Iniciando checagem dos dados");

		Cenario cenario = ConvertBeans.toCenario(cenarioDTO);
		Cenario cenarioOriginal = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());

		cenario.setCriadoPor(getUsuario());
		cenario.setDataCriacao(new Date());
		cenario.setAtualizadoPor(getUsuario());
		cenario.setDataAtualizacao(new Date());
		cenario.setNome(clone.getNome());
		cenario.setOficial(clone.getOficial());
		cenario.setAno(clone.getAno());
		cenario.setSemestre(clone.getSemestre());
		cenario.setComentario(clone.getComentario());
		
		clonarEntidades( cenario, cenarioOriginal, clonarSolucao );
		
		getProgressReport().setPartial("Etapa concluída");
		getProgressReport().finish();
	}

	@Transactional
	private void clonarEntidades(Cenario cenario, Cenario cenarioOriginal, boolean clonarSolucao) {
		InstituicaoEnsino instituicaoEnsino = InstituicaoEnsino.find(cenario.getInstituicaoEnsino().getId());
		
		cenario.setId( null );
		cenario.persist();
		
		CenarioClone cenarioClone = new CenarioClone(cenario);
		getProgressReport().setPartial("Clonando TipoSala");
		for (TipoSala tipoSala : TipoSala.findByCenario(instituicaoEnsino, cenarioOriginal))
		{
			TipoSala ts = cenarioClone.clone(tipoSala);
			ts.persist();
		}
		getProgressReport().setPartial("Clonando TipoDisciplina");
		for (TipoDisciplina tipoDisciplina : TipoDisciplina.findByCenario(instituicaoEnsino, cenarioOriginal))
		{
			TipoDisciplina td = cenarioClone.clone(tipoDisciplina);
			td.persist();
		}
		getProgressReport().setPartial("Clonando TipoContrato");
		for (TipoContrato tipoContrato : TipoContrato.findByCenario(instituicaoEnsino, cenarioOriginal))
		{
			TipoContrato tc = cenarioClone.clone(tipoContrato);
			tc.persist();
		}
		getProgressReport().setPartial("Clonando Titulacoes");
		for (Titulacao titulacao : Titulacao.findByCenario(instituicaoEnsino, cenarioOriginal))
		{
			Titulacao t = cenarioClone.clone(titulacao);
			t.persist();
		}
		getProgressReport().setPartial("Clonando Turnos");
		for (Turno turno : cenarioOriginal.getTurnos())
		{
			Turno t = cenarioClone.clone(turno);
			t.persist();
		}
		getProgressReport().setPartial("Clonando Semanas Letivas");
		for (SemanaLetiva semanaLetiva : cenarioOriginal.getSemanasLetivas())
		{
			SemanaLetiva sl = cenarioClone.clone(semanaLetiva);
			sl.persist();
		}
		getProgressReport().setPartial("Clonando Campi");
		for (Campus campus : cenarioOriginal.getCampi())
		{
			Campus c = cenarioClone.clone(campus);
			c.persist();
		}
		getProgressReport().setPartial("Clonando Deslocamento Campi");
		for (DeslocamentoCampus deslocamentoCampus : DeslocamentoCampus.findByCenario(instituicaoEnsino, cenarioOriginal))
		{
			DeslocamentoCampus dc = cenarioClone.clone(deslocamentoCampus);
			dc.persist();
		}
		getProgressReport().setPartial("Clonando Deslocamento Unidade");
		for (DeslocamentoUnidade deslocamentoUnidade : DeslocamentoUnidade.findAll(instituicaoEnsino, cenarioOriginal))
		{
			DeslocamentoUnidade du = cenarioClone.clone(deslocamentoUnidade);
			du.persist();
		}
		getProgressReport().setPartial("Clonando Tipos Curso");
		for (TipoCurso tipoCurso : TipoCurso.findByCenario(instituicaoEnsino, cenarioOriginal))
		{
			TipoCurso tc = cenarioClone.clone(tipoCurso);
			tc.persist();
		}
		getProgressReport().setPartial("Clonando Areas Titulacoes");
		for (AreaTitulacao areaTitulacao : AreaTitulacao.findByCenario(instituicaoEnsino, cenarioOriginal))
		{
			AreaTitulacao at = cenarioClone.clone(areaTitulacao);
			at.persist();
		}
		getProgressReport().setPartial("Clonando Cursos");
		for (Curso curso : cenarioOriginal.getCursos())
		{
			Curso c = cenarioClone.clone(curso);
			c.persist();
		}
		getProgressReport().setPartial("Clonando Disciplinas");
		for (Disciplina disciplina : cenarioOriginal.getDisciplinas())
		{
			Disciplina d = cenarioClone.clone(disciplina);
			d.persist();
		}
		getProgressReport().setPartial("Clonando Divisões de Creditos");
		for (DivisaoCredito divisaoCredito : cenarioOriginal.getDivisoesCredito())
		{
			divisaoCredito.getCenario().add(cenario);
		}
		getProgressReport().setPartial("Clonando Equivalencias");
		for (Equivalencia equivalencia : Equivalencia.findByCenario(instituicaoEnsino, cenarioOriginal))
		{
			Equivalencia eq = cenarioClone.clone(equivalencia);
			eq.persist();
		}
		getProgressReport().setPartial("Clonando Curriculos");
		for (Curriculo curriculo : cenarioOriginal.getCurriculos())
		{
			Curriculo c = cenarioClone.clone(curriculo);
			c.persist();
		}
		getProgressReport().setPartial("Clonando Alunos");
		for (Aluno aluno : cenarioOriginal.getAlunos())
		{
			Aluno a = cenarioClone.clone(aluno);
			a.persist();
		}
		getProgressReport().setPartial("Clonando Ofertas");
		for (Oferta oferta : Oferta.findByCenario(instituicaoEnsino, cenarioOriginal))
		{
			Oferta o = cenarioClone.clone(oferta);
			o.persist();
		}
		getProgressReport().setPartial("Clonando Professor");
		for (Professor professor : cenarioOriginal.getProfessores())
		{
			Professor p = cenarioClone.clone(professor);
			p.persist();
		}
		
		if (clonarSolucao)
		{
			getProgressReport().setPartial("Clonando Atendimentos Tatico");
			for (AtendimentoTatico atendimentoTatico : cenarioOriginal.getAtendimentosTaticos())
			{
				AtendimentoTatico at = cenarioClone.clone(atendimentoTatico);
				at.persist();
			}
			getProgressReport().setPartial("Clonando Professores Virtuais");
			for (ProfessorVirtual professorVirtual : ProfessorVirtual.findBy(getInstituicaoEnsinoUser(), cenarioOriginal))
			{
				ProfessorVirtual pv = cenarioClone.clone(professorVirtual);
				pv.persist();
			}
			getProgressReport().setPartial("Clonando Dicas Eliminacao");
			for (DicaEliminacaoProfessorVirtual dicaEliminacao : DicaEliminacaoProfessorVirtual.findByCenario(instituicaoEnsino, cenarioOriginal))
			{
				DicaEliminacaoProfessorVirtual d = cenarioClone.clone(dicaEliminacao);
				d.persist();
			}
			getProgressReport().setPartial("Clonando Motivos de Uso");
			for (MotivoUsoProfessorVirtual motivoUso : MotivoUsoProfessorVirtual.findByCenario(instituicaoEnsino, cenarioOriginal))
			{
				MotivoUsoProfessorVirtual m = cenarioClone.clone(motivoUso);
				m.persist();
			}
			getProgressReport().setPartial("Clonando Atendimentos Operacionais");
			for (AtendimentoOperacional atendimentoOperacional : cenarioOriginal.getAtendimentosOperacionais())
			{
				AtendimentoOperacional ao = cenarioClone.clone(atendimentoOperacional);
				ao.persist();
			}
			getProgressReport().setPartial("Clonando Turmas");
			for (Turma turma : cenarioOriginal.getTurmas())
			{
				Turma t = cenarioClone.clone(turma);
				t.persist();
			}
			getProgressReport().setPartial("Clonando Aulas");
			for (Aula aula : cenarioOriginal.getAulas())
			{
				Aula a = cenarioClone.clone(aula);
				a.persist();
			}
		}
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
		
		int totalSalas = Sala.countSalaDeAulaNaoExternas( getInstituicaoEnsinoUser(), cenario );
		int totalLaboratorios = Sala.countLaboratorioNaoExternos( getInstituicaoEnsinoUser(), cenario );
		int totalAmbientes = totalSalas + totalLaboratorios;
		
		int totalSalasExternas = Sala.countSalaDeAulaExternas( getInstituicaoEnsinoUser(), cenario );
		int totalLaboratoriosExternos = Sala.countLaboratorioExternos( getInstituicaoEnsinoUser(), cenario );
		int totalAmbientesExternos = totalSalasExternas + totalLaboratoriosExternos;

		List< TreeNodeDTO > list = new ArrayList< TreeNodeDTO >();

		list.add( new TreeNodeDTO( new ResumoDTO( "Total de campi ", "<b>" + numberFormatter.print( Campus.count( getInstituicaoEnsinoUser(), cenario ),pt_BR ) + "</b>"), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "Total de unidades ", "<b>" + numberFormatter.print( Unidade.findByCenario( getInstituicaoEnsinoUser(), cenario ).size(),pt_BR ) + "</b>"), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "Total de cursos ", "<b>" +  numberFormatter.print( Curso.count( getInstituicaoEnsinoUser(), cenario ),pt_BR ) + "</b>"), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "Total de matrizes curriculares ", "<b>" +  numberFormatter.print( Curriculo.count( getInstituicaoEnsinoUser(), cenario ),pt_BR ) + "</b>"), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "Total de disciplinas ", "<b>" +  numberFormatter.print( Disciplina.count( getInstituicaoEnsinoUser(), cenario ),pt_BR ) + "</b>"), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "Total de alunos ", "<b>" +  numberFormatter.print( Aluno.count( getInstituicaoEnsinoUser(), cenario, null, null, null, null, null, null ),pt_BR ) + "</b>"), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "|--- Alunos com pelo menos uma demanda ", "<b>" +
				numberFormatter.print( AlunoDemanda.countAlunosUteis( getInstituicaoEnsinoUser(), cenario ),pt_BR ) + "</b>"), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "Total de professores ", "<b>" +  numberFormatter.print( Professor.count( getInstituicaoEnsinoUser(), cenario, null, null, null, null, null, null, null, null, null, null, null, null, null, null, null, null, null, null, null, null, null, null, null ),pt_BR ) + "</b>"), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "|--- Professores com pelo menos uma habilitação, disponibilidade e campus de trabalho ", "<b>" +
				numberFormatter.print( Professor.findProfessoresUteis(getInstituicaoEnsinoUser(), cenario, null).size(),pt_BR ) + "</b>"), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "|--- Professores da instituição utilizados ", "<b>" +
				numberFormatter.print( AtendimentoOperacional.countProfessores( getInstituicaoEnsinoUser(), cenario ),pt_BR ) + "</b>"), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "|--- Professores virtuais utilizados ", "<b>" +
				numberFormatter.print( AtendimentoOperacional.countProfessoresVirtuais( getInstituicaoEnsinoUser(), cenario ),pt_BR ) + "</b>"), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "Demanda total(pares aluno-disciplina): ", "<b>" +  numberFormatter.print( Demanda.sumDemanda( getInstituicaoEnsinoUser(), cenario ),pt_BR ) + "</b>"), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "|--- P1 ", "<b>" +  numberFormatter.print( AlunoDemanda.sumDemandaPorPrioridade(getInstituicaoEnsinoUser(),cenario,1),pt_BR ) + "</b>"), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "|--- P2 ", "<b>" +  numberFormatter.print( AlunoDemanda.sumDemandaPorPrioridade(getInstituicaoEnsinoUser(),cenario,2),pt_BR ) + "</b>"), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "Total de ambientes ", "<b>" +  numberFormatter.print( totalAmbientes,pt_BR )), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "|--- Total de salas de aula ", "<b>" +
				 numberFormatter.print( totalSalas,pt_BR ) + "</b>"), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "|--- Total de laboratórios ", "<b>" +
				 numberFormatter.print( totalLaboratorios,pt_BR ) + "</b>"), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "Total de ambientes externos ", "<b>" +  numberFormatter.print( totalAmbientesExternos,pt_BR )), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "|--- Total de salas de aula externas ", "<b>" +
				 numberFormatter.print( totalSalasExternas,pt_BR ) + "</b>"), null, true ) );
		list.add( new TreeNodeDTO( new ResumoDTO( "|--- Total de laboratórios externos ", "<b>" +
				 numberFormatter.print( totalLaboratoriosExternos,pt_BR ) + "</b>"), null, true ) );
		
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
	
	public void setProgressReport(List<String> fbl){
		progressReport = new ProgressReportListWriter(fbl);
	}
	
	public void setProgressReport(File f) throws IOException{
		progressReport = new ProgressReportFileWriter(f);
	}
	
	public ProgressReportWriter getProgressReport(){
		return progressReport;
	}
}
