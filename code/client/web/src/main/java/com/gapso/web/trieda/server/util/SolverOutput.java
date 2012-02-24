package com.gapso.web.trieda.server.util;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.springframework.transaction.annotation.Transactional;

import com.gapso.trieda.domain.AlunoDemanda;
import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.ProfessorVirtual;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.Titulacao;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.xml.output.ItemAlunoDemanda;
import com.gapso.web.trieda.server.xml.output.ItemAtendimentoCampus;
import com.gapso.web.trieda.server.xml.output.ItemAtendimentoDiaSemana;
import com.gapso.web.trieda.server.xml.output.ItemAtendimentoHorarioAula;
import com.gapso.web.trieda.server.xml.output.ItemAtendimentoOferta;
import com.gapso.web.trieda.server.xml.output.ItemAtendimentoSala;
import com.gapso.web.trieda.server.xml.output.ItemAtendimentoTatico;
import com.gapso.web.trieda.server.xml.output.ItemAtendimentoTurno;
import com.gapso.web.trieda.server.xml.output.ItemAtendimentoUnidade;
import com.gapso.web.trieda.server.xml.output.ItemProfessorVirtual;
import com.gapso.web.trieda.server.xml.output.TriedaOutput;
import com.google.gwt.dev.util.collect.HashSet;

@Transactional
public class SolverOutput
{
	private InstituicaoEnsino instituicaoEnsino;
	private Cenario cenario;
	private TriedaOutput triedaOutput;
	private List< AtendimentoTatico > atendimentosTatico;
	private List< AtendimentoOperacional > atendimentosOperacional;
	protected Map< Integer, ProfessorVirtual > professoresVirtuais
		= new HashMap< Integer, ProfessorVirtual >();

	public SolverOutput( InstituicaoEnsino instituicaoEnsino,
		Cenario cenario, TriedaOutput triedaOutput )
	{
		this.cenario = cenario;
		this.triedaOutput = triedaOutput;
		this.instituicaoEnsino = instituicaoEnsino;

		this.atendimentosTatico = new ArrayList< AtendimentoTatico >();
		this.atendimentosOperacional = new ArrayList< AtendimentoOperacional >();
	}

	@Transactional
	public List< AtendimentoTatico > generateAtendimentosTatico()
	{
		List< ItemAtendimentoCampus > itemAtendimentoCampusList
			= this.triedaOutput.getAtendimentos().getAtendimentoCampus();

		for ( ItemAtendimentoCampus itemAtendimentoCampus : itemAtendimentoCampusList )
		{
			List< ItemAtendimentoUnidade > itemAtendimentoUnidadeList
				= itemAtendimentoCampus.getAtendimentosUnidades().getAtendimentoUnidade();

			for ( ItemAtendimentoUnidade itemAtendimentoUnidade : itemAtendimentoUnidadeList )
			{
				List< ItemAtendimentoSala > itemAtendimentoSalaList
					= itemAtendimentoUnidade.getAtendimentosSalas().getAtendimentoSala();

				for ( ItemAtendimentoSala itemAtendimentoSala : itemAtendimentoSalaList )
				{
					Sala sala = Sala.find( Long.valueOf(
						itemAtendimentoSala.getSalaId() ), this.instituicaoEnsino );

					List< ItemAtendimentoDiaSemana > itemAtendimentoDiaSemanaList
						= itemAtendimentoSala.getAtendimentosDiasSemana().getAtendimentoDiaSemana();

					for ( ItemAtendimentoDiaSemana itemAtendimentoDiaSemana
						: itemAtendimentoDiaSemanaList )
					{
						Semanas semana = Semanas.get(
							itemAtendimentoDiaSemana.getDiaSemana() );

						if ( itemAtendimentoDiaSemana.getAtendimentosTatico() == null )
						{
							continue;
						}

						// COLETANDO INFORMAÇÕES DO TÁTICO
						List< ItemAtendimentoTatico > itemAtendimentoTaticoList
							= itemAtendimentoDiaSemana.getAtendimentosTatico().getAtendimentoTatico();

						for ( ItemAtendimentoTatico itemAtendimentoTatico
							: itemAtendimentoTaticoList )
						{
							int qtdeCreditosTeoricos = itemAtendimentoTatico.getQtdeCreditosTeoricos();
							int qtdeCreditosPraticos = itemAtendimentoTatico.getQtdeCreditosPraticos();

							ItemAtendimentoOferta itemAtendimentoOferta
								= itemAtendimentoTatico.getAtendimentoOferta();

							Oferta oferta = Oferta.find( Long.valueOf(
								itemAtendimentoOferta.getOfertaCursoCampiId() ), this.instituicaoEnsino );

							Disciplina disciplina = Disciplina.find( Long.valueOf(
								itemAtendimentoOferta.getDisciplinaId() ), this.instituicaoEnsino );

							int quantidade = itemAtendimentoOferta.getQuantidade();
							String turma = itemAtendimentoOferta.getTurma();

							AtendimentoTatico atendimentoTatico = new AtendimentoTatico();

							atendimentoTatico.setInstituicaoEnsino( this.instituicaoEnsino );
							atendimentoTatico.setCenario( this.cenario );
							atendimentoTatico.setTurma( turma );
							atendimentoTatico.setSala( sala );
							atendimentoTatico.setSemana( semana );
							atendimentoTatico.setCreditosTeorico( qtdeCreditosTeoricos );
							atendimentoTatico.setCreditosPratico( qtdeCreditosPraticos );
							atendimentoTatico.setOferta( oferta );
							atendimentoTatico.setDisciplina( disciplina );
							atendimentoTatico.setQuantidadeAlunos( quantidade );

							this.atendimentosTatico.add( atendimentoTatico );
						}
					}
				}
			}
		}

		return this.atendimentosTatico;
	}

