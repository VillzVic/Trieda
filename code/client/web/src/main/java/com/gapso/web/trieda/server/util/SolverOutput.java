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
import com.gapso.trieda.domain.Unidade;
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
	public List<AtendimentoTatico> generateAtendimentosTatico(Turno turnoSelecionado) throws Exception {
		// [AlunoDemandaId -> AlunoDemanda]
		Map<Long,AlunoDemanda> alunoDemandaIdToAlunoDemandaMap = createAlunoDemandasMap(turnoSelecionado);
		
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
							
							Set<AlunoDemanda> listAlunoDemanda = atendimentoAlunoDemandaAssoc(itemAtendimentoOferta,alunosDemandaTatico,atendimentoTatico,alunoDemandaIdToAlunoDemandaMap);
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
	public List<AtendimentoOperacional> generateAtendimentosOperacional(Turno turnoSelecionado) throws Exception {
		// [SalaId -> Sala]
		Map<Long,Sala> salaIdToSalaMap = new HashMap<Long,Sala>();
		// [OfertaId -> Oferta]
		Map<Long,Oferta> ofertaIdToOfertaMap = new HashMap<Long,Oferta>();
		for (Campus campus : this.cenario.getCampi()) {
			for (Unidade unidade : campus.getUnidades()) {
				for (Sala sala : unidade.getSalas()) {
					salaIdToSalaMap.put(sala.getId(),sala);
				}
			}
			for (Oferta oferta : campus.getOfertas()) {
				ofertaIdToOfertaMap.put(oferta.getId(),oferta);
			}
		}
		
		// [HorarioAulaId -> Sala]
		Map<Long,HorarioAula> horarioAulaIdToHorarioAulaMap = new HashMap<Long,HorarioAula>();
		// [InstituicaoEnsinoId-HorarioAulaId-DiaSemanaId -> HorarioDisponivelCenario]
		Map<String,HorarioDisponivelCenario> horarioDisponivelCenarioIdToHorarioDisponivelCenarioMap = new HashMap<String,HorarioDisponivelCenario>();
		for (Turno turno : this.cenario.getTurnos()) {
			for (HorarioAula horarioAula : turno.getHorariosAula()) {
				horarioAulaIdToHorarioAulaMap.put(horarioAula.getId(),horarioAula);
				for (HorarioDisponivelCenario hdc : horarioAula.getHorariosDisponiveisCenario()) {
					String key = this.instituicaoEnsino.getId() + "-" + horarioAula.getId() + "-" + hdc.getDiaSemana().ordinal();
					horarioDisponivelCenarioIdToHorarioDisponivelCenarioMap.put(key,hdc);
				}
			}
		}
		
		// [ProfessorId -> Professor]
		Map<Long,Professor> professorIdToProfessorMap = new HashMap<Long,Professor>();
		for (Professor professor : this.cenario.getProfessores()) {
			professorIdToProfessorMap.put(professor.getId(),professor);
		}
		
		// [DisciplinaId -> Disciplina]
		Map<Long,Disciplina> disciplinaIdToDisciplinaMap = new HashMap<Long,Disciplina>();
		for (Disciplina disciplina : this.cenario.getDisciplinas()) {
			disciplinaIdToDisciplinaMap.put(disciplina.getId(),disciplina);
		}
		
		// [TitulacaoId -> Titulacao]
		Map<Long,Titulacao> titulacaoIdToTitulacaoMap = new HashMap<Long,Titulacao>();
		List<Titulacao> titulacoes = Titulacao.findAll(instituicaoEnsino);
		for (Titulacao titulacao : titulacoes) {
			titulacaoIdToTitulacaoMap.put(titulacao.getId(),titulacao);
		}
		
		// [ProfessorVirtualId -> ProfessorVirtual]
		Map<Long,ProfessorVirtual> professorVirtualIdToProfessorVirtualMap = new HashMap<Long,ProfessorVirtual>();
		for (ItemProfessorVirtual itemProfessorVirtual : this.triedaOutput.getProfessoresVirtuais().getProfessorVirtual()) {
			Long id = Long.valueOf(-itemProfessorVirtual.getId());
			if (!professorVirtualIdToProfessorVirtualMap.containsKey(id)) {
				ProfessorVirtual pv = new ProfessorVirtual();
				
				pv.setCargaHorariaMax(itemProfessorVirtual.getChMax());
				pv.setCargaHorariaMin(itemProfessorVirtual.getChMin());
				pv.setInstituicaoEnsino(this.instituicaoEnsino);
				pv.setTitulacao(titulacaoIdToTitulacaoMap.get(Long.valueOf(itemProfessorVirtual.getTitulacaoId())));// Titulacao.find(Integer.valueOf(itemProfessorVirtual.getTitulacaoId()).longValue(),this.instituicaoEnsino));
				for (Integer disciplinaId : itemProfessorVirtual.getDisciplinas().getId()) {
					Disciplina disciplina = disciplinaIdToDisciplinaMap.get(Long.valueOf(Math.abs(disciplinaId)));//Disciplina.find(Math.abs(disciplinaId.longValue()),this.instituicaoEnsino);
					if (disciplina != null) {
						pv.getDisciplinas().add(disciplina);
					}
				}
	
				pv.persist();
				
				professorVirtualIdToProfessorVirtualMap.put(id,pv);
			}
		}
		
		// [AlunoDemandaId -> AlunoDemanda]
		Map<Long,AlunoDemanda> alunoDemandaIdToAlunoDemandaMap = createAlunoDemandasMap(turnoSelecionado);
		
		List<ItemAtendimentoCampus> itemAtendimentoCampusList = this.triedaOutput.getAtendimentos().getAtendimentoCampus();
		// para cada campi
		for (ItemAtendimentoCampus itemAtendimentoCampus : itemAtendimentoCampusList) {
			List<ItemAtendimentoUnidade> itemAtendimentoUnidadeList = itemAtendimentoCampus.getAtendimentosUnidades().getAtendimentoUnidade();
			// para cada unidade
			for (ItemAtendimentoUnidade itemAtendimentoUnidade : itemAtendimentoUnidadeList) {
				List<ItemAtendimentoSala> itemAtendimentoSalaList = itemAtendimentoUnidade.getAtendimentosSalas().getAtendimentoSala();
				// para cada sala
				for (ItemAtendimentoSala itemAtendimentoSala : itemAtendimentoSalaList) {
					// obtém sala
					Sala sala = salaIdToSalaMap.get(Long.valueOf(itemAtendimentoSala.getSalaId()));//Sala.find(Long.valueOf(itemAtendimentoSala.getSalaId()),this.instituicaoEnsino);
					List<ItemAtendimentoDiaSemana> itemAtendimentoDiaSemanaList = itemAtendimentoSala.getAtendimentosDiasSemana().getAtendimentoDiaSemana();
					// para cada dia da semana
					for (ItemAtendimentoDiaSemana itemAtendimentoDiaSemana : itemAtendimentoDiaSemanaList) {
						if (itemAtendimentoDiaSemana.getAtendimentosTurnos() == null) {
							continue;
						}
						// obtém dia da semana
						Semanas diaSemana = Semanas.get(itemAtendimentoDiaSemana.getDiaSemana());
						List<ItemAtendimentoTurno>  itemAtendimentoTurnoList = itemAtendimentoDiaSemana.getAtendimentosTurnos().getAtendimentoTurno();
						// para cada turno
						for (ItemAtendimentoTurno itemAtendimentoTurno : itemAtendimentoTurnoList) {
							List<ItemAtendimentoHorarioAula> itemAtendimentoHorarioAulaList = itemAtendimentoTurno.getAtendimentosHorariosAula().getAtendimentoHorarioAula();
							// para cada horário de aual
							for (ItemAtendimentoHorarioAula itemAtendimentoHorarioAula : itemAtendimentoHorarioAulaList) {
								// obtém horário aula
								HorarioAula horarioAula = horarioAulaIdToHorarioAulaMap.get(Long.valueOf(itemAtendimentoHorarioAula.getHorarioAulaId()));//HorarioAula.find(Long.valueOf(itemAtendimentoHorarioAula.getHorarioAulaId()),this.instituicaoEnsino);
								// obtém o horário disponível cenário
								String hdcKey = this.instituicaoEnsino.getId() + "-" + horarioAula.getId() + "-" + diaSemana.ordinal();
								HorarioDisponivelCenario horarioDisponivelCenario = horarioDisponivelCenarioIdToHorarioDisponivelCenarioMap.get(hdcKey);

								// obtém professor
								Professor professor = null;
								ProfessorVirtual professorVirtual = null;
								if (!itemAtendimentoHorarioAula.isVirtual()) {
									professor = professorIdToProfessorMap.get(Long.valueOf(itemAtendimentoHorarioAula.getProfessorId())); //Professor.find(Long.valueOf(itemAtendimentoHorarioAula.getProfessorId()),this.instituicaoEnsino);
								} else {
									professorVirtual = professorVirtualIdToProfessorVirtualMap.get(Long.valueOf(-itemAtendimentoHorarioAula.getProfessorId()));
								}

								boolean creditoTeorico = itemAtendimentoHorarioAula.isCreditoTeorico();

								List<ItemAtendimentoOferta> itemAtendimentoOfertaList = itemAtendimentoHorarioAula.getAtendimentosOfertas().getAtendimentoOferta();
								// para cada atendimento oferta
								for (ItemAtendimentoOferta itemAtendimentoOferta : itemAtendimentoOfertaList) {
									// obtém oferta
									Oferta oferta = ofertaIdToOfertaMap.get(Long.valueOf(itemAtendimentoOferta.getOfertaCursoCampiId()));//Oferta.find(Long.valueOf(itemAtendimentoOferta.getOfertaCursoCampiId()),this.instituicaoEnsino);
									// obtém disciplina
									Disciplina disciplina = disciplinaIdToDisciplinaMap.get(Long.valueOf(itemAtendimentoOferta.getDisciplinaId()));//Disciplina.find(Long.valueOf(itemAtendimentoOferta.getDisciplinaId()),this.instituicaoEnsino);
									// obtém disciplina substituta
									Disciplina disciplinaSubstituta = null;
									if (itemAtendimentoOferta.getDisciplinaSubstitutaId() != null) {
										disciplinaSubstituta = disciplinaIdToDisciplinaMap.get(Long.valueOf(itemAtendimentoOferta.getDisciplinaSubstitutaId()));//Disciplina.find(Long.valueOf(itemAtendimentoOferta.getDisciplinaSubstitutaId()),this.instituicaoEnsino);
									}

									int quantidade = itemAtendimentoOferta.getQuantidade();
									String turma = itemAtendimentoOferta.getTurma();

									AtendimentoOperacional atendimentoOperacional = new AtendimentoOperacional();

									atendimentoOperacional.setInstituicaoEnsino(this.instituicaoEnsino);
									atendimentoOperacional.setCenario(this.cenario);
									atendimentoOperacional.setTurma(turma);
									atendimentoOperacional.setSala(sala);
									atendimentoOperacional.setOferta(oferta);
									atendimentoOperacional.setDisciplina(disciplina);
									atendimentoOperacional.setDisciplinaSubstituta(disciplinaSubstituta);
									atendimentoOperacional.setQuantidadeAlunos(quantidade);
									if (!itemAtendimentoHorarioAula.isVirtual()) {
										atendimentoOperacional.setProfessor(professor);
									} else {
										atendimentoOperacional.setProfessorVirtual(professorVirtual);
									}
									atendimentoOperacional.setCreditoTeorico(creditoTeorico);
									atendimentoOperacional.setHorarioDisponivelCenario(horarioDisponivelCenario);//HorarioDisponivelCenario horarioDisponivelCenario= HorarioDisponivelCenario.findBy(this.instituicaoEnsino,horarioAula,diaSemana);

									Set<AlunoDemanda> listAlunoDemanda = atendimentoAlunoDemandaAssoc(itemAtendimentoOferta,alunosDemandaOperacional,atendimentoOperacional,alunoDemandaIdToAlunoDemandaMap);
									atendimentoOperacional.getAlunosDemanda().addAll(listAlunoDemanda);
									
									this.atendimentosOperacional.add(atendimentoOperacional);
								}
							}
						}
					}
				}
			}
		}

		return this.atendimentosOperacional;
	}

	private Map<Long, AlunoDemanda> createAlunoDemandasMap(Turno turnoSelecionado) {
		// [AlunoDemandaId -> AlunoDemanda]
		Map<Long,AlunoDemanda> alunoDemandaIdToAlunoDemandaMap = new HashMap<Long,AlunoDemanda>();
		AlunoDemanda.entityManager().flush();
		List<AlunoDemanda> alunosDemanda = AlunoDemanda.findByCampusAndTurno(this.instituicaoEnsino,this.cenario.getCampi(),turnoSelecionado);
		for (AlunoDemanda alunoDemanda : alunosDemanda) {
			alunoDemandaIdToAlunoDemandaMap.put(alunoDemanda.getId(),alunoDemanda);
		}
		return alunoDemandaIdToAlunoDemandaMap;
	}
	
	private <T> Set<AlunoDemanda> atendimentoAlunoDemandaAssoc(ItemAtendimentoOferta itemAtendimentoOferta, Map<AlunoDemanda,List<T>> alunosDemandaAtendimento, T atendimento, Map<Long,AlunoDemanda> alunoDemandaIdToAlunoDemandaMap) throws Exception {
		Set<AlunoDemanda> listAlunoDemanda = new HashSet<AlunoDemanda>();
		for(Integer i : itemAtendimentoOferta.getAlunosDemandasAtendidas().getId()){
			AlunoDemanda alunoDemanda = alunoDemandaIdToAlunoDemandaMap.get((long)i);//AlunoDemanda.find((long) i, this.instituicaoEnsino);
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

	@Transactional
	public void salvarAtendimentosTatico(Set<Campus> campi, Turno turno) {
		removerTodosAtendimentosTaticoJaSalvos(campi,turno);
		removerTodosAtendimentosOperacionalJaSalvos(campi,turno);

		for (AtendimentoTatico at : this.atendimentosTatico) {
			at.detach();
			at.persist();
		}
		
		AlunoDemanda.entityManager().flush();
		
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
		
		AtendimentoOperacional.entityManager().flush();
		
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
