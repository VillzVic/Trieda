package com.gapso.web.trieda.server.util;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.transaction.annotation.Transactional;

import com.gapso.trieda.domain.AreaTitulacao;
import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
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
public class SolverOutput {

	private Cenario cenario;
	private TriedaOutput triedaOutput;
	private List<AtendimentoTatico> atendimentosTatico;
	private List<AtendimentoOperacional> atendimentosOperacional;

	public SolverOutput(Cenario cenario, TriedaOutput triedaOutput) {
		this.cenario = cenario;
		this.triedaOutput = triedaOutput;
		atendimentosTatico = new ArrayList<AtendimentoTatico>();
		atendimentosOperacional = new ArrayList<AtendimentoOperacional>();
	}

	@Transactional
	public List<AtendimentoTatico> generateAtendimentosTatico() {
		List<ItemAtendimentoCampus> itemAtendimentoCampusList =  triedaOutput.getAtendimentos().getAtendimentoCampus();
		for(ItemAtendimentoCampus itemAtendimentoCampus : itemAtendimentoCampusList) {
			// Não há necessidade de procurar campus, pq essa informação já existe em sala
			// Campus campus = Campus.find(Long.valueOf(itemAtendimentoCampus.getCampusId()));
			List<ItemAtendimentoUnidade> itemAtendimentoUnidadeList = itemAtendimentoCampus.getAtendimentosUnidades().getAtendimentoUnidade();
			for(ItemAtendimentoUnidade itemAtendimentoUnidade : itemAtendimentoUnidadeList) {
				// Não há necessidade de procurar campus, pq essa informação já existe em sala
				// Unidade unidade = Unidade.find(Long.valueOf(itemAtendimentoUnidade.getUnidadeId()));
				List<ItemAtendimentoSala> itemAtendimentoSalaList = itemAtendimentoUnidade.getAtendimentosSalas().getAtendimentoSala();
				for(ItemAtendimentoSala itemAtendimentoSala : itemAtendimentoSalaList) {
					Sala sala = Sala.find(Long.valueOf(itemAtendimentoSala.getSalaId()));
					List<ItemAtendimentoDiaSemana> itemAtendimentoDiaSemanaList = itemAtendimentoSala.getAtendimentosDiasSemana().getAtendimentoDiaSemana();
					for(ItemAtendimentoDiaSemana itemAtendimentoDiaSemana : itemAtendimentoDiaSemanaList) {
						Semanas semana = Semanas.get(itemAtendimentoDiaSemana.getDiaSemana());
						if(itemAtendimentoDiaSemana.getAtendimentosTatico() == null) continue;
						
						// COLETANDO INFORMAÇÕES DO TÁTICO
						List<ItemAtendimentoTatico> itemAtendimentoTaticoList = itemAtendimentoDiaSemana.getAtendimentosTatico().getAtendimentoTatico();
						for(ItemAtendimentoTatico itemAtendimentoTatico : itemAtendimentoTaticoList) {
							int qtdeCreditosTeoricos = itemAtendimentoTatico.getQtdeCreditosTeoricos();
							int qtdeCreditosPraticos = itemAtendimentoTatico.getQtdeCreditosPraticos();
							ItemAtendimentoOferta itemAtendimentoOferta = itemAtendimentoTatico.getAtendimentoOferta();
							Oferta oferta = Oferta.find(Long.valueOf(itemAtendimentoOferta.getOfertaCursoCampiId()));
							Disciplina disciplina = Disciplina.find(Long.valueOf(itemAtendimentoOferta.getDisciplinaId()));
							int quantidade = itemAtendimentoOferta.getQuantidade();
							String turma = "TURMA" + itemAtendimentoOferta.getTurma();
							
							AtendimentoTatico atendimentoTatico = new AtendimentoTatico();
							atendimentoTatico.setCenario(cenario);
							atendimentoTatico.setTurma(turma);
							atendimentoTatico.setSala(sala);
							atendimentoTatico.setSemana(semana);
							atendimentoTatico.setCreditosTeorico(qtdeCreditosTeoricos);
							atendimentoTatico.setCreditosPratico(qtdeCreditosPraticos);
							atendimentoTatico.setOferta(oferta);
							atendimentoTatico.setDisciplina(disciplina);
							atendimentoTatico.setQuantidadeAlunos(quantidade);
							
							atendimentosTatico.add(atendimentoTatico);
						}
						
					}
				}
			}
		}
		return atendimentosTatico;
	}
	