	@Transactional
	public List< AtendimentoOperacional > generateAtendimentosOperacional()
	{
		Map< Integer, ProfessorVirtual > virtuais
			= new HashMap< Integer, ProfessorVirtual >();

		List< ItemAtendimentoCampus > itemAtendimentoCampusList
			= this.triedaOutput.getAtendimentos().getAtendimentoCampus();

		for ( ItemAtendimentoCampus itemAtendimentoCampus
			: itemAtendimentoCampusList )
		{
			List< ItemAtendimentoUnidade > itemAtendimentoUnidadeList
				= itemAtendimentoCampus.getAtendimentosUnidades().getAtendimentoUnidade();

			for ( ItemAtendimentoUnidade itemAtendimentoUnidade : itemAtendimentoUnidadeList )
			{
				List< ItemAtendimentoSala > itemAtendimentoSalaList
					= itemAtendimentoUnidade.getAtendimentosSalas().getAtendimentoSala();

				for ( ItemAtendimentoSala itemAtendimentoSala : itemAtendimentoSalaList )
				{
					Sala sala = Sala.find( Long.valueOf(
						itemAtendimentoSala.getSalaId() ), this.instituicaoEnsino );

					List< ItemAtendimentoDiaSemana > itemAtendimentoDiaSemanaList
						= itemAtendimentoSala.getAtendimentosDiasSemana().getAtendimentoDiaSemana();

					for ( ItemAtendimentoDiaSemana itemAtendimentoDiaSemana
						: itemAtendimentoDiaSemanaList )
					{
						Semanas semana = Semanas.get(
							itemAtendimentoDiaSemana.getDiaSemana() );

						if ( itemAtendimentoDiaSemana.getAtendimentosTurnos() == null )
						{
							continue;
						}

						// COLETANDO INFORMAÇÕES DO OPERACIONAL
						List< ItemAtendimentoTurno>  itemAtendimentoTurnoList
							= itemAtendimentoDiaSemana.getAtendimentosTurnos().getAtendimentoTurno();

						for ( ItemAtendimentoTurno itemAtendimentoTurno
							: itemAtendimentoTurnoList )
						{
							List< ItemAtendimentoHorarioAula > itemAtendimentoHorarioAulaList
								= itemAtendimentoTurno.getAtendimentosHorariosAula().getAtendimentoHorarioAula();

							for ( ItemAtendimentoHorarioAula itemAtendimentoHorarioAula
								: itemAtendimentoHorarioAulaList )
							{
								HorarioAula horarioAula = HorarioAula.find( Long.valueOf(
									itemAtendimentoHorarioAula.getHorarioAulaId() ), this.instituicaoEnsino );

								Professor professor = null;
								ProfessorVirtual professorVirtual = null;

								Integer idProfessor = itemAtendimentoHorarioAula.getProfessorId();

								if ( !itemAtendimentoHorarioAula.isVirtual() )
								{
									professor = Professor.find( Long.valueOf(
										idProfessor ), this.instituicaoEnsino );
								}
								else
								{
									Integer professorVirtualIdAux = ( idProfessor * -1 );

									if ( virtuais.containsKey( professorVirtualIdAux ) )
									{
										professorVirtual = virtuais.get( professorVirtualIdAux );
									}
									else
									{
										professorVirtual = getProfessorVirtual( professorVirtualIdAux );
									}
								}

								boolean creditoTeorico = itemAtendimentoHorarioAula.isCreditoTeorico();

								List< ItemAtendimentoOferta > itemAtendimentoOfertaList
									= itemAtendimentoHorarioAula.getAtendimentosOfertas().getAtendimentoOferta();

								for ( ItemAtendimentoOferta itemAtendimentoOferta : itemAtendimentoOfertaList )
								{
									Oferta oferta = Oferta.find( Long.valueOf(
										itemAtendimentoOferta.getOfertaCursoCampiId() ), this.instituicaoEnsino );

									Disciplina disciplina = Disciplina.find( Long.valueOf(
										itemAtendimentoOferta.getDisciplinaId() ), this.instituicaoEnsino );

									int quantidade = itemAtendimentoOferta.getQuantidade();
									String turma = itemAtendimentoOferta.getTurma();

									AtendimentoOperacional atendimentoOperacional = new AtendimentoOperacional();

									atendimentoOperacional.setInstituicaoEnsino( this.instituicaoEnsino );
									atendimentoOperacional.setCenario( this.cenario );
									atendimentoOperacional.setTurma( turma );
									atendimentoOperacional.setSala( sala );
									atendimentoOperacional.setOferta( oferta );
									atendimentoOperacional.setDisciplina( disciplina );
									atendimentoOperacional.setQuantidadeAlunos( quantidade );

									if ( !itemAtendimentoHorarioAula.isVirtual() )
									{
										atendimentoOperacional.setProfessor( professor );
									}
									else
									{
										atendimentoOperacional.setProfessorVirtual( professorVirtual );
									}

									atendimentoOperacional.setCreditoTeorico( creditoTeorico );
									HorarioDisponivelCenario horarioDisponivelCenario
										= HorarioDisponivelCenario.findBy(
											this.instituicaoEnsino, horarioAula, semana );

									atendimentoOperacional.setHorarioDisponivelCenario( horarioDisponivelCenario );

									this.atendimentosOperacional.add( atendimentoOperacional );
								}
							}
						}
					}
				}
			}
		}

		return this.atendimentosOperacional;
	}

