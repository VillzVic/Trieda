package com.gapso.web.trieda.server.util;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.util.HtmlUtils;

import com.gapso.trieda.domain.AlunoDemanda;
import com.gapso.trieda.domain.AreaTitulacao;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.CursoDescompartilha;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.DeslocamentoCampus;
import com.gapso.trieda.domain.DeslocamentoUnidade;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.DivisaoCredito;
import com.gapso.trieda.domain.Equivalencia;
import com.gapso.trieda.domain.Fixacao;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.Incompatibilidade;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Parametro;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.ProfessorDisciplina;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.TipoContrato;
import com.gapso.trieda.domain.TipoCurso;
import com.gapso.trieda.domain.TipoDisciplina;
import com.gapso.trieda.domain.TipoSala;
import com.gapso.trieda.domain.Titulacao;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.domain.Unidade;
import com.gapso.trieda.misc.Dificuldades;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.xml.input.GrupoAlunoDemanda;
import com.gapso.web.trieda.server.xml.input.GrupoAreaTitulacao;
import com.gapso.web.trieda.server.xml.input.GrupoCalendario;
import com.gapso.web.trieda.server.xml.input.GrupoCampus;
import com.gapso.web.trieda.server.xml.input.GrupoCreditoDisponivel;
import com.gapso.web.trieda.server.xml.input.GrupoCurriculo;
import com.gapso.web.trieda.server.xml.input.GrupoCurso;
import com.gapso.web.trieda.server.xml.input.GrupoDemanda;
import com.gapso.web.trieda.server.xml.input.GrupoDeslocamento;
import com.gapso.web.trieda.server.xml.input.GrupoDiaSemana;
import com.gapso.web.trieda.server.xml.input.GrupoDisciplina;
import com.gapso.web.trieda.server.xml.input.GrupoDisciplinaPeriodo;
import com.gapso.web.trieda.server.xml.input.GrupoDivisaoCreditos;
import com.gapso.web.trieda.server.xml.input.GrupoFixacao;
import com.gapso.web.trieda.server.xml.input.GrupoGrupo;
import com.gapso.web.trieda.server.xml.input.GrupoHorario;
import com.gapso.web.trieda.server.xml.input.GrupoHorarioAula;
import com.gapso.web.trieda.server.xml.input.GrupoIdentificador;
import com.gapso.web.trieda.server.xml.input.GrupoNivelDificuldade;
import com.gapso.web.trieda.server.xml.input.GrupoOfertaCurso;
import com.gapso.web.trieda.server.xml.input.GrupoProfessor;
import com.gapso.web.trieda.server.xml.input.GrupoProfessorDisciplina;
import com.gapso.web.trieda.server.xml.input.GrupoSala;
import com.gapso.web.trieda.server.xml.input.GrupoTipoContrato;
import com.gapso.web.trieda.server.xml.input.GrupoTipoCurso;
import com.gapso.web.trieda.server.xml.input.GrupoTipoDisciplina;
import com.gapso.web.trieda.server.xml.input.GrupoTipoSala;
import com.gapso.web.trieda.server.xml.input.GrupoTipoTitulacao;
import com.gapso.web.trieda.server.xml.input.GrupoTurno;
import com.gapso.web.trieda.server.xml.input.GrupoUnidade;
import com.gapso.web.trieda.server.xml.input.ItemAlunoDemanda;
import com.gapso.web.trieda.server.xml.input.ItemAreaTitulacao;
import com.gapso.web.trieda.server.xml.input.ItemAtendimentoCampusSolucao;
import com.gapso.web.trieda.server.xml.input.ItemAtendimentoDiaSemanaSolucao;
import com.gapso.web.trieda.server.xml.input.ItemAtendimentoOfertaSolucao;
import com.gapso.web.trieda.server.xml.input.ItemAtendimentoSalaSolucao;
import com.gapso.web.trieda.server.xml.input.ItemAtendimentoTaticoSolucao;
import com.gapso.web.trieda.server.xml.input.ItemAtendimentoUnidadeSolucao;
import com.gapso.web.trieda.server.xml.input.ItemCalendario;
import com.gapso.web.trieda.server.xml.input.ItemCampus;
import com.gapso.web.trieda.server.xml.input.ItemCreditoDisponivel;
import com.gapso.web.trieda.server.xml.input.ItemCurriculo;
import com.gapso.web.trieda.server.xml.input.ItemCurso;
import com.gapso.web.trieda.server.xml.input.ItemDemanda;
import com.gapso.web.trieda.server.xml.input.ItemDeslocamento;
import com.gapso.web.trieda.server.xml.input.ItemDisciplina;
import com.gapso.web.trieda.server.xml.input.ItemDisciplinaPeriodo;
import com.gapso.web.trieda.server.xml.input.ItemDivisaoCreditos;
import com.gapso.web.trieda.server.xml.input.ItemFixacao;
import com.gapso.web.trieda.server.xml.input.ItemHorario;
import com.gapso.web.trieda.server.xml.input.ItemHorarioAula;
import com.gapso.web.trieda.server.xml.input.ItemNivelDificuldade;
import com.gapso.web.trieda.server.xml.input.ItemOfertaCurso;
import com.gapso.web.trieda.server.xml.input.ItemParametrosPlanejamento;
import com.gapso.web.trieda.server.xml.input.ItemParametrosPlanejamento.CargaHorariaSemanalAluno;
import com.gapso.web.trieda.server.xml.input.ItemParametrosPlanejamento.CargaHorariaSemanalProfessor;
import com.gapso.web.trieda.server.xml.input.ItemPercentualMinimo;
import com.gapso.web.trieda.server.xml.input.ItemProfessor;
import com.gapso.web.trieda.server.xml.input.ItemProfessorDisciplina;
import com.gapso.web.trieda.server.xml.input.ItemSala;
import com.gapso.web.trieda.server.xml.input.ItemTipoContrato;
import com.gapso.web.trieda.server.xml.input.ItemTipoCurso;
import com.gapso.web.trieda.server.xml.input.ItemTipoDisciplina;
import com.gapso.web.trieda.server.xml.input.ItemTipoSala;
import com.gapso.web.trieda.server.xml.input.ItemTipoTitulacao;
import com.gapso.web.trieda.server.xml.input.ItemTurno;
import com.gapso.web.trieda.server.xml.input.ItemUnidade;
import com.gapso.web.trieda.server.xml.input.ObjectFactory;
import com.gapso.web.trieda.server.xml.input.TriedaInput;
import com.gapso.web.trieda.shared.util.view.CargaHorariaComboBox.CargaHoraria;

@Transactional
public class SolverInput
{
	private Cenario cenario;
	private ObjectFactory of;
	private TriedaInput triedaInput;
	private List< Campus > campi;
	private Parametro parametro;
	private InstituicaoEnsino instituicaoEnsino;
	private List< String > errors;
	private List< String > warnings;

	private List< HorarioDisponivelCenario > todosHorarioDisponivelCenario;
	private Map< Campus, Set< HorarioDisponivelCenario > > horariosCampus;
	private Map< Unidade, Set< HorarioDisponivelCenario > > horariosUnidades;
	private Map< Sala, Set< HorarioDisponivelCenario > > horariosSalas;
	private Map< Disciplina, Set< HorarioDisponivelCenario > > horariosDisciplinas;
	private Map< Professor, Set< HorarioDisponivelCenario > > horariosProfessores;
	private Map< Fixacao, Set< HorarioDisponivelCenario > > horariosFixacoes;

	private Set< Demanda > demandasCampusTurno = new HashSet< Demanda >();
	private Set< Disciplina > disciplinasComDemandaCurriculo = new HashSet< Disciplina >();
	private Map< Curriculo, List< Integer > > mapCurriculosPeriodos = new HashMap< Curriculo, List< Integer > >();

	public SolverInput(
		InstituicaoEnsino instituicaoEnsino, Cenario cenario,
		Parametro parametro, List< Campus > listCampi )
	{
		this.instituicaoEnsino = instituicaoEnsino;
		this.cenario = cenario;
		this.parametro = parametro;

		// Está sendo enviado apenas um campus nessa lista
		this.campi = new ArrayList< Campus >();

		for ( Campus c : listCampi )
		{
			if ( c.getInstituicaoEnsino() == instituicaoEnsino )
			{
				this.campi.add( c );
			}
		}

		this.of = new ObjectFactory();
		this.triedaInput = of.createTriedaInput();
		this.errors = new ArrayList< String >();
		this.warnings = new ArrayList< String >();

		this.preencheMapHorarios();
		this.carregaDemandasDisciplinas();
		this.relacionaCurriculosPeriodos();
	}

	private void relacionaCurriculosPeriodos()
	{
		Set< Curso > cursos = cenario.getCursos();
		
		for ( Curso curso : cursos )
		{
			for ( Curriculo curriculo : curso.getCurriculos() )
			{
				List< Integer > periodos
					= curriculo.getPeriodos( this.instituicaoEnsino );

				if ( !this.mapCurriculosPeriodos.containsKey( curriculo ) )
				{
					this.mapCurriculosPeriodos.put( curriculo, periodos );
				}
			}
		}
	}
	
	private void carregaDemandasDisciplinas()
	{
		// Inicialmente, consideramos que todas as
		// disciplinas serão enviadas para o solver
		this.disciplinasComDemandaCurriculo.addAll(
			Disciplina.findAll( this.instituicaoEnsino ) );

		// Removemos do input todas as disciplinas
		// que não possuem nenhuma demanda cadastrada
		this.demandasCampusTurno.addAll( Demanda.findByCampusTurno(
				this.instituicaoEnsino, this.parametro.getCampus(), this.parametro.getTurno() ) );

		Set< Disciplina > disciplinasComDemanda = new HashSet< Disciplina >();

		for ( Demanda demanda : this.demandasCampusTurno )
		{
			disciplinasComDemanda.add( demanda.getDisciplina() );
		}

		this.disciplinasComDemandaCurriculo.retainAll( disciplinasComDemanda );

		// Removemos do input todas as disciplinas
		// que não estão associadas a nenhuma matriz curriular
		Set< Disciplina > disciplinasComCurriculo = new HashSet< Disciplina >();

		Set< Curso > cursos = this.cenario.getCursos();
		
		for ( Curso curso : cursos )
		{
			for ( Curriculo curriculo : curso.getCurriculos() )
			{
				for ( CurriculoDisciplina cd : curriculo.getDisciplinas() )
				{
					disciplinasComCurriculo.add( cd.getDisciplina() );
				}
			}
		}

		this.disciplinasComDemandaCurriculo.retainAll( disciplinasComCurriculo );
	}