	@Transactional
	public List<AtendimentoOperacional> generateAtendimentosOperacional() {
		
		Map<Integer, ProfessorVirtual> virtuais = new HashMap<Integer, ProfessorVirtual>();
		
		List<ItemAtendimentoCampus> itemAtendimentoCampusList =  triedaOutput.getAtendimentos().getAtendimentoCampus();
		for(ItemAtendimentoCampus itemAtendimentoCampus : itemAtendimentoCampusList) {
			// Não há necessidade de procurar campus, pq essa informação já existe em sala
			// Campus campus = Campus.find(Long.valueOf(itemAtendimentoCampus.getCampusId()));
			List<ItemAtendimentoUnidade> itemAtendimentoUnidadeList = itemAtendimentoCampus.getAtendimentosUnidades().getAtendimentoUnidade();
			for(ItemAtendimentoUnidade itemAtendimentoUnidade : itemAtendimentoUnidadeList) {
				// Não há necessidade de procurar campus, pq essa informação já existe em sala
				// Unidade unidade = Unidade.find(Long.valueOf(itemAtendimentoUnidade.getUnidadeId()));
				List<ItemAtendimentoSala> itemAtendimentoSalaList = itemAtendimentoUnidade.getAtendimentosSalas().getAtendimentoSala();
				for(ItemAtendimentoSala itemAtendimentoSala : itemAtendimentoSalaList) {
					Sala sala = Sala.find(Long.valueOf(itemAtendimentoSala.getSalaId()));
					List<ItemAtendimentoDiaSemana> itemAtendimentoDiaSemanaList = itemAtendimentoSala.getAtendimentosDiasSemana().getAtendimentoDiaSemana();
					for(ItemAtendimentoDiaSemana itemAtendimentoDiaSemana : itemAtendimentoDiaSemanaList) {
						Semanas semana = Semanas.get(itemAtendimentoDiaSemana.getDiaSemana());
						if(itemAtendimentoDiaSemana.getAtendimentosTatico() == null) continue;
						
						// COLETANDO INFORMAÇÕES DO OPERACIONAL
						List<ItemAtendimentoTurno> itemAtendimentoTurnoList = itemAtendimentoDiaSemana.getAtendimentosTurnos().getAtendimentoTurno();
						for(ItemAtendimentoTurno itemAtendimentoTurno : itemAtendimentoTurnoList) {
							// Não há necessidade do turno
							// Turno turno = Turno.find(Long.valueOf(itemAtendimentoTurno.getTurnoId()));
							List<ItemAtendimentoHorarioAula> itemAtendimentoHorarioAulaList = itemAtendimentoTurno.getAtendimentosHorariosAula().getAtendimentoHorarioAula();
							for(ItemAtendimentoHorarioAula itemAtendimentoHorarioAula : itemAtendimentoHorarioAulaList) {
								HorarioAula horarioAula = HorarioAula.find(Long.valueOf(itemAtendimentoHorarioAula.getHorarioAulaId()));
								Professor professor = null;
								ProfessorVirtual professorVirtual = null;
								Integer idProfessor = itemAtendimentoHorarioAula.getProfessorId();
								if(!itemAtendimentoHorarioAula.isVirtual()) {
									professor = Professor.find(Long.valueOf(idProfessor));
								} else {
									Integer professorVirtualIdAux = idProfessor * -1;
									if(virtuais.containsKey(professorVirtualIdAux)) {
										professorVirtual = virtuais.get(professorVirtualIdAux); 
									} else {
										professorVirtual = getProfessorVirtual(professorVirtualIdAux);
									}
								}
								boolean creditoTeorico = itemAtendimentoHorarioAula.isCreditoTeorico();
								List<ItemAtendimentoOferta> itemAtendimentoOfertaList = itemAtendimentoHorarioAula.getAtendimentosOfertas().getAtendimentoOferta();
								for(ItemAtendimentoOferta itemAtendimentoOferta : itemAtendimentoOfertaList) {
									Oferta oferta = Oferta.find(Long.valueOf(itemAtendimentoOferta.getOfertaCursoCampiId()));
									Disciplina disciplina = Disciplina.find(Long.valueOf(itemAtendimentoOferta.getDisciplinaId()));
									int quantidade = itemAtendimentoOferta.getQuantidade();
									String turma = "TURMA" + itemAtendimentoOferta.getTurma();
									
									AtendimentoOperacional atendimentoOperacional = new AtendimentoOperacional();
									atendimentoOperacional.setCenario(cenario);
									atendimentoOperacional.setTurma(turma);
									atendimentoOperacional.setSala(sala);
									atendimentoOperacional.setOferta(oferta);
									atendimentoOperacional.setDisciplina(disciplina);
									atendimentoOperacional.setQuantidadeAlunos(quantidade);
									if(!itemAtendimentoHorarioAula.isVirtual()) {
										atendimentoOperacional.setProfessor(professor);
									} else {
										atendimentoOperacional.setProfessorVirtual(professorVirtual);
									}
									atendimentoOperacional.setCreditoTeorico(creditoTeorico);
									HorarioDisponivelCenario horarioDisponivelCenario = HorarioDisponivelCenario.findBy(horarioAula, semana);
									atendimentoOperacional.setHorarioDisponivelCenario(horarioDisponivelCenario);
									
									atendimentosOperacional.add(atendimentoOperacional);
								}
							}
						}
					}
				}
			}
		}
		return atendimentosOperacional;
	}
	