	private ProfessorVirtual getProfessorVirtual( Integer idAux )
	{
		ProfessorVirtual professorVirtualAux
			= this.professoresVirtuais.get( idAux );

		if ( professorVirtualAux != null )
		{
			return professorVirtualAux;
		}

		for ( ItemProfessorVirtual pvAux :
			this.triedaOutput.getProfessoresVirtuais().getProfessorVirtual() )
		{
			if ( idAux.equals( pvAux.getId() * -1 ) )
			{
				ProfessorVirtual pv = new ProfessorVirtual();

				pv.setCargaHorariaMax( pvAux.getChMax() );
				pv.setCargaHorariaMin( pvAux.getChMin() );
				pv.setInstituicaoEnsino( this.instituicaoEnsino );

				for ( Integer discId : pvAux.getDisciplinas().getId() )
				{
					Disciplina disciplina = Disciplina.find(
						Math.abs( discId.longValue() ), this.instituicaoEnsino );

					if ( disciplina != null )
					{
						pv.getDisciplinas().add( disciplina );
					}
				}

				pv.setTitulacao( Titulacao.find( Integer.valueOf(
					pvAux.getTitulacaoId() ).longValue(), this.instituicaoEnsino ) );

				pv.persist();
				this.professoresVirtuais.put( idAux, pv );

				return pv;
			}
		}

		return null;
	}