	private void preencheMapHorarios()
	{
		this.horariosCampus = new HashMap< Campus, Set< HorarioDisponivelCenario > >();
		this.horariosUnidades = new HashMap< Unidade, Set< HorarioDisponivelCenario > >();
		this.horariosSalas = new HashMap< Sala, Set< HorarioDisponivelCenario > >();
		this.horariosDisciplinas = new HashMap< Disciplina, Set< HorarioDisponivelCenario > >();
		this.horariosProfessores = new HashMap< Professor, Set< HorarioDisponivelCenario > >();
		this.horariosFixacoes = new HashMap< Fixacao, Set< HorarioDisponivelCenario > >();

		this.todosHorarioDisponivelCenario
			= HorarioDisponivelCenario.findAll( this.instituicaoEnsino );

		for (HorarioDisponivelCenario hdc : this.todosHorarioDisponivelCenario) {
			// TRIEDA-1154: Os "horarios disponiveis" de uma disciplina ja associada a alguma matriz curricular devem pertencer somente 'a semana letiva da matriz curricular correspondente.
			SemanaLetiva semanaLetivaDeHDC = hdc.getHorarioAula().getSemanaLetiva();
			
			for ( Campus campus : hdc.getCampi() )
			{
				Set< HorarioDisponivelCenario > horarios
					= this.horariosCampus.get( campus );

				if ( horarios == null )
				{
					horarios = new HashSet< HorarioDisponivelCenario >();
					this.horariosCampus.put( campus, horarios );
				}

				horarios.add( hdc );
			}

			for ( Unidade unidade : hdc.getUnidades() )
			{
				Set< HorarioDisponivelCenario > horarios
					= this.horariosUnidades.get( unidade );

				if ( horarios == null )
				{
					horarios = new HashSet< HorarioDisponivelCenario >();
					this.horariosUnidades.put( unidade, horarios ); 
				}

				horarios.add( hdc );
			}

			for ( Sala sala : hdc.getSalas() )
			{
				Set< HorarioDisponivelCenario > horarios
					= this.horariosSalas.get( sala );

				if ( horarios == null )
				{
					horarios = new HashSet< HorarioDisponivelCenario >();
					this.horariosSalas.put( sala, horarios ); 
				}

				horarios.add( hdc );
			}

			for ( Disciplina disciplina : hdc.getDisciplinas() )
			{
				Set< HorarioDisponivelCenario > horarios
					= this.horariosDisciplinas.get( disciplina );

				if ( horarios == null )
				{
					horarios = new HashSet< HorarioDisponivelCenario >();
					this.horariosDisciplinas.put( disciplina, horarios ); 
				}
				 
				// TRIEDA-1154: Os "horarios disponiveis" de uma disciplina ja associada a alguma matriz curricular devem pertencer somente 'a semana letiva da matriz curricular correspondente.
				if (disciplina.getSemanasLetivas().isEmpty() || disciplina.getSemanasLetivas().contains(semanaLetivaDeHDC)) {
					horarios.add( hdc );
				}
			}

			for ( Professor professor : hdc.getProfessores() )
			{
				Set< HorarioDisponivelCenario > horarios
					= this.horariosProfessores.get( professor );

				if ( horarios == null )
				{
					horarios = new HashSet< HorarioDisponivelCenario >();
					this.horariosProfessores.put( professor, horarios ); 
				}

				horarios.add( hdc );
			}

			for ( Fixacao fixacao : hdc.getFixacoes() )
			{
				Set< HorarioDisponivelCenario > horarios
					= this.horariosFixacoes.get( fixacao );

				if ( horarios == null )
				{
					horarios = new HashSet< HorarioDisponivelCenario >();
					this.horariosFixacoes.put( fixacao, horarios ); 
				}

				horarios.add( hdc );
			}
		}
	}

	public List< String > getErrors()
	{
		return this.errors;
	}

	public void setErrors(
		List< String > errors )
	{
		this.errors = errors;
	}

	public List< String > getWarnings()
	{
		return this.warnings;
	}

	public void setWarnings(
		List< String > warnings )
	{
		this.warnings = warnings;
	}

	@Transactional
	public TriedaInput generateTaticoTriedaInput()
	{
		generate( true );
		return this.triedaInput;
	}

	@Transactional
	public TriedaInput generateOperacionalTriedaInput()
	{
		generate( false );
		return this.triedaInput;
	}

	private void checkErrorsWarnings()
	{
		checkMaxCreditosSemanaisPorPeriodo_e_DisciplinasRepetidasPorCurriculo();
		checkDemandasComDisciplinasSemCurriculo();
		
		// PRIMEIRA VERIFICAÇÃO

		// Verificar se existem disciplinas que não
		// foram associadas a nenhuma matriz curricular
		Set< Disciplina > disciplinas = new HashSet< Disciplina >();

		for ( Curso curso : this.cenario.getCursos() )
		{
			for ( Curriculo curriculo : curso.getCurriculos() )
			{
				for ( CurriculoDisciplina curriculoDisciplina : curriculo.getDisciplinas() )
				{
					disciplinas.add( curriculoDisciplina.getDisciplina() );
				}
			}
		}

		for ( Disciplina disciplinaCenario : this.cenario.getDisciplinas() )
		{
			boolean encontrou = false;

			for ( Disciplina disciplinaCurriculos : disciplinas )
			{
				if ( disciplinaCurriculos.getId() == disciplinaCenario.getId() )
				{
					encontrou = true;
					break;
				}
			}

			if ( !encontrou )
			{
				String warningMessage = "A disciplina " + disciplinaCenario.getCodigo()
					+ " n&atilde;o foi associada a nenhuma matriz curricular.";

				createWarningMessage( warningMessage );
			}
		}

		// SEGUNDA VERIFICAÇÃO

		// Fixação de disciplina com qtde de horários
		// menores do que a qtde de créditos da disciplinas
		List< Fixacao > fixacoes
			= Fixacao.findAll( this.instituicaoEnsino );

		for ( Disciplina disciplinaCenario : this.cenario.getDisciplinas() )
		{
			for ( Fixacao fixacao : fixacoes )
			{
				Integer totalCreditos = this.getHorarios( fixacao  ).size();

				Disciplina disciplinaFixacao = fixacao.getDisciplina();

				if ( totalCreditos != null && totalCreditos > 0
					&& disciplinaFixacao != null
					&& disciplinaFixacao.getId() == disciplinaCenario.getId()
					&& totalCreditos < disciplinaCenario.getTotalCreditos() )
				{
					String warningMessage = "A disciplina " + disciplinaCenario.getCodigo()
						+ ", que possui " + disciplinaCenario.getTotalCreditos() + " cr&eacute;ditos,"
						+ " tem uma fixa&ccedil;&atilde;o de apenas " + totalCreditos.toString() + ".";

					createWarningMessage( warningMessage );

					break;
				}
			}
		}

		// TERCEIRA VERIFICAÇÃO

		// Impossibilidade de associar uma regra
		// genérica de divisão de créditos a uma disciplina
		Set< DivisaoCredito > regras
			= this.cenario.getDivisoesCredito();

		if ( regras.size() > 0 )
		{
			for ( Disciplina disciplinaCenario : this.cenario.getDisciplinas() )
			{
				int creditosDisciplina = disciplinaCenario.getTotalCreditos();

				boolean encontrou = false;

				for ( DivisaoCredito divisaoCredito : regras )
				{
					if ( divisaoCredito.getCreditos() >= creditosDisciplina )
					{
						encontrou = true;
						break;
					}
				}

				if ( !encontrou )
				{
					String warningMessage = "A disciplina " + disciplinaCenario.getCodigo()
						+ " n&atilde;o possui nenhuma regra de cr&eacute;ditos gen&eacute;rica "
						+ "que atenda ao total de cr&eacute;ditos da disciplina.";

					createWarningMessage( warningMessage );
				}
			}
		}
	}

	private void checkDemandasComDisciplinasSemCurriculo() {
		// [CurriculoId -> Set<DisciplinaId>]
		Map<Long,Set<Long>> curriculoIdToDisciplinasIdsMap = new HashMap<Long,Set<Long>>();
		for (Oferta oferta : this.parametro.getCampus().getOfertas()) {
			Curriculo curriculo = oferta.getCurriculo();
			
			// obtém as disciplinas associadas com o currículo em questão
			Set<Long> disciplinasDoCurriculo = curriculoIdToDisciplinasIdsMap.get(curriculo.getId());
			if (disciplinasDoCurriculo == null) {
				disciplinasDoCurriculo = new HashSet<Long>();
				for (CurriculoDisciplina curriculoDisciplina : curriculo.getCurriculoDisciplinas()) {
					disciplinasDoCurriculo.add(curriculoDisciplina.getDisciplina().getId());
				}
			}
			
			// verifica se alguma demanda contém alguma disciplina que não esteja no currículo
			// da oferta associada com a demanda
			for (Demanda demanda : oferta.getDemandas()) {
				if (!disciplinasDoCurriculo.contains(demanda.getDisciplina().getId())) {
					createErrorMessage("A demanda [" + demanda.getNaturalKeyString() + "] é inválida pois a disciplina [" + demanda.getDisciplina().getCodigo() + "] não pertence a nenhum período da matriz curricular [" + curriculo.getCodigo() + "].");
					System.out.println("A demanda [" + demanda.getNaturalKeyString() + "] é inválida pois a disciplina [" + demanda.getDisciplina().getCodigo() + "] não pertence a nenhum período da matriz curricular [" + curriculo.getCodigo() + "].");
				}
			}
		}
	}

	private void checkMaxCreditosSemanaisPorPeriodo_e_DisciplinasRepetidasPorCurriculo() {
		// obtém os currículos do campus selecionado para otimização
		Set<Curriculo> curriculosDoCampusSelecionado = new HashSet<Curriculo>();
		for (Oferta oferta : this.parametro.getCampus().getOfertas()) {
			curriculosDoCampusSelecionado.add(oferta.getCurriculo());
		}
		
		// [CurriculoId -> Máximo Créditos Semanais]
		Map<Long,Integer> maxCreditosSemanaisPorSemanaLetivaMap = new HashMap<Long,Integer>();
 
		for (Curriculo curriculo : this.cenario.getCurriculos()) {
			// filtra os currículos do campus selecionado para otimização
			if (curriculosDoCampusSelecionado.contains(curriculo)) {
				// obtém o máximo de créditos semanais da semana letiva associada com o currículo
				Integer maxCreditosSemanais = maxCreditosSemanaisPorSemanaLetivaMap.get(curriculo.getSemanaLetiva().getId());
				if (maxCreditosSemanais == null) {
					maxCreditosSemanais = curriculo.getSemanaLetiva().calcTotalCreditosSemanais(this.parametro.getTurno());
					maxCreditosSemanaisPorSemanaLetivaMap.put(curriculo.getSemanaLetiva().getId(),maxCreditosSemanais);
				}
				
				Set<Long> disciplinasDoCurriculo = new HashSet<Long>();
				Set<String> disciplinasRepetidasNoCurriculo = new HashSet<String>();
				List<String> periodosQueViolamMaxCreditosSemanais = new ArrayList<String>();
				for (Integer periodo : curriculo.getPeriodos(instituicaoEnsino)) {
					Integer totalCreditosDoPeriodo = 0;
					for (CurriculoDisciplina curriculoDisciplina : curriculo.getCurriculoDisciplinasByPeriodo(instituicaoEnsino,periodo)) {
						if (!disciplinasDoCurriculo.add(curriculoDisciplina.getDisciplina().getId())) {
							disciplinasRepetidasNoCurriculo.add(curriculoDisciplina.getDisciplina().getCodigo());
						}
						totalCreditosDoPeriodo += curriculoDisciplina.getDisciplina().getCreditosTotal();
					}
					
					if (totalCreditosDoPeriodo > maxCreditosSemanais) {
						periodosQueViolamMaxCreditosSemanais.add(periodo + "("+totalCreditosDoPeriodo+")");
					}
					
				}
				
				if (!disciplinasRepetidasNoCurriculo.isEmpty()) {
					createErrorMessage("A matriz curricular [" + curriculo.getCodigo() + "] contém disciplinas repetidas, são elas: " + disciplinasRepetidasNoCurriculo.toString());
					System.out.println("A matriz curricular [" + curriculo.getCodigo() + "] contém disciplinas repetidas, são elas: " + disciplinasRepetidasNoCurriculo.toString());
				}
				
				if (!periodosQueViolamMaxCreditosSemanais.isEmpty()) {
					createErrorMessage("Na matriz curricular [" + curriculo.getCodigo() + "] existem períodos que violam a quantidade máxima de créditos semanais da Semana Letiva. Máximo de Créditos Semanais = " + maxCreditosSemanais + ". Período(TotalCréditos) = " + periodosQueViolamMaxCreditosSemanais.toString());
					System.out.println("Na matriz curricular [" + curriculo.getCodigo() + "] existem períodos que violam a quantidade máxima de créditos semanais da Semana Letiva. Máximo de Créditos Semanais = " + maxCreditosSemanais + ". Período(TotalCréditos) = " + periodosQueViolamMaxCreditosSemanais.toString());
				}
			}
		}
	}

