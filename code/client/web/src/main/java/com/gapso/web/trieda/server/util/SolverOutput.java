package com.gapso.web.trieda.server.util;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
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

@Transactional
public class SolverOutput
{
	private InstituicaoEnsino instituicaoEnsino;
	private Cenario cenario;
	private TriedaOutput triedaOutput;
	private List< AtendimentoTatico > atendimentosTatico;
	private List< AtendimentoOperacional > atendimentosOperacional;
	private Map<AlunoDemanda, List<AtendimentoTatico>> alunosDemandaTatico;
	private Map<AlunoDemanda, List<AtendimentoOperacional>> alunosDemandaOperacional;
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
		
		this.alunosDemandaTatico = new HashMap<AlunoDemanda, List<AtendimentoTatico>>();
		this.alunosDemandaOperacional = new HashMap<AlunoDemanda, List<AtendimentoOperacional>>();
	}

	@Transactional
	public List< AtendimentoTatico > generateAtendimentosTatico() throws Exception
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
							Disciplina disciplinaSubstituta = null;
							if (itemAtendimentoOferta.getDisciplinaSubstitutaId() != null) {
								disciplinaSubstituta = Disciplina.find(Long.valueOf(itemAtendimentoOferta.getDisciplinaSubstitutaId()),this.instituicaoEnsino);
							}

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
							atendimentoTatico.setDisciplinaSubstituta(disciplinaSubstituta);
							atendimentoTatico.setQuantidadeAlunos( quantidade );
							
							Set<AlunoDemanda> listAlunoDemanda = atendimentoAlunoDemandaAssoc(itemAtendimentoOferta, alunosDemandaTatico, atendimentoTatico);
							atendimentoTatico.getAlunosDemanda().addAll(listAlunoDemanda);

							this.atendimentosTatico.add( atendimentoTatico );
						}
					}
				}
			}
		}

		return this.atendimentosTatico;
	}

	@Transactional
	public List< AtendimentoOperacional > generateAtendimentosOperacional() throws Exception
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
									Disciplina disciplinaSubstituta = null;
									if (itemAtendimentoOferta.getDisciplinaSubstitutaId() != null) {
										disciplinaSubstituta = Disciplina.find(Long.valueOf(itemAtendimentoOferta.getDisciplinaSubstitutaId()),this.instituicaoEnsino);
									}

									int quantidade = itemAtendimentoOferta.getQuantidade();
									String turma = itemAtendimentoOferta.getTurma();

									AtendimentoOperacional atendimentoOperacional = new AtendimentoOperacional();

									atendimentoOperacional.setInstituicaoEnsino( this.instituicaoEnsino );
									atendimentoOperacional.setCenario( this.cenario );
									atendimentoOperacional.setTurma( turma );
									atendimentoOperacional.setSala( sala );
									atendimentoOperacional.setOferta( oferta );
									atendimentoOperacional.setDisciplina( disciplina );
									atendimentoOperacional.setDisciplinaSubstituta(disciplinaSubstituta);
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

									Set<AlunoDemanda> listAlunoDemanda = atendimentoAlunoDemandaAssoc(itemAtendimentoOferta, alunosDemandaOperacional, atendimentoOperacional);
									atendimentoOperacional.getAlunosDemanda().addAll(listAlunoDemanda);
									
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
	
	private <T> Set<AlunoDemanda> atendimentoAlunoDemandaAssoc(ItemAtendimentoOferta itemAtendimentoOferta, 
		Map<AlunoDemanda, List<T>> alunosDemandaAtendimento, T atendimento) throws Exception
	{
		Set<AlunoDemanda> listAlunoDemanda = new HashSet<AlunoDemanda>();
		for(Integer i : itemAtendimentoOferta.getAlunosDemandasAtendidas().getId()){
			AlunoDemanda alunoDemanda = AlunoDemanda.find((long) i, this.instituicaoEnsino);
			if(alunoDemanda != null){
				listAlunoDemanda.add(alunoDemanda);
				List<T> atendimentoAluno = alunosDemandaAtendimento.get(alunoDemanda);
				if(atendimentoAluno == null){
					atendimentoAluno = new ArrayList<T>();
					alunosDemandaAtendimento.put(alunoDemanda, atendimentoAluno);
				}
				atendimentoAluno.add(atendimento);
			}
			else{
				throw new Exception("Erro ao tentar associar atendimentos a demandas de alunos inexistentes");
			}
		}
		
		return listAlunoDemanda;
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
	public void salvarAtendimentosTatico(Set<Campus> campi, Turno turno) {
		removerTodosAtendimentosTaticoJaSalvos(campi,turno);
		removerTodosAtendimentosOperacionalJaSalvos(campi,turno);

		for (AtendimentoTatico at : this.atendimentosTatico) {
			at.detach();
			at.persist();
		}
		
		for(AlunoDemanda alunoDemanda : alunosDemandaTatico.keySet()){
			int creditosAtendidos = 0;
			for(AtendimentoTatico tatico : alunosDemandaTatico.get(alunoDemanda)){
				alunoDemanda.getAtendimentosTatico().add(tatico);
				creditosAtendidos += tatico.getCreditosPratico() + tatico.getCreditosTeorico();
			}
			alunoDemanda.setAtendido(alunoDemanda.getDemanda().getDisciplina().getCreditosTotal() == creditosAtendidos);
			alunoDemanda.merge();
		}
	}

	@Transactional
	public void salvarAtendimentosOperacional(Set<Campus> campi, Turno turno) {
		removerTodosAtendimentosTaticoJaSalvos(campi,turno);
		removerTodosAtendimentosOperacionalJaSalvos(campi,turno);

		for (AtendimentoOperacional at : this.atendimentosOperacional) {
			at.detach();
			at.persist();
		}
		
		for(AlunoDemanda alunoDemanda : alunosDemandaOperacional.keySet()){
			int creditosAtendidos = 0;
			for(AtendimentoOperacional operacional : alunosDemandaOperacional.get(alunoDemanda)){
				alunoDemanda.getAtendimentosOperacional().add(operacional);
				creditosAtendidos += 1;
			}
			alunoDemanda.setAtendido(alunoDemanda.getDemanda().getDisciplina().getCreditosTotal() == creditosAtendidos);
			alunoDemanda.merge();
		}
		
	}

	@Transactional
	private void removerTodosAtendimentosTaticoJaSalvos(Set<Campus> campi, Turno turno) {
		List<AtendimentoTatico> atendimentosTatico = AtendimentoTatico.findAllBy(instituicaoEnsino,campi,turno);
		for (AtendimentoTatico at : atendimentosTatico) {
			at.remove();
		}
	}

	@Transactional
	private void removerTodosAtendimentosOperacionalJaSalvos(Set<Campus> campi, Turno turno) {
		List<AtendimentoOperacional> atendimentosOperacional = AtendimentoOperacional.findAllBy(campi,turno,instituicaoEnsino);
		for (AtendimentoOperacional at : atendimentosOperacional) {
			at.remove();
		}
	}

	@Transactional
	public void atualizarAlunosDemanda(Set<Campus> campi, Turno turno){
		List<AlunoDemanda> alunosDemandasBD = AlunoDemanda.findByCampusAndTurno(instituicaoEnsino,campi,turno);
		for(AlunoDemanda alunoDemandaBD : alunosDemandasBD){
			if(alunoDemandaBD.getAtendido()){
				alunoDemandaBD.setAtendido(false);
				alunoDemandaBD.merge();
			}
		}
	}
	
}