	private ProfessorVirtual getProfessorVirtual(Integer idAux) {
		for(ItemProfessorVirtual pvAux : triedaOutput.getProfessoresVirtuais().getProfessorVirtual()) {
			if(idAux.equals(pvAux.getId())) {
				ProfessorVirtual pv = new ProfessorVirtual();
				pv.setAreaTitulacao(AreaTitulacao.find(pvAux.getAreaTitulacaoId().longValue()));
				pv.setCargaHorariaMax(pvAux.getChMax());
				pv.setCargaHorariaMin(pvAux.getChMin());
				for(Integer discId : pvAux.getDisciplinas().getId()) {
					pv.getDisciplinas().add(Disciplina.find(discId.longValue()));
				}
				pv.setTitulacao(Titulacao.find(Integer.valueOf(pvAux.getTitulacaoId()).longValue()));
				pv.persist();
				return pv;
			}
		}
		return null;
	}
	
	@Transactional
	public void salvarAtendimentosTatico(Campus campus, Turno turno) {
		removerTodosAtendimentosTaticoJaSalvos(campus, turno);
		for(AtendimentoTatico atendimentoTatico : atendimentosTatico) {
			atendimentoTatico.persist();
		}
	}
	
	@Transactional
	private void removerTodosAtendimentosTaticoJaSalvos(Campus campus, Turno turno) {
		List<AtendimentoTatico> atendimentosTatico = AtendimentoTatico.findAllBy(campus, turno);
		for(AtendimentoTatico atendimentoTatico : atendimentosTatico) {
			atendimentoTatico.remove();
		}
	}
	
	@Transactional
	public void salvarAtendimentosOperacional(Campus campus, Turno turno) {
		removerTodosAtendimentosOperacionalJaSalvos(campus, turno);
		for(AtendimentoOperacional atendimentoOperacional : atendimentosOperacional) {
			atendimentoOperacional.persist();
		}
	}
	
	@Transactional
	private void removerTodosAtendimentosOperacionalJaSalvos(Campus campus, Turno turno) {
		List<AtendimentoOperacional> atendimentosOperacional = AtendimentoOperacional.findAllBy(campus, turno);
		for(AtendimentoOperacional atendimentoOperacional : atendimentosOperacional) {
			atendimentoOperacional.remove();
		}
	}

}