	private Set< HorarioDisponivelCenario > getHorarios( Campus campus )
	{
		if ( campus == null )
		{
			return Collections.emptySet();
		}		

		Set< HorarioDisponivelCenario > result
			= this.horariosCampus.get( campus );

		if ( result == null )
		{
			result = new HashSet< HorarioDisponivelCenario >();
			this.horariosCampus.put( campus, result );
		}

		return result;
	}
	
	private Set< HorarioDisponivelCenario > getHorarios( Unidade unidade )
	{
		if ( unidade == null )
		{
			return Collections.emptySet();
		}		

		Set< HorarioDisponivelCenario > result
			= this.horariosUnidades.get( unidade );

		if ( result == null )
		{
			result = new HashSet< HorarioDisponivelCenario >();
			this.horariosUnidades.put( unidade, result );
		}

		return result;
	}
	
	private Set< HorarioDisponivelCenario > getHorarios( Sala sala )
	{
		if ( sala == null )
		{
			return Collections.emptySet();
		}

		Set< HorarioDisponivelCenario > result
			= this.horariosSalas.get( sala );

		if ( result == null )
		{
			result = new HashSet< HorarioDisponivelCenario >();
			this.horariosSalas.put( sala, result );
		}

		return result;
	}
	
	private Set< HorarioDisponivelCenario > getHorarios( Disciplina disciplina )
	{
		if ( disciplina == null )
		{
			return Collections.emptySet();
		}

		Set< HorarioDisponivelCenario > result
			= this.horariosDisciplinas.get( disciplina );

		if ( result == null )
		{
			result = new HashSet< HorarioDisponivelCenario >();
			this.horariosDisciplinas.put( disciplina, result );
		}

		return result;
	}

	private Set< HorarioDisponivelCenario > getHorarios( Professor professor )
	{
		if ( professor == null )
		{
			return Collections.emptySet();
		}

		Set< HorarioDisponivelCenario > result
			= this.horariosProfessores.get( professor );

		if ( result == null )
		{
			result = new HashSet< HorarioDisponivelCenario >();
			this.horariosProfessores.put( professor, result );
		}

		return result;
	}

	private Set< HorarioDisponivelCenario > getHorarios( Fixacao fixacao )
	{
		if ( fixacao == null )
		{
			return Collections.emptySet();
		}

		Set< HorarioDisponivelCenario > result
			= this.horariosFixacoes.get( fixacao );

		if ( result == null )
		{
			result = new HashSet< HorarioDisponivelCenario >();
			this.horariosFixacoes.put( fixacao, result );
		}

		return result;
	}

	@Transactional
	private void generate( boolean tatico )
	{
		generateCalendario();
		generateTiposSala();
		generateTiposContrato();
		generateTiposTitulacao();
		generateAreasTitulacao();
		generateTiposDisciplina();
		generateNiveisDificuldade();
		generateTiposCurso();
		generateDivisoesDeCredito();
		generateDeslocamentoCampi();
		generateDeslocamentoUnidades();
		generateDisciplinas();
		generateCurso();
		generateCampi( tatico );
		generateOfertaCursoCampi();
		generateDemandas();
		generateAlunosDemanda();
		generateParametrosPlanejamento( tatico );
		generateFixacoes();

		if ( !tatico )
		{
			generateTaticoInput();
		}

		checkErrorsWarnings();
	}

	@Transactional
	private void generateCalendario()
	{
		GrupoCalendario grupoCalendario = this.of.createGrupoCalendario();

		List< SemanaLetiva > semanasLetivas = SemanaLetiva.findAll( this.instituicaoEnsino );
		
		Collections.sort(semanasLetivas); // para que as informações sejam impressas em ordem alfabética e, desse modo, facilitar a comparação entre dois XMLs de input 
		for ( SemanaLetiva calendario : semanasLetivas )
		{
			ItemCalendario itemCalendario
				= this.of.createItemCalendario();

			itemCalendario.setId( calendario.getId().intValue() );
			itemCalendario.setCodigo( calendario.getCodigo() );
			itemCalendario.setTempoAula( calendario.getTempo() );

			GrupoTurno grupoTurno = this.of.createGrupoTurno();

			List< Turno > turnos = Turno.findByCalendario(
				this.instituicaoEnsino, calendario );

			Collections.sort(turnos); // para que as informações sejam impressas em ordem alfabética e, desse modo, facilitar a comparação entre dois XMLs de input
			// Lendo turnos
			for ( Turno turno : turnos )
			{
				if ( turno != this.parametro.getTurno() )
				{
					continue;
				}

				ItemTurno itemTurno = this.of.createItemTurno();

				itemTurno.setId( turno.getId().intValue() );
				itemTurno.setNome( turno.getNome() );

				// Lendo horários de aula
				GrupoHorarioAula grupoHorarioAula = this.of.createGrupoHorarioAula();

				//Set< HorarioAula > horariosAula = new HashSet< HorarioAula >( HorarioAula.findBySemanaLetiva( this.instituicaoEnsino, calendario ) );
				// TRIEDA-1165
				List< HorarioAula > horariosAula = HorarioAula.findHorarioAulasBySemanaLetivaAndTurno(this.instituicaoEnsino, calendario, turno );
				
				Collections.sort(horariosAula); // para que as informações sejam impressas em ordem alfabética e, desse modo, facilitar a comparação entre dois XMLs de input
				for ( HorarioAula horarioAula : horariosAula )
				{
					ItemHorarioAula itemHorarioAula
						= this.of.createItemHorarioAula();

					itemHorarioAula.setId( horarioAula.getId().intValue() );
					itemHorarioAula.setInicio( new XMLGregorianCalendarUtil(
						horarioAula.getHorario() ) );

					GrupoDiaSemana grupoDiasSemana
						= this.of.createGrupoDiaSemana();

					List< HorarioDisponivelCenario > horariosDisponivelCenario = new ArrayList<HorarioDisponivelCenario>(horarioAula.getHorariosDisponiveisCenario());
					// para que as informações sejam impressas em ordem alfabética e, desse modo, facilitar a comparação entre dois XMLs de input
					Collections.sort(horariosDisponivelCenario,new Comparator<HorarioDisponivelCenario>() {
						@Override
						public int compare(HorarioDisponivelCenario o1, HorarioDisponivelCenario o2) {
							return o1.getDiaSemana().compareTo(o2.getDiaSemana());
						}
					}); 
					for ( HorarioDisponivelCenario hdc : horariosDisponivelCenario )
					{
						grupoDiasSemana.getDiaSemana().add(
							Semanas.toInt( hdc.getDiaSemana() ) );
					}

					itemHorarioAula.setDiasSemana( grupoDiasSemana );
					grupoHorarioAula.getHorarioAula().add( itemHorarioAula );
				}

				itemTurno.setHorariosAula( grupoHorarioAula );
				grupoTurno.getTurno().add( itemTurno );
			}

			itemCalendario.setTurnos( grupoTurno );
			grupoCalendario.getCalendario().add( itemCalendario );
		}

		this.triedaInput.setCalendarios( grupoCalendario );
	}

	private void generateTiposSala()
	{
		GrupoTipoSala grupoTipoSala
			= this.of.createGrupoTipoSala();

		List< TipoSala > tipos
			= TipoSala.findAll( this.instituicaoEnsino );

		for ( TipoSala tipo : tipos )
		{
			ItemTipoSala itemTipoSala
				= this.of.createItemTipoSala();

			itemTipoSala.setId( tipo.getId().intValue() );
			itemTipoSala.setNome( tipo.getNome() );

			grupoTipoSala.getTipoSala().add( itemTipoSala );
		}

		this.triedaInput.setTiposSala( grupoTipoSala );
	}

	private void generateTiposContrato()
	{
		GrupoTipoContrato grupoTipoContrato
			= this.of.createGrupoTipoContrato();

		List< TipoContrato > listTiposContrato
			= TipoContrato.findAll( this.instituicaoEnsino );

		for ( TipoContrato tipo : listTiposContrato )
		{
			ItemTipoContrato itemTipoContrato
				= this.of.createItemTipoContrato();

			itemTipoContrato.setId( tipo.getId().intValue() );
			itemTipoContrato.setNome( tipo.getNome() );

			grupoTipoContrato.getTipoContrato().add( itemTipoContrato );
		}

		this.triedaInput.setTiposContrato( grupoTipoContrato );
	}

	private void generateTiposTitulacao()
	{
		GrupoTipoTitulacao grupoTipoTitulacao
			= this.of.createGrupoTipoTitulacao();

		List< Titulacao > tipos
			= Titulacao.findAll( this.instituicaoEnsino );

		for ( Titulacao tipo : tipos )
		{
			ItemTipoTitulacao itemTipoTitulacao = this.of.createItemTipoTitulacao();

			itemTipoTitulacao.setId( tipo.getId().intValue() );
			itemTipoTitulacao.setNome( tipo.getNome() );

			grupoTipoTitulacao.getTipoTitulacao().add( itemTipoTitulacao );
		}

		this.triedaInput.setTiposTitulacao( grupoTipoTitulacao );
	}

	private void generateAreasTitulacao()
	{
		GrupoAreaTitulacao grupoAreaTitulacao
			= this.of.createGrupoAreaTitulacao();

		List< AreaTitulacao > tipos
			= AreaTitulacao.findAll( this.instituicaoEnsino );

		for ( AreaTitulacao tipo : tipos )
		{
			ItemAreaTitulacao itemAreaTitulacao
				= this.of.createItemAreaTitulacao();

			itemAreaTitulacao.setId( tipo.getId().intValue() );
			itemAreaTitulacao.setNome( tipo.getCodigo() );

			grupoAreaTitulacao.getAreaTitulacao().add( itemAreaTitulacao );
		}

		this.triedaInput.setAreasTitulacao( grupoAreaTitulacao );
	}

	private void generateTiposDisciplina()
	{
		GrupoTipoDisciplina grupoTipoDisciplina
			= this.of.createGrupoTipoDisciplina();

		List< TipoDisciplina > tipos	
			= TipoDisciplina.findAll( this.instituicaoEnsino );

		for ( TipoDisciplina tipo : tipos )
		{
			ItemTipoDisciplina itemTipoDisciplina
				= this.of.createItemTipoDisciplina();

			itemTipoDisciplina.setId( tipo.getId().intValue() );
			itemTipoDisciplina.setNome( tipo.getNome() );

			grupoTipoDisciplina.getTipoDisciplina().add( itemTipoDisciplina );
		}

		this.triedaInput.setTiposDisciplina( grupoTipoDisciplina );
	}