	@Transactional
	public void salvarAtendimentosTatico( Campus campus, Turno turno )
	{
		removerTodosAtendimentosTaticoJaSalvos( campus, turno );
		removerTodosAtendimentosOperacionalJaSalvos( campus, turno );

		for ( AtendimentoTatico at : this.atendimentosTatico )
		{
			at.detach();
			at.persist();
		}
	}

	@Transactional
	public void salvarAtendimentosOperacional( Campus campus, Turno turno )
	{
		removerTodosAtendimentosTaticoJaSalvos( campus, turno );
		removerTodosAtendimentosOperacionalJaSalvos( campus, turno );

		for ( AtendimentoOperacional at : this.atendimentosOperacional )
		{
			at.detach();
			at.persist();
		}
	}

	@Transactional
	private void removerTodosAtendimentosTaticoJaSalvos(
		Campus campus, Turno turno )
	{
		List< AtendimentoTatico > atendimentosTatico	
			= AtendimentoTatico.findAllBy(
				this.instituicaoEnsino, campus, turno );

		for ( AtendimentoTatico at : atendimentosTatico )
		{
			at.remove();
		}
	}

	@Transactional
	private void removerTodosAtendimentosOperacionalJaSalvos(
		Campus campus, Turno turno )
	{
		InstituicaoEnsino instituicaoEnsinoCampus = ( campus == null ?
			null : campus.getInstituicaoEnsino() );

		InstituicaoEnsino instituicaoEnsinoTurno = ( turno == null ?
			null : turno.getInstituicaoEnsino() );

		if ( ( instituicaoEnsinoCampus != null || instituicaoEnsinoTurno != null )
			&& instituicaoEnsinoCampus.getId() != instituicaoEnsinoTurno.getId() )
		{
			System.out.println(
				"Operação inválida: remover atendimentos operacionais " +
				"que correspondem a instituições de ensino diferentes." );

			return;
		}

		List< AtendimentoOperacional > atendimentosOperacional
			= AtendimentoOperacional.findAllBy( campus, turno, instituicaoEnsinoCampus );

		for ( AtendimentoOperacional at : atendimentosOperacional )
		{
			at.remove();
		}
	}

	@Transactional
	public void salvarAlunosDemanda(Campus campus, Turno turno) {
		// 
		Map<Long,Long> alunosDemandasAtendidasIdsMap = new HashMap<Long,Long>();
		for (ItemAlunoDemanda itemAlunoDemanda : triedaOutput.getAlunosDemanda().getAlunoDemanda()) {
			long id = itemAlunoDemanda.getId();
			alunosDemandasAtendidasIdsMap.put(id,id);
		}
		
		List<AlunoDemanda> alunosDemandasBD = AlunoDemanda.findByCampusAndTurno(instituicaoEnsino,campus,turno);
		Set<AlunoDemanda> alunosDemandasAlteradosBD = new HashSet<AlunoDemanda>(); 
		for (AlunoDemanda alunoDemandaBD : alunosDemandasBD) {
			if(alunosDemandasAtendidasIdsMap.get(alunoDemandaBD.getId()) != null) {
				if (!alunoDemandaBD.getAtendido()) {
					alunoDemandaBD.setAtendido(true);
					alunosDemandasAlteradosBD.add(alunoDemandaBD);
				}
			} else {
				if (alunoDemandaBD.getAtendido()) {
					alunoDemandaBD.setAtendido(false);
					alunosDemandasAlteradosBD.add(alunoDemandaBD);
				}
			}
		}
		
		for (AlunoDemanda alunoDemandaAlteradoBD : alunosDemandasAlteradosBD) {
			alunoDemandaAlteradoBD.merge();
		}
	}
}