	private void generateNiveisDificuldade()
	{
		GrupoNivelDificuldade grupoNivelDificuldade
			= this.of.createGrupoNivelDificuldade();

		for ( Dificuldades dificuldade : Dificuldades.values() )
		{
			ItemNivelDificuldade itemNivelDificuldade
				= this.of.createItemNivelDificuldade();

			itemNivelDificuldade.setId( Dificuldades.toInt( dificuldade ) );
			itemNivelDificuldade.setNome( dificuldade.name() );

			grupoNivelDificuldade.getNivelDificuldade().add( itemNivelDificuldade );
		}

		this.triedaInput.setNiveisDificuldade( grupoNivelDificuldade );
	}

	private void generateTiposCurso()
	{
		GrupoTipoCurso grupoTipoCurso = this.of.createGrupoTipoCurso();
		List< TipoCurso > tipos = TipoCurso.findAll( this.instituicaoEnsino );

		for ( TipoCurso tipo : tipos )
		{
			ItemTipoCurso itemTipoCurso = this.of.createItemTipoCurso();

			itemTipoCurso.setId( tipo.getId().intValue() );
			itemTipoCurso.setNome( tipo.getCodigo() );
			grupoTipoCurso.getTipoCurso().add( itemTipoCurso );
		}

		this.triedaInput.setTiposCurso( grupoTipoCurso );
	}

	private void generateDivisoesDeCredito()
	{
		GrupoDivisaoCreditos grupoDivisaoCreditos
			= this.of.createGrupoDivisaoCreditos();

		Set< DivisaoCredito > regras = this.cenario.getDivisoesCredito();

		if ( this.parametro.getRegrasGenericasDivisaoCredito()
			&& regras.size() == 0 )
		{
			String warningMessage = "O campo de regras gen&eacute;ricas " +
				"de cr&eacute;ditos est&aacute; habililtado, mas n&atilde;o " +
				"h&aacute; nenhuma regra de cr&eacute;dito cadastrada no sistema";

			createWarningMessage( warningMessage );
		}

		for ( DivisaoCredito regra : regras )
		{
			ItemDivisaoCreditos item = this.of.createItemDivisaoCreditos();

			item.setId( regra.getId().intValue() );
			item.setCreditos( regra.getCreditos() );
			item.setDia1( regra.getDia1() );
			item.setDia2( regra.getDia2() );
			item.setDia3( regra.getDia3() );
			item.setDia4( regra.getDia4() );
			item.setDia5( regra.getDia5() );
			item.setDia6( regra.getDia6() );
			item.setDia7( regra.getDia7() );

			grupoDivisaoCreditos.getDivisaoCreditos().add( item );
		}

		this.triedaInput.setRegrasDivisaoCredito( grupoDivisaoCreditos );
	}

	private void generateCampi( boolean tatico )
	{
		GrupoCampus grupoCampus = this.of.createGrupoCampus();

		String warningMessage = "";
		String errorMessage = "";

		boolean existeUnidades = false;
		boolean campusSemUnidades = false;

		boolean existeSalas = false;
		boolean unidadeSemSalas = false;

		boolean grupoSalaVazio = false;

		for ( Campus campus : this.campi )
		{
			ItemCampus itemCampus = this.of.createItemCampus();

			itemCampus.setId( campus.getId().intValue() );
			itemCampus.setCodigo( campus.getCodigo() );
			itemCampus.setNome( campus.getNome() );
			
			Set< HorarioDisponivelCenario > horarios
				= new HashSet< HorarioDisponivelCenario >();

			horarios.addAll( this.getHorarios( campus ) );

			if ( horarios.size() == 0 )
			{
				warningMessage = "O campus " + campus.getCodigo()
					+ " n&atilde;o possui nenhum hor&aacute;rio dispon&iacute;vel na grade.";

				createWarningMessage( warningMessage );
			}

			itemCampus.setHorariosDisponiveis( createGrupoHorario(
				new ArrayList< HorarioDisponivelCenario >( horarios ) ) );

			if ( campus.getValorCredito() == null
				|| campus.getValorCredito() == 0 )
			{
				if ( campus.getValorCredito() == null )
				{
					warningMessage = "O campus " + campus.getCodigo()
						+ " n&atilde;o possui cadastrado o custo m&eacute;dio do cr&eacute;dito.";
				}
				else
				{
					warningMessage = "O campus " + campus.getCodigo()
						+ " possui cadastrado o custo m&eacute;dio do cr&eacute;dito com valor zero.";
				}

				this.createWarningMessage( warningMessage );
			}

			itemCampus.setCusto( campus.getValorCredito() );

			Set< String > salasJaAssociadasADisciplina = new HashSet< String >();

			// COLETANDO UNIDADES
			GrupoUnidade grupoUnidade = this.of.createGrupoUnidade();
			Set< Unidade > unidades = campus.getUnidades();

			if ( unidades.size() != 0 )
			{
				existeUnidades = true;
			}
			else
			{
				campusSemUnidades = true;
			}

			for ( Unidade unidade : unidades )
			{
				ItemUnidade itemUnidade = this.of.createItemUnidade();

				itemUnidade.setId( unidade.getId().intValue() );
				itemUnidade.setCodigo( unidade.getCodigo() );
				itemUnidade.setNome( unidade.getNome() );

				Set< HorarioDisponivelCenario > setHorariosUnidade
					= new HashSet< HorarioDisponivelCenario >();

				setHorariosUnidade.addAll( this.getHorarios( unidade ) );

				List< HorarioDisponivelCenario > listHorariosUnidade
					= new ArrayList< HorarioDisponivelCenario >( setHorariosUnidade );

				if ( listHorariosUnidade.size() == 0 )
				{
					warningMessage = "A unidade " + unidade.getCodigo()
						+ " n&atilde;o possui nenhum hor&aacute;rio dispon&iacute;vel na grade.";

					createWarningMessage( warningMessage );
				}
				
				itemUnidade.setHorariosDisponiveis(
					createGrupoHorario( listHorariosUnidade ) );

				GrupoSala grupoSala = this.of.createGrupoSala();
				Set< Sala > salas = unidade.getSalas();

				if ( salas.size() != 0 )
				{
					existeSalas = true;
				}
				else
				{
					unidadeSemSalas = true;
				}

				for ( Sala sala : salas )
				{
					ItemSala itemSala = this.of.createItemSala();

					itemSala.setId( sala.getId().intValue() );
					itemSala.setCodigo( sala.getCodigo() );
					itemSala.setAndar( sala.getAndar() );
					itemSala.setNumero( sala.getNumero() );
					itemSala.setTipoSalaId(
						sala.getTipoSala().getId().intValue() );
					itemSala.setCapacidade( sala.getCapacidade() );

					Set< HorarioDisponivelCenario > setHorariosSala
						= new HashSet< HorarioDisponivelCenario >();

					setHorariosSala.addAll( this.getHorarios( sala ) );

					// Carregando 'CRÉDITOS' ( modelo tático )
					// ou 'HORÁRIOS' ( modelo operacional )
					List< HorarioDisponivelCenario > listHorariosSala
						= new ArrayList< HorarioDisponivelCenario >( setHorariosSala );

					// TRIEDA-1164: Enviar sempre HoráriosDisponíveis para o solver ao invés de CréditosDisponíveis (no caso do Tático)
//					if ( tatico )
//					{
//						// Tático
//						itemSala.setCreditosDisponiveis(
//							createCreditosDisponiveis( createGrupoHorario( listHorariosSala ) ) );
//					}
//					else
//					{
						// Operacional
						itemSala.setHorariosDisponiveis( createGrupoHorario( listHorariosSala ) );
					//}

					GrupoIdentificador grupoIdentificador = this.of.createGrupoIdentificador();

					Set< CurriculoDisciplina > curriculoDisciplinas = sala.getCurriculoDisciplinas();

					for ( CurriculoDisciplina curriculoDisciplina : curriculoDisciplinas )
					{
						if ( salasJaAssociadasADisciplina.add(
								curriculoDisciplina.getDisciplina().getId() + "-" + sala.getId() ) )
						{
							grupoIdentificador.getId().add(
									curriculoDisciplina.getDisciplina().getId().intValue() );
						}
					}

					itemSala.setDisciplinasAssociadas( grupoIdentificador );

					grupoSala.getSala().add( itemSala );
				}

				if ( grupoSala.getSala().size() == 0 )
				{
					grupoSalaVazio = true;
				}

				itemUnidade.setSalas( grupoSala );

				grupoUnidade.getUnidade().add( itemUnidade );
			}

			itemCampus.setUnidades( grupoUnidade );

			// COLETANDO PROFESSORES
			GrupoProfessor grupoProfessor = this.of.createGrupoProfessor();
			Set< Professor > professores = campus.getProfessores();

			for ( Professor professor : professores )
			{
				ItemProfessor itemProfessor = this.of.createItemProfessor();

				itemProfessor.setId( professor.getId().intValue() );
				itemProfessor.setCpf( professor.getCpf() );
				itemProfessor.setNome( professor.getNome() );
				itemProfessor.setTipoContratoId(
					professor.getTipoContrato().getId().intValue() );
				itemProfessor.setChMin( professor.getCargaHorariaMin() );
				itemProfessor.setChMax( professor.getCargaHorariaMax() );
				itemProfessor.setTitulacaoId(
					professor.getTitulacao().getId().intValue() );

				if ( professor.getAreaTitulacao() != null )
				{
					itemProfessor.setAreaTitulacaoId(
						professor.getAreaTitulacao().getId().intValue() );
				}

				itemProfessor.setCredAnterior( professor.getCreditoAnterior() );
				itemProfessor.setValorCred( professor.getValorCredito() );

				Set< HorarioDisponivelCenario > setHorarios
					= new HashSet< HorarioDisponivelCenario >();

				setHorarios.addAll( this.getHorarios( professor ) );

				List< HorarioDisponivelCenario > listHorarios
					= new ArrayList< HorarioDisponivelCenario >( setHorarios );

				itemProfessor.setHorariosDisponiveis( createGrupoHorario( listHorarios ) );

				GrupoProfessorDisciplina grupoProfessorDisciplina	
					= this.of.createGrupoProfessorDisciplina();

				Set< ProfessorDisciplina > professorDisciplinas = professor.getDisciplinas();

				if ( professorDisciplinas.size() == 0 )
				{
					warningMessage = "O professor " + professor.getNome()
						+ " n&atilde;o possui nenhuma habilita&ccedil;&atilde;o de disciplina associada a ele.";

					createWarningMessage( warningMessage );
				}

				if ( professor.getCampi().size() == 0 )
				{
					warningMessage = "O professor " + professor.getNome()
						+ " n&atilde;o possui nenhum campi de trabalho associado a ele.";

					createWarningMessage( warningMessage );
				}

				for ( ProfessorDisciplina professorDisciplina : professorDisciplinas )
				{
					if ( !this.disciplinasComDemandaCurriculo.contains(
						professorDisciplina.getDisciplina() ) )
					{
						continue;
					}

					ItemProfessorDisciplina itemProfessorDisciplina
						= this.of.createItemProfessorDisciplina();

					itemProfessorDisciplina.setNota( professorDisciplina.getNota() );
					itemProfessorDisciplina.setPreferencia( professorDisciplina.getPreferencia() );
					itemProfessorDisciplina.setDisciplinaId(
						professorDisciplina.getDisciplina().getId().intValue() );

					grupoProfessorDisciplina.getProfessorDisciplina().add( itemProfessorDisciplina );
				}

				itemProfessor.setDisciplinas( grupoProfessorDisciplina );

				grupoProfessor.getProfessor().add( itemProfessor );
			}

			itemCampus.setProfessores( grupoProfessor );

			grupoCampus.getCampus().add( itemCampus );
		}

		// Input inválido
		if ( !existeUnidades )
		{
			errorMessage = "Não h&aacute; nenhuma unidade cadastrada no sistema.";
			createErrorMessage( errorMessage );
		}
		// Input válido, com alertas
		else if( campusSemUnidades )
		{
			warningMessage = "Existem campus sem nenhuma unidade cadastrada no sistema.";
			createWarningMessage( warningMessage );
		}

		// Input inválido
		if ( !existeSalas )
		{
			errorMessage = "Não h&aacute; nenhuma sala cadastrada no sistema.";
			createErrorMessage( errorMessage );
		}
		// Input válido, com alertas
		else if( unidadeSemSalas )
		{
			warningMessage = "Existem unidades sem nenhuma sala cadastrada no sistema.";
			createWarningMessage( warningMessage );
		}

		if ( grupoSalaVazio == true )
		{
			warningMessage = "Existe(m) unidade(s) que possuem grupo(s) de salas vazios.";
			createWarningMessage( warningMessage );
		}

		this.triedaInput.setCampi( grupoCampus );
	}

	private void generateDeslocamentoCampi()
	{
		GrupoDeslocamento grupoDeslocamento = this.of.createGrupoDeslocamento();

		for ( Campus campus : this.campi )
		{
			Set< DeslocamentoCampus > deslocamentos = campus.getDeslocamentos();

			for ( DeslocamentoCampus deslocamento : deslocamentos )
			{
				ItemDeslocamento itemDeslocamento = this.of.createItemDeslocamento();

				itemDeslocamento.setOrigemId(
					deslocamento.getOrigem().getId().intValue() );
				itemDeslocamento.setDestinoId(
					deslocamento.getDestino().getId().intValue() );
				itemDeslocamento.setTempo( deslocamento.getTempo() );
				itemDeslocamento.setCusto( deslocamento.getCusto() );

				grupoDeslocamento.getDeslocamento().add( itemDeslocamento );
			}
		}

		int totalDeslocamentos = this.campi.size() * ( this.campi.size() - 1 );
		int numDeslocamentos = grupoDeslocamento.getDeslocamento().size();

		if ( this.campi.size() > 1 && numDeslocamentos < totalDeslocamentos )
		{
			String warningMessage = "Existem campi que n&atilde;o " +
				"possuem cadastrados o delocamento entre eles.";

			createWarningMessage( warningMessage );
		}

		this.triedaInput.setTemposDeslocamentosCampi( grupoDeslocamento );
	}

	private void generateDeslocamentoUnidades()
	{
		GrupoDeslocamento grupoDeslocamento
			= this.of.createGrupoDeslocamento();
		
		boolean warningDeslocamentos = false;

		for ( Campus campus : this.campi )
		{
			Set< Unidade > unidades = campus.getUnidades();

			final int totalUnidades = unidades.size();

			for ( Unidade unidade : unidades )
			{
				Set< DeslocamentoUnidade > deslocamentos
					= unidade.getDeslocamentos();

				final int totalDeslocamentos = totalUnidades - 1;

				if ( totalUnidades > 1 && deslocamentos.size() < totalDeslocamentos )
				{
					warningDeslocamentos = true;
				}

				for ( DeslocamentoUnidade deslocamento : deslocamentos )
				{
					ItemDeslocamento itemDeslocamento
						= this.of.createItemDeslocamento();

					itemDeslocamento.setOrigemId(
						deslocamento.getOrigem().getId().intValue() );
					itemDeslocamento.setDestinoId(
						deslocamento.getDestino().getId().intValue() );
					itemDeslocamento.setTempo( deslocamento.getTempo() );
					itemDeslocamento.setCusto( deslocamento.getCusto() );

					grupoDeslocamento.getDeslocamento().add( itemDeslocamento );
				}
			}
		}

		if ( warningDeslocamentos )
		{
			String warningMessage = "Existem unidades que n&atilde;o possuem cadastrados todos " +
				"os deslocamentos entre as demais unidades do mesmo campus.";

			createWarningMessage( warningMessage );
		}

		this.triedaInput.setTemposDeslocamentosUnidades( grupoDeslocamento );
	}

	private void generateDisciplinas()
	{
		GrupoDisciplina grupoDisciplina = this.of.createGrupoDisciplina();

		boolean disciplinaSemDemanda = ( this.disciplinasComDemandaCurriculo.size()
			!= this.cenario.getDisciplinas().size() );

		if ( disciplinaSemDemanda )
		{
			String warningMessage1 = "H&aacute; disciplinas cadastradas no sistema" +
				"que n&atilde;o possuem demanda associada a ela.";

			String warningMessage2 = "H&aacute; disciplinas cadastradas no sistema" +
				"que n&atilde;o est&atilde;o associadas a nenhuma matriz curricular.";

			createWarningMessage( warningMessage1 );
			createWarningMessage( warningMessage2 );
		}

		for ( Disciplina disciplina : this.disciplinasComDemandaCurriculo )
		{
			ItemDisciplina itemDisciplina = this.of.createItemDisciplina();

			itemDisciplina.setId( disciplina.getId().intValue() );
			itemDisciplina.setCodigo( disciplina.getCodigo() );
			itemDisciplina.setNome( disciplina.getNome() );
			itemDisciplina.setCredTeoricos( disciplina.getCreditosTeorico() );
			itemDisciplina.setCredPraticos( disciplina.getCreditosPratico() );
			itemDisciplina.setLaboratorio( disciplina.getLaboratorio() );
			itemDisciplina.setMaxAlunosTeorico( disciplina.getMaxAlunosTeorico() );
			itemDisciplina.setMaxAlunosPratico( disciplina.getMaxAlunosPratico() );
			itemDisciplina.setTipoDisciplinaId( disciplina.getTipoDisciplina().getId().intValue() );
			itemDisciplina.setNivelDificuldadeId( Dificuldades.toInt( disciplina.getDificuldade() ) );

			DivisaoCredito divisaoCredito = null;

			if ( this.parametro.getRegrasEspecificasDivisaoCredito() )
			{
				divisaoCredito = disciplina.getDivisaoCreditos();
			}

			if ( divisaoCredito == null
				&& this.parametro.getRegrasGenericasDivisaoCredito() )
			{
				divisaoCredito = DivisaoCredito.findByCredito(
					disciplina.getTotalCreditos(), this.instituicaoEnsino );
			}

			if ( divisaoCredito != null )
			{
				ItemDivisaoCreditos itemDivisaoCreditos = this.of.createItemDivisaoCreditos();

				itemDivisaoCreditos.setId( divisaoCredito.getId().intValue() );
				itemDivisaoCreditos.setCreditos( divisaoCredito.getCreditos() );
				itemDivisaoCreditos.setDia1( divisaoCredito.getDia1() );
				itemDivisaoCreditos.setDia2( divisaoCredito.getDia2() );
				itemDivisaoCreditos.setDia3( divisaoCredito.getDia3() );
				itemDivisaoCreditos.setDia4( divisaoCredito.getDia4() );
				itemDivisaoCreditos.setDia5( divisaoCredito.getDia5() );
				itemDivisaoCreditos.setDia6( divisaoCredito.getDia6() );
				itemDivisaoCreditos.setDia7( divisaoCredito.getDia7() );

				itemDisciplina.setDivisaoDeCreditos( itemDivisaoCreditos );
			}

			GrupoIdentificador grupoIdentificadorEquivalencias = this.of.createGrupoIdentificador();

			if ( this.parametro.getConsiderarEquivalencia() )
			{
				Set< Equivalencia > equivalencias = disciplina.getEquivalencias();

				for ( Equivalencia equivalencia : equivalencias )
				{
					Set< Disciplina > eliminas = equivalencia.getElimina();

					if ( eliminas != null && eliminas.size() > 0 )
					{
						for ( Disciplina disciplinaElimina : eliminas )
						{
							grupoIdentificadorEquivalencias.getId().add(
								disciplinaElimina.getId().intValue() );
						}
					}
				}
			}

			itemDisciplina.setDisciplinasEquivalentes( grupoIdentificadorEquivalencias );

			GrupoIdentificador grupoIdentificadorIncompativeis = this.of.createGrupoIdentificador();
			Set< Incompatibilidade > incompatibilidades = disciplina.getIncompatibilidades();

			for ( Incompatibilidade incompatibilidade : incompatibilidades )
			{
				grupoIdentificadorIncompativeis.getId().add(
					incompatibilidade.getDisciplina2().getId().intValue() );
			}

			itemDisciplina.setDisciplinasIncompativeis( grupoIdentificadorIncompativeis );

			Set< HorarioDisponivelCenario > setHorarios
				= new HashSet< HorarioDisponivelCenario >();

			setHorarios.addAll( this.getHorarios( disciplina ) );

			List< HorarioDisponivelCenario > listHorarios
				= new ArrayList< HorarioDisponivelCenario >( setHorarios );
			
			itemDisciplina.setHorariosDisponiveis( createGrupoHorario( listHorarios ) );

			grupoDisciplina.getDisciplina().add( itemDisciplina );
		}

		this.triedaInput.setDisciplinas( grupoDisciplina );
	}

	// Informa se existe demanda para uma
	// determinada disciplina, no curso informado
	private boolean verificaDemandaCurriculoDisciplina(
		Curso curso, CurriculoDisciplina curriculoPeriodo )
	{
		if ( !this.disciplinasComDemandaCurriculo.contains(
			curriculoPeriodo.getDisciplina() ) )
		{
			return false;
		}

		Disciplina disciplina = curriculoPeriodo.getDisciplina();
		Curriculo curriculo = curriculoPeriodo.getCurriculo();
		Integer periodo = curriculoPeriodo.getPeriodo();

		Set< Oferta > ofertasCandidatas = new HashSet< Oferta >(); 
		Set< Oferta > ofertasCampi = this.parametro.getCampus().getOfertas();

		for ( Oferta oferta : ofertasCampi )
		{
			if ( oferta.getCurriculo().equals( curriculo )
				&& oferta.getCurso().equals( curso )
				&& oferta.getTurno().equals( this.parametro.getTurno() )
				&& oferta.getCampus().equals( this.parametro.getCampus() ) )
			{
				ofertasCandidatas.add( oferta );
			}
		}

		boolean encontrouDemandaPeriodo = false;

		for ( Oferta oferta : ofertasCandidatas )
		{
			Set< Demanda > demandas = oferta.getDemandas();

			for ( Demanda demanda : demandas )
			{
				if ( demanda.getDisciplina().equals( disciplina ) )
				{
					List< Integer > periodosCurriculo
						= this.mapCurriculosPeriodos.get( curriculo );

					// Verifica se o map possui o curriculo atual
					encontrouDemandaPeriodo = ( periodosCurriculo != null );

					// Verifica se o curriculo possui o período informado
					encontrouDemandaPeriodo = ( encontrouDemandaPeriodo
						&& periodosCurriculo.contains( periodo ) );

					if ( encontrouDemandaPeriodo )
					{
						break;
					}
				}

				if ( encontrouDemandaPeriodo )
				{
					break;
				}
			}
		}

		return encontrouDemandaPeriodo;
	}

	private void generateCurso()
	{
		GrupoCurso grupoCurso = this.of.createGrupoCurso();
		Set< Curso > cursos = this.cenario.getCursos();

		String errorMessage = "";
		String warningMessage = "";

		boolean existeCurriculo = false;
		boolean cursoSemCurriculo = false;

		boolean existeCurriculoComDisciplinas = false;
		boolean curriculoSemDisciplinas = false;

		boolean existeCurriculoComOfertas = false;
		boolean curriculoSemOfertas = false;

		for ( Curso curso : cursos )
		{
			ItemCurso itemCurso = this.of.createItemCurso();

			itemCurso.setId( curso.getId().intValue() );
			itemCurso.setCodigo( curso.getCodigo() );
			itemCurso.setTipoId( curso.getTipoCurso().getId().intValue() );

			ItemPercentualMinimo itemPercentualMinimoMestres
				= this.of.createItemPercentualMinimo();

			itemPercentualMinimoMestres.setPercMinimo( curso.getNumMinMestres() );
			itemPercentualMinimoMestres.setTipoTitulacaoId( 4 );
			itemCurso.setRegraPercMinMestres( itemPercentualMinimoMestres );

			ItemPercentualMinimo itemPercentualMinimoDoutores
				= this.of.createItemPercentualMinimo();

			itemPercentualMinimoDoutores.setPercMinimo( curso.getNumMinDoutores() );
			itemPercentualMinimoDoutores.setTipoTitulacaoId( 5 );
			itemCurso.setRegraPercMinDoutores( itemPercentualMinimoDoutores );

			itemCurso.setMinTempoIntegralParcial( curso.getMinTempoIntegralParcial() );
			itemCurso.setMinTempoIntegral( curso.getMinTempoIntegral() );
			itemCurso.setMaisDeUmaDiscPeriodo( curso.getAdmMaisDeUmDisciplina() );

			if ( curso.getMaxDisciplinasPeloProfessor() == 0 )
			{
				errorMessage = "O curso " + curso.getCodigo()
					+ " possui o limite de disciplinas por professor zerado.";

				createErrorMessage( errorMessage );
			}

			itemCurso.setQtdMaxProfDisc( curso.getMaxDisciplinasPeloProfessor() );

			// ÁREA DE TITULAÇÃO
			Set< AreaTitulacao > areas = curso.getAreasTitulacao();
			GrupoIdentificador grupoIdentificadorAreasTitulacao
				= this.of.createGrupoIdentificador();

			for ( AreaTitulacao area : areas )
			{
				grupoIdentificadorAreasTitulacao.getId().add(
					area.getId().intValue() );
			}

			itemCurso.setAreasTitulacao( grupoIdentificadorAreasTitulacao );

			// CURRÍCULOS
			GrupoCurriculo grupoCurriculo = this.of.createGrupoCurriculo();
			Set< Curriculo > curriculos = curso.getCurriculos();

			if ( curriculos.size() > 0 )
			{
				existeCurriculo = true;
			}
			else
			{
				cursoSemCurriculo = true;
			}

			for ( Curriculo curriculo : curriculos )
			{
				ItemCurriculo itemCurriculo = this.of.createItemCurriculo();

				if ( curriculo.getOfertas().size() > 0 )
				{
					existeCurriculoComOfertas = true;
				}
				else
				{
					curriculoSemOfertas = true;
				}

				itemCurriculo.setId( curriculo.getId().intValue() );
				itemCurriculo.setCodigo( curriculo.getCodigo() );
				itemCurriculo.setSemanaLetivaId(
					curriculo.getSemanaLetiva().getId().intValue() );

				GrupoDisciplinaPeriodo grupoDisciplinaPeriodo
					= this.of.createGrupoDisciplinaPeriodo();

				Set< CurriculoDisciplina > curriculoPeriodos
					= curriculo.getDisciplinas();

				if ( curriculoPeriodos.size() != 0 )
				{
					existeCurriculoComDisciplinas = true;
				}
				else
				{
					curriculoSemDisciplinas = true;
				}
				
				for ( CurriculoDisciplina curriculoPeriodo : curriculoPeriodos )
				{
					Boolean existeDemanda = verificaDemandaCurriculoDisciplina( curso, curriculoPeriodo );

					if ( existeDemanda == null || !existeDemanda )
					{
						warningMessage = "N&atilde;o existe demanda cadastrada para a disciplina "
							+ curriculoPeriodo.getDisciplina().getCodigo()
							+ " no per&iacute;odo " + curriculoPeriodo.getPeriodo();

						createWarningMessage( warningMessage );

						continue;
					}

					ItemDisciplinaPeriodo itemDisciplinaPeriodo	
						= this.of.createItemDisciplinaPeriodo();

					itemDisciplinaPeriodo.setPeriodo(
						curriculoPeriodo.getPeriodo() );

					itemDisciplinaPeriodo.setDisciplinaId(
						curriculoPeriodo.getDisciplina().getId().intValue() );

					grupoDisciplinaPeriodo.getDisciplinaPeriodo().add( itemDisciplinaPeriodo );
				}

				itemCurriculo.setDisciplinasPeriodo( grupoDisciplinaPeriodo );

				if ( grupoDisciplinaPeriodo.getDisciplinaPeriodo().size() > 0 )
				{
					grupoCurriculo.getCurriculo().add( itemCurriculo );
				}
			}

			itemCurso.setCurriculos( grupoCurriculo );

			if ( grupoCurriculo.getCurriculo().size() > 0 )
			{
				grupoCurso.getCurso().add( itemCurso );
			}
		}

		// Input inválido
		if ( !existeCurriculo )
		{
			errorMessage = "Nenhum curso possui curr&iacute;culo cadastrado.";
			createErrorMessage( errorMessage );
		}
		// Input válido, com algum alerta
		else if ( cursoSemCurriculo )
		{
			warningMessage = "Existe(m) curso(s) sem nenhum curr&iacute;culo cadastrado.";
			createWarningMessage( warningMessage );
		}

		// Input inválido
		if ( !existeCurriculoComDisciplinas )
		{
			errorMessage = "Nenhum curr&iacute;culo possui disciplinas cadastradas.";
			createErrorMessage( errorMessage );
		}
		// Input válido, com algum alerta
		else if ( curriculoSemDisciplinas )
		{
			warningMessage = "Existe(m) curr&iacute;culo(s) sem nenhuma disciplina cadastrada.";
			createWarningMessage( warningMessage );
		}

		// Input inválido
		if ( !existeCurriculoComOfertas )
		{
			errorMessage = "Nenhum curr&iacute;culo possui ofertas relacionadas.";
			createErrorMessage( errorMessage );
		}
		// Input válido, com algum alerta
		else if ( curriculoSemOfertas )
		{
			warningMessage = "Existe(m) curr&iacute;culo(s) sem nenhuma oferta relacionada.";
			createWarningMessage( warningMessage );
		}

		this.triedaInput.setCursos( grupoCurso );
	}

	private void generateOfertaCursoCampi()
	{
		GrupoOfertaCurso grupoOfertaCurso = this.of.createGrupoOfertaCurso();

		String warningMessage = "";
		String errorMessage = "";

		boolean existeOferta = false;
		boolean campusSemOferta = false;

		for ( Campus campus : this.campi )
		{
			Set< Oferta > ofertas = campus.getOfertas();

			if ( ofertas.size() != 0 )
			{
				existeOferta = true;
			}
			else
			{
				campusSemOferta = true;
			}

			for ( Oferta oferta : ofertas )
			{
				if ( !oferta.getTurno().equals( this.parametro.getTurno() )
					|| !oferta.getCampus().equals( this.parametro.getCampus() ) )
				{
					continue;
				}

				ItemOfertaCurso itemOfertaCurso = this.of.createItemOfertaCurso();
				itemOfertaCurso.setId( oferta.getId().intValue() );

				Curriculo curriculo = oferta.getCurriculo();
				itemOfertaCurso.setCurriculoId( curriculo.getId().intValue());
				itemOfertaCurso.setCursoId( curriculo.getCurso().getId().intValue() );
				itemOfertaCurso.setTurnoId( oferta.getTurno().getId().intValue() );
				itemOfertaCurso.setCampusId( campus.getId().intValue() );
				itemOfertaCurso.setReceita( oferta.getReceita() == null ? 0.0 : oferta.getReceita() );

				grupoOfertaCurso.getOfertaCurso().add( itemOfertaCurso );
			}
		}

		if ( !existeOferta )
		{
			errorMessage = "Não h&aacute; nenhuma oferta cadastrada no sistema.";
			createErrorMessage( errorMessage );
		}
		else if ( campusSemOferta )
		{
			warningMessage = "Existem campus sem nenhuma oferta cadastrada.";
			createWarningMessage( warningMessage );
		}

		this.triedaInput.setOfertaCursosCampi( grupoOfertaCurso );
	}

	private void generateDemandas()
	{
		GrupoDemanda grupoDemanda = this.of.createGrupoDemanda();

		for ( Demanda demanda : this.demandasCampusTurno )
		{
			ItemDemanda itemDemanda = this.of.createItemDemanda();

			itemDemanda.setId( demanda.getId().intValue() );
			itemDemanda.setOfertaCursoCampiId( demanda.getOferta().getId().intValue() );
			itemDemanda.setDisciplinaId( demanda.getDisciplina().getId().intValue() );
			itemDemanda.setQuantidade( demanda.getQuantidade() == null ? 0 : demanda.getQuantidade() );

			grupoDemanda.getDemanda().add( itemDemanda );
		}

		this.triedaInput.setDemandas( grupoDemanda );
	}

	private void generateAlunosDemanda()
	{
		GrupoAlunoDemanda grupoAlunosDemanda
			= this.of.createGrupoAlunoDemanda();

		List< AlunoDemanda > alunos = AlunoDemanda.findAll( this.instituicaoEnsino );

		for ( AlunoDemanda alunoDemanda : alunos )
		{
			boolean contemDemanda = this.demandasCampusTurno.contains( alunoDemanda.getDemanda() );

			boolean contemDisciplina = this.disciplinasComDemandaCurriculo.contains(
				alunoDemanda.getDemanda().getDisciplina() );

			if ( !contemDemanda || !contemDisciplina )
			{
				continue;
			}

			ItemAlunoDemanda itemAlunoDemanda = this.of.createItemAlunoDemanda();

			itemAlunoDemanda.setId( alunoDemanda.getId().intValue() );
			itemAlunoDemanda.setAlunoId( alunoDemanda.getAluno().getId().intValue() );
			itemAlunoDemanda.setNomeAluno( alunoDemanda.getAluno().getNome() );
			itemAlunoDemanda.setDemandaId( alunoDemanda.getDemanda().getId().intValue() );

			grupoAlunosDemanda.getAlunoDemanda().add( itemAlunoDemanda );
		}

		this.triedaInput.setAlunosDemanda( grupoAlunosDemanda );
	}

	private void generateParametrosPlanejamento( boolean tatico )
	{
		ItemParametrosPlanejamento itemParametrosPlanejamento
			= this.of.createItemParametrosPlanejamento();

		itemParametrosPlanejamento.setModoOtimizacao(
			tatico ? "TATICO" : "OPERACIONAL" );

		itemParametrosPlanejamento.setFuncaoObjetivo(
			itemParametrosPlanejamento.getFuncaoObjetivo() );

		CargaHorariaSemanalAluno cargaHorariaSemanalAluno
			= this.of.createItemParametrosPlanejamentoCargaHorariaSemanalAluno();

		cargaHorariaSemanalAluno.setIndiferente( "" );

		if ( this.parametro.getCargaHorariaAluno() )
		{
			if ( parametro.getCargaHorariaAlunoSel().equals(
					CargaHoraria.CONCENTRAR.name() ) )
			{
				cargaHorariaSemanalAluno.setMinimizarDias( "" );
			}
			else if ( parametro.getCargaHorariaAlunoSel().equals(
						CargaHoraria.DISTRIBUIR.name() ) )
			{
				cargaHorariaSemanalAluno.setEquilibrar( "" );
			}
		}

		itemParametrosPlanejamento.setCargaHorariaSemanalAluno( cargaHorariaSemanalAluno );

		itemParametrosPlanejamento.setAlunosMesmoPeriodoNaMesmaSala(
			this.parametro.getAlunoDePeriodoMesmaSala() );

		itemParametrosPlanejamento.setPermitirAlunosEmVariosCampi(
			this.parametro.getAlunoEmMuitosCampi() );

		itemParametrosPlanejamento.setMinimizarDeslocAluno(
			this.parametro.getMinimizarDeslocamentoAluno() );

		CargaHorariaSemanalProfessor cargaHorariaSemanaProfessor
			= this.of.createItemParametrosPlanejamentoCargaHorariaSemanalProfessor();

		cargaHorariaSemanaProfessor.setIndiferente( "" );

		if ( this.parametro.getCargaHorariaProfessor() )
		{
			if ( this.parametro.getCargaHorariaProfessorSel().equals(
					CargaHoraria.CONCENTRAR.name() ) )
			{
				cargaHorariaSemanaProfessor.setMinimizarDias( "" );
			}
			else if ( this.parametro.getCargaHorariaAlunoSel().equals(
						CargaHoraria.DISTRIBUIR.name() ) )
			{
				cargaHorariaSemanaProfessor.setEquilibrar( "" );
			}
		}

		itemParametrosPlanejamento.setCargaHorariaSemanalProfessor( cargaHorariaSemanaProfessor );

		itemParametrosPlanejamento.setPermitirProfessorEmVariosCampi(
			this.parametro.getProfessorEmMuitosCampi() );

		itemParametrosPlanejamento.setMinimizarDeslocProfessor(
			this.parametro.getMinimizarDeslocamentoProfessor() );

		itemParametrosPlanejamento.setMaxDeslocProfessor(
			this.parametro.getMinimizarDeslocamentoProfessorValue() );

		itemParametrosPlanejamento.setMinimizarHorariosVaziosProfessor(
			this.parametro.getMinimizarGapProfessor() );

		itemParametrosPlanejamento.setEvitarReducaoCargaHorariaProfValor(
			this.parametro.getEvitarReducaoCargaHorariaProfessorValue() );

		itemParametrosPlanejamento.setEvitarReducaoCargaHorariaProf(
			this.parametro.getEvitarReducaoCargaHorariaProfessor() );

		itemParametrosPlanejamento.setEvitarProfUltimoPrimeiroHor(
			this.parametro.getEvitarUltimoEPrimeiroHorarioProfessor() );

		itemParametrosPlanejamento.setPreferenciaProfessorDisciplina(
			this.parametro.getPreferenciaDeProfessores() );

		itemParametrosPlanejamento.setDesempenhoProfDisponibilidade(
			this.parametro.getAvaliacaoDesempenhoProfessor() );

		itemParametrosPlanejamento.setConsiderarEquivalencia(
			this.parametro.getConsiderarEquivalencia() );

		itemParametrosPlanejamento.setMinAlunosAberturaTurmas(
			this.parametro.getMinAlunosParaAbrirTurma() );

		itemParametrosPlanejamento.setMinAlunosAberturaTurmasValor(
			this.parametro.getMinAlunosParaAbrirTurmaValue() );

		itemParametrosPlanejamento.setNiveisDificuldadeHorario(
			this.of.createGrupoNivelDificuldadeHorario() );

		itemParametrosPlanejamento.setEquilibrarDiversidadeDiscDia(
			this.parametro.getCompatibilidadeDisciplinasMesmoDia() );

		itemParametrosPlanejamento.setRegrasGenericasDivisaoCredito(
			this.parametro.getRegrasGenericasDivisaoCredito() );

		itemParametrosPlanejamento.setRegrasEspecificasDivisaoCredito(
			this.parametro.getRegrasEspecificasDivisaoCredito() );

		itemParametrosPlanejamento.setMaximizarAvaliacaoCursosSel(
			this.parametro.getMaximizarNotaAvaliacaoCorpoDocente() );

		GrupoIdentificador cursosAvaliacaoGrupo = this.of.createGrupoIdentificador();

		if ( this.parametro.getMaximizarNotaAvaliacaoCorpoDocente() )
		{
			for ( Curso curso : this.parametro.getCursosMaxNotaAval() )
			{
				cursosAvaliacaoGrupo.getId().add( curso.getId().intValue() );
			}
		}

		itemParametrosPlanejamento.setMaximizarAvaliacaoCursos( cursosAvaliacaoGrupo );

		itemParametrosPlanejamento.setMinimizarCustoDocenteCursosSel(
			this.parametro.getMinimizarCustoDocenteCursos() );

		GrupoIdentificador cursosCustoGrupo = this.of.createGrupoIdentificador();

		if ( this.parametro.getMinimizarCustoDocenteCursos() )
		{
			for ( Curso curso : this.parametro.getCursosMinCust() )
			{
				cursosCustoGrupo.getId().add( curso.getId().intValue() );
			}
		}

		itemParametrosPlanejamento.setMinimizarCustoDocenteCursos( cursosCustoGrupo );

		itemParametrosPlanejamento.setPermiteCompartilhamentoTurmaSel(
			this.parametro.getCompartilharDisciplinasCampi() );

		GrupoGrupo cursosCompartilharTurmaGrupo = of.createGrupoGrupo();

		if ( this.parametro.getCompartilharDisciplinasCampi() )
		{
			for ( CursoDescompartilha cursoDescompartilha
				: this.parametro.getCursosDescompartDiscCampi() )
			{
				GrupoIdentificador cursosCompartilharTurmas = this.of.createGrupoIdentificador();

				cursosCompartilharTurmas.getId().add(
					cursoDescompartilha.getCurso1().getId().intValue() );
				cursosCompartilharTurmas.getId().add(
					cursoDescompartilha.getCurso2().getId().intValue() );

				cursosCompartilharTurmaGrupo.getGrupoIdentificador().add( cursosCompartilharTurmas );
			}
		}

		itemParametrosPlanejamento.setPermiteCompartilhamentoTurma( cursosCompartilharTurmaGrupo );

		itemParametrosPlanejamento.setPercentuaisMinimoMestres(
			this.parametro.getPercentuaisMinimosMestres() );

		itemParametrosPlanejamento.setPercentuaisMinimoDoutores(
			this.parametro.getPercentuaisMinimosDoutores() );

		itemParametrosPlanejamento.setAreaTitulacaoProfessorCurso(
			this.parametro.getAreaTitulacaoProfessoresECursos() );

		itemParametrosPlanejamento.setMaximoDisciplinasDeUmProfessorPorCurso(
			this.parametro.getLimitarMaximoDisciplinaProfessor() );

		itemParametrosPlanejamento.setCustoProfDisponibilidade( false );

		this.triedaInput.setParametrosPlanejamento( itemParametrosPlanejamento );
	}

	private void generateFixacoes()
	{
		int id = 1;
		GrupoFixacao grupoFixacao = this.of.createGrupoFixacao();

		List< Fixacao > fixacoes
			= Fixacao.findAll( this.instituicaoEnsino );

		for ( Fixacao fixacao : fixacoes )
		{
			Set< HorarioDisponivelCenario > horarios
				= this.getHorarios( fixacao );

			if ( horarios.size() > 0 )
			{
				for ( HorarioDisponivelCenario horario : horarios )
				{
					if ( !horario.getHorarioAula().getTurno().equals(
							this.parametro.getTurno() ) )
					{
						continue;
					}

					ItemFixacao itemFixacao = this.of.createItemFixacao();

					itemFixacao.setId( id++ );
					itemFixacao.setDiaSemana( Semanas.toInt( horario.getDiaSemana() ) );
					itemFixacao.setTurnoId( horario.getHorarioAula().getTurno().getId().intValue() );
					itemFixacao.setHorarioAulaId( horario.getHorarioAula().getId().intValue() );

					if ( fixacao.getProfessor() != null )
					{
						itemFixacao.setProfessorId(
							fixacao.getProfessor().getId().intValue() );
					}

					if ( fixacao.getDisciplina() != null )
					{
						itemFixacao.setDisciplinaId(
							fixacao.getDisciplina().getId().intValue() );
					}

					if ( fixacao.getSala() != null )
					{
						itemFixacao.setSalaId(
							fixacao.getSala().getId().intValue() );
					}

					grupoFixacao.getFixacao().add( itemFixacao );
				}
			}
			else
			{
				ItemFixacao itemFixacao = this.of.createItemFixacao();

				itemFixacao.setId( id++ );

				if ( fixacao.getProfessor() != null )
				{
					itemFixacao.setProfessorId(
						fixacao.getProfessor().getId().intValue() );
				}

				if ( fixacao.getDisciplina() != null )
				{
					itemFixacao.setDisciplinaId(
						fixacao.getDisciplina().getId().intValue() );
				}

				if ( fixacao.getSala() != null )
				{
					itemFixacao.setSalaId( fixacao.getSala().getId().intValue() );
				}

				grupoFixacao.getFixacao().add( itemFixacao );
			}

		}

		this.triedaInput.setFixacoes( grupoFixacao );
	}

	private void generateTaticoInput()
	{
		Set< AtendimentoTatico > ats = this.cenario.getAtendimentosTaticos();

		for ( AtendimentoTatico at : ats )
		{
			if ( !at.getOferta().getTurno().equals( this.parametro.getTurno() )
				|| !at.getOferta().getCampus().equals( this.parametro.getCampus() ) )
			{
				continue;
			}

			createItemAtendimentoTaticoSolucao( at.getSala(), at.getSemana(),
				at.getOferta(), at.getDisciplina(),
				at.getQuantidadeAlunos(), at.getTurma(),
				at.getCreditosTeorico(), at.getCreditosPratico() );
		}
	}

	private ItemAtendimentoCampusSolucao getItemAtendimentoCampusSolucao( Campus campus )
	{
		if ( this.triedaInput.getAtendimentosTatico() == null )
		{
			this.triedaInput.setAtendimentosTatico(
				this.of.createGrupoAtendimentoCampusSolucao() );
		}

		for ( ItemAtendimentoCampusSolucao atSolucao
			: this.triedaInput.getAtendimentosTatico().getAtendimentoCampus() )
		{
			if ( atSolucao.getCampusId() == campus.getId().intValue() )
			{
				return atSolucao;
			}
		}

		ItemAtendimentoCampusSolucao atSolucao
			= this.of.createItemAtendimentoCampusSolucao();

		atSolucao.setCampusId( campus.getId().intValue() );
		atSolucao.setCampusCodigo( campus.getCodigo() );

		this.triedaInput.getAtendimentosTatico().getAtendimentoCampus().add( atSolucao );

		return atSolucao;
	}

	private ItemAtendimentoUnidadeSolucao getItemAtendimentoUnidadeSolucao( Unidade unidade )
	{
		for ( ItemAtendimentoCampusSolucao atCampusSolucao
			: this.triedaInput.getAtendimentosTatico().getAtendimentoCampus() )
		{
			if ( atCampusSolucao.getAtendimentosUnidades() == null )
			{
				atCampusSolucao.setAtendimentosUnidades(
					this.of.createGrupoAtendimentoUnidadeSolucao() );
			}

			for ( ItemAtendimentoUnidadeSolucao atUnidadeSolucao
				: atCampusSolucao.getAtendimentosUnidades().getAtendimentoUnidade() )
			{
				if ( atUnidadeSolucao.getUnidadeId() == unidade.getId().intValue() )
				{
					return atUnidadeSolucao;
				}
			}

		}

		ItemAtendimentoUnidadeSolucao atUnidadeSolucao
			= this.of.createItemAtendimentoUnidadeSolucao();

		atUnidadeSolucao.setUnidadeId( unidade.getId().intValue() );
		atUnidadeSolucao.setUnidadeCodigo( unidade.getCodigo() );

		ItemAtendimentoCampusSolucao atCampusSolucao
			= getItemAtendimentoCampusSolucao( unidade.getCampus() );

		if ( atCampusSolucao.getAtendimentosUnidades() == null )
		{
			atCampusSolucao.setAtendimentosUnidades(
				this.of.createGrupoAtendimentoUnidadeSolucao() );
		}

		atCampusSolucao.getAtendimentosUnidades().getAtendimentoUnidade().add( atUnidadeSolucao );

		return atUnidadeSolucao;
	}

	private ItemAtendimentoSalaSolucao getItemAtendimentoSalaSolucao( Sala sala )
	{
		if ( this.triedaInput.getAtendimentosTatico() == null )
		{
			this.triedaInput.setAtendimentosTatico(
				this.of.createGrupoAtendimentoCampusSolucao() );
		}

		for ( ItemAtendimentoCampusSolucao atCampusSolucao
			: this.triedaInput.getAtendimentosTatico().getAtendimentoCampus() )
		{
			if ( atCampusSolucao.getAtendimentosUnidades() == null )
			{
				atCampusSolucao.setAtendimentosUnidades(
					this.of.createGrupoAtendimentoUnidadeSolucao() );
			}

			for ( ItemAtendimentoUnidadeSolucao atUnidadeSolucao
				: atCampusSolucao.getAtendimentosUnidades().getAtendimentoUnidade() )
			{
				if ( atUnidadeSolucao.getAtendimentosSalas() == null )
				{
					atUnidadeSolucao.setAtendimentosSalas(
						this.of.createGrupoAtendimentoSalaSolucao() );
				}

				for ( ItemAtendimentoSalaSolucao atSalaSolucao
					: atUnidadeSolucao.getAtendimentosSalas().getAtendimentoSala() )
				{
					if ( atSalaSolucao.getSalaId() == sala.getId().intValue() )
					{
						return atSalaSolucao;
					}
				}
			}
		}

		ItemAtendimentoSalaSolucao atSalaSolucao
			= this.of.createItemAtendimentoSalaSolucao();

		atSalaSolucao.setSalaId( sala.getId().intValue() );
		atSalaSolucao.setSalaNome( sala.getCodigo() );

		ItemAtendimentoUnidadeSolucao atUnidadeSolucao
			= getItemAtendimentoUnidadeSolucao( sala.getUnidade() );

		if ( atUnidadeSolucao.getAtendimentosSalas() == null )
		{
			atUnidadeSolucao.setAtendimentosSalas(
				this.of.createGrupoAtendimentoSalaSolucao() );
		}

		atUnidadeSolucao.getAtendimentosSalas().getAtendimentoSala().add( atSalaSolucao );

		return atSalaSolucao;
	}

	private ItemAtendimentoDiaSemanaSolucao getItemAtendimentoDiaSemanaSolucao(
		Sala sala, Semanas semana )
	{
		if ( this.triedaInput.getAtendimentosTatico() == null )
		{
			this.triedaInput.setAtendimentosTatico(
				this.of.createGrupoAtendimentoCampusSolucao() );
		}

		for ( ItemAtendimentoCampusSolucao atCampusSolucao
			: this.triedaInput.getAtendimentosTatico().getAtendimentoCampus() )
		{
			if ( atCampusSolucao.getAtendimentosUnidades() == null )
			{
				atCampusSolucao.setAtendimentosUnidades(
					this.of.createGrupoAtendimentoUnidadeSolucao() );
			}

			for ( ItemAtendimentoUnidadeSolucao atUnidadeSolucao
				: atCampusSolucao.getAtendimentosUnidades().getAtendimentoUnidade() )
			{
				if ( atUnidadeSolucao.getAtendimentosSalas() == null )
				{
					atUnidadeSolucao.setAtendimentosSalas(
						this.of.createGrupoAtendimentoSalaSolucao() );
				}

				for ( ItemAtendimentoSalaSolucao atSalaSolucao
					: atUnidadeSolucao.getAtendimentosSalas().getAtendimentoSala() )
				{
					if ( atSalaSolucao.getSalaId() != sala.getId().intValue() )
					{
						continue;
					}

					if ( atSalaSolucao.getAtendimentosDiasSemana() == null )
					{
						atSalaSolucao.setAtendimentosDiasSemana(
							this.of.createGrupoAtendimentoDiaSemanaSolucao() );
					}

					for ( ItemAtendimentoDiaSemanaSolucao atDiaSemanaSolucao
						: atSalaSolucao.getAtendimentosDiasSemana().getAtendimentoDiaSemana() )
					{
						if ( Semanas.get( atDiaSemanaSolucao.getDiaSemana() ).equals( semana ) )
						{
							return atDiaSemanaSolucao;
						}
					}
				}
			}
		}

		ItemAtendimentoDiaSemanaSolucao atDiaSemanaSolucao
			= this.of.createItemAtendimentoDiaSemanaSolucao();

		atDiaSemanaSolucao.setDiaSemana( Semanas.toInt( semana ) );
		ItemAtendimentoSalaSolucao atSalaSolucao
			= getItemAtendimentoSalaSolucao( sala );

		if ( atSalaSolucao.getAtendimentosDiasSemana() == null )
		{
			atSalaSolucao.setAtendimentosDiasSemana(
				this.of.createGrupoAtendimentoDiaSemanaSolucao() );
		}

		atSalaSolucao.getAtendimentosDiasSemana().getAtendimentoDiaSemana().add( atDiaSemanaSolucao );

		return atDiaSemanaSolucao;
	}

	private ItemAtendimentoTaticoSolucao createItemAtendimentoTaticoSolucao(
		Sala sala, Semanas semana, Oferta oferta, Disciplina disciplina,
		int quantidade, String turma, int qtdCreditosTeoricos, int qtdCreditosPraticos )
	{
		ItemAtendimentoDiaSemanaSolucao atDiaSemanaSolucao
			= getItemAtendimentoDiaSemanaSolucao( sala, semana );

		ItemAtendimentoOfertaSolucao atOfertaSolucao
			= this.of.createItemAtendimentoOfertaSolucao();

		atOfertaSolucao.setOfertaCursoCampiId( oferta.getId().intValue() );
		atOfertaSolucao.setDisciplinaId( disciplina.getId().intValue() );
		atOfertaSolucao.setQuantidade( quantidade );
		atOfertaSolucao.setTurma( turma );

		ItemAtendimentoTaticoSolucao atTaticoSolucao
			= this.of.createItemAtendimentoTaticoSolucao();

		atTaticoSolucao.setAtendimentoOferta( atOfertaSolucao );
		atTaticoSolucao.setQtdeCreditosPraticos( qtdCreditosPraticos );
		atTaticoSolucao.setQtdeCreditosTeoricos( qtdCreditosTeoricos );

		if ( atDiaSemanaSolucao.getAtendimentosTatico() == null )
		{
			atDiaSemanaSolucao.setAtendimentosTatico(
				this.of.createGrupoAtendimentoTaticoSolucao() );
		}

		atDiaSemanaSolucao.getAtendimentosTatico().getAtendimentoTatico().add( atTaticoSolucao );

		return atTaticoSolucao;
	}

	private GrupoHorario createGrupoHorario(
		Collection< HorarioDisponivelCenario > horarios )
	{
		GrupoHorario grupoHorario = this.of.createGrupoHorario();

		for ( HorarioDisponivelCenario horarioDisponivelCenario : horarios )
		{
			HorarioAula horarioAula = horarioDisponivelCenario.getHorarioAula();
			Semanas semana = horarioDisponivelCenario.getDiaSemana();
			ItemHorario itemHorarioAux = null;

			for ( ItemHorario itemHorario : grupoHorario.getHorario() )
			{
				if ( itemHorario.getHorarioAulaId() == horarioAula.getId() )
				{
					itemHorarioAux = itemHorario;
					break;
				}
			}

			if ( itemHorarioAux != null )
			{
				itemHorarioAux.getDiasSemana().getDiaSemana().add( Semanas.toInt( semana ) );
			}
			else
			{
				if ( !horarioAula.getTurno().equals( this.parametro.getTurno() ) )
				{
					continue;
				}

				itemHorarioAux = this.of.createItemHorario();

				itemHorarioAux.setHorarioAulaId( horarioAula.getId().intValue() );
				itemHorarioAux.setTurnoId(
					horarioAula.getTurno().getId().intValue() );
				itemHorarioAux.setDiasSemana( this.of.createGrupoDiaSemana() );
				itemHorarioAux.getDiasSemana().getDiaSemana().add( Semanas.toInt( semana ) );

				grupoHorario.getHorario().add( itemHorarioAux );
			}
		}

		return grupoHorario;
	}

	private GrupoCreditoDisponivel createCreditosDisponiveis(
		GrupoHorario horariosDisponiveis )
	{
		GrupoCreditoDisponivel grupoCreditoDisponivel
			= this.of.createGrupoCreditoDisponivel();

		List< ItemHorario > horarios = horariosDisponiveis.getHorario();

		for ( Semanas semana : Semanas.values() )
		{
			for ( Turno turno : this.cenario.getTurnos() )
			{
				if ( !turno.equals( this.parametro.getTurno() ) )
				{
					continue;
				}

				ItemCreditoDisponivel itemCD = this.of.createItemCreditoDisponivel();

				itemCD.setDiaSemana( Semanas.toInt( semana ) );
				itemCD.setTurnoId( turno.getId().intValue() );
				itemCD.setMaxCreditos( 0 );

				for ( ItemHorario itemHorario : horarios )
				{
					if ( itemHorario.getTurnoId() == itemCD.getTurnoId()
						&& itemHorario.getDiasSemana().getDiaSemana().contains(
							Semanas.toInt( semana ) ) )
					{
						itemCD.setMaxCreditos( itemCD.getMaxCreditos() + 1 );
					}
				}

				if ( itemCD.getMaxCreditos() > 0 )
				{
					grupoCreditoDisponivel.getCreditoDisponivel().add( itemCD );
				}
			}
		}

		return grupoCreditoDisponivel;
	}

	private void createWarningMessage( String warningMessage )
	{
		if ( this.getWarnings().size() <= 10 )
		{
			this.getWarnings().add( HtmlUtils.htmlUnescape( warningMessage ) );
		}
	}

	private void createErrorMessage( String errorMessage )
	{
		if ( this.getErrors().size() <= 10 )
		{
			this.getErrors().add( HtmlUtils.htmlUnescape( errorMessage ) );
		}
	}
}
