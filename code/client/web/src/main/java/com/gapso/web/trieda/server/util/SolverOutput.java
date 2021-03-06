package com.gapso.web.trieda.server.util;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import javax.persistence.TransactionRequiredException;

import org.springframework.transaction.annotation.Transactional;

import com.gapso.trieda.domain.AlunoDemanda;
import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.DicaEliminacaoProfessorVirtual;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.MotivoUsoProfessorVirtual;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.ProfessorVirtual;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.TipoContrato;
import com.gapso.trieda.domain.Titulacao;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.domain.Unidade;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.xml.output.GrupoAlocacoes;
import com.gapso.web.trieda.server.xml.output.GrupoHorarioAula;
import com.gapso.web.trieda.server.xml.output.ItemAlocacao;
import com.gapso.web.trieda.server.xml.output.ItemAtendimentoCampus;
import com.gapso.web.trieda.server.xml.output.ItemAtendimentoDiaSemana;
import com.gapso.web.trieda.server.xml.output.ItemAtendimentoHorarioAula;
import com.gapso.web.trieda.server.xml.output.ItemAtendimentoOferta;
import com.gapso.web.trieda.server.xml.output.ItemAtendimentoSala;
import com.gapso.web.trieda.server.xml.output.ItemAtendimentoTatico;
import com.gapso.web.trieda.server.xml.output.ItemAtendimentoTurno;
import com.gapso.web.trieda.server.xml.output.ItemAtendimentoUnidade;
import com.gapso.web.trieda.server.xml.output.ItemDicaEliminacao;
import com.gapso.web.trieda.server.xml.output.ItemFolgaAlunoDemanda;
import com.gapso.web.trieda.server.xml.output.ItemMotivoDeUso;
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
	public void generateMotivosNaoAtendimento()
	{
		Map<Integer, AlunoDemanda> alunoDemandaIdMapAlunoDemanda = AlunoDemanda.buildAlunoDemandaIdToAlunoDemandaMap(instituicaoEnsino, cenario);
		for (ItemFolgaAlunoDemanda motivosNaoAtendimento: this.triedaOutput.getFolgaAlunoDemanda().getFolgaAlunoDemanda())
		{
			AlunoDemanda alunoDemanda =  alunoDemandaIdMapAlunoDemanda.get(motivosNaoAtendimento.getId());
			if (alunoDemanda != null)
			{
				String motivoString = "";
				for (String motivo : motivosNaoAtendimento.getMotivos().getMotivo())
				{
					motivoString += motivo + "\n";
				}
				alunoDemanda.setMotivoNaoAtendimento(motivoString);
			}
		}
	}

	//@Transactional
	public List<AtendimentoTatico> generateAtendimentosTatico(Set<Turno> turnosSelecionados) throws Exception {
		Map<Long,Sala> salasMap = new HashMap<Long,Sala>();// [SalaId -> Sala]
		Map<Long,Oferta> ofertasMap = new HashMap<Long,Oferta>();// [OfertaId -> Oferta]
		Map<Long,Disciplina> disciplinasMap = new HashMap<Long,Disciplina>();// [DisciplinaId -> Disciplina]
		Map<Long,HorarioAula> horarioAulaIdToHorarioAulaMap = new HashMap<Long,HorarioAula>();// [HorarioAulaId -> Sala]
		Map<String,HorarioDisponivelCenario> horarioDisponivelCenarioIdToHorarioDisponivelCenarioMap = new HashMap<String,HorarioDisponivelCenario>();// [InstituicaoEnsinoId-HorarioAulaId-DiaSemanaId -> HorarioDisponivelCenario]
		preencheMaps(salasMap,ofertasMap,disciplinasMap,horarioAulaIdToHorarioAulaMap,horarioDisponivelCenarioIdToHorarioDisponivelCenarioMap);
		
		// [AlunoDemandaId -> AlunoDemanda]
		Map<Long,AlunoDemanda> alunoDemandaIdToAlunoDemandaMap = createAlunoDemandasMap(turnosSelecionados);
		
		List< ItemAtendimentoCampus > itemAtendimentoCampusList = this.triedaOutput.getAtendimentos().getAtendimentoCampus();
		for ( ItemAtendimentoCampus itemAtendimentoCampus : itemAtendimentoCampusList ) {
			List< ItemAtendimentoUnidade > itemAtendimentoUnidadeList = itemAtendimentoCampus.getAtendimentosUnidades().getAtendimentoUnidade();
			for ( ItemAtendimentoUnidade itemAtendimentoUnidade : itemAtendimentoUnidadeList ) {
				List< ItemAtendimentoSala > itemAtendimentoSalaList = itemAtendimentoUnidade.getAtendimentosSalas().getAtendimentoSala();
				for ( ItemAtendimentoSala itemAtendimentoSala : itemAtendimentoSalaList ) {
					Sala sala = salasMap.get(Long.valueOf(itemAtendimentoSala.getSalaId()));//Sala.find( Long.valueOf(itemAtendimentoSala.getSalaId() ), this.instituicaoEnsino );
					List< ItemAtendimentoDiaSemana > itemAtendimentoDiaSemanaList = itemAtendimentoSala.getAtendimentosDiasSemana().getAtendimentoDiaSemana();
					for ( ItemAtendimentoDiaSemana itemAtendimentoDiaSemana : itemAtendimentoDiaSemanaList ) {
						Semanas semana = Semanas.get( itemAtendimentoDiaSemana.getDiaSemana() );
						if ( itemAtendimentoDiaSemana.getAtendimentosTatico() == null ) {
							continue;
						}

						// COLETANDO INFORMAÇÕES DO TÁTICO
						List< ItemAtendimentoTatico > itemAtendimentoTaticoList = itemAtendimentoDiaSemana.getAtendimentosTatico().getAtendimentoTatico();
						for ( ItemAtendimentoTatico itemAtendimentoTatico : itemAtendimentoTaticoList ) {
							int qtdeCreditosTeoricos = itemAtendimentoTatico.getQtdeCreditosTeoricos();
							int qtdeCreditosPraticos = itemAtendimentoTatico.getQtdeCreditosPraticos();

							ItemAtendimentoOferta itemAtendimentoOferta = itemAtendimentoTatico.getAtendimentoOferta();
							Oferta oferta = ofertasMap.get(Long.valueOf(itemAtendimentoOferta.getOfertaCursoCampiId()));//Oferta.find( Long.valueOf( itemAtendimentoOferta.getOfertaCursoCampiId() ), this.instituicaoEnsino );

							Disciplina disciplina = disciplinasMap.get(Long.valueOf(itemAtendimentoOferta.getDisciplinaId()));//Disciplina.find( Long.valueOf( itemAtendimentoOferta.getDisciplinaId() ), this.instituicaoEnsino );
							Disciplina disciplinaSubstituta = null;
							if (itemAtendimentoOferta.getDisciplinaSubstitutaId() != null) {
								disciplinaSubstituta = disciplinasMap.get(Long.valueOf(itemAtendimentoOferta.getDisciplinaSubstitutaId()));//Disciplina.find(Long.valueOf(itemAtendimentoOferta.getDisciplinaSubstitutaId()),this.instituicaoEnsino);
							}

							int quantidade = itemAtendimentoOferta.getQuantidade();
							String turma = itemAtendimentoOferta.getTurma();
							
							HorarioAula menorHorarioAula = null;
							GrupoHorarioAula grupoHorarioAula = itemAtendimentoTatico.getHorariosAula();
							if (grupoHorarioAula != null && grupoHorarioAula.getHorarioAulaId() != null) {
								for (Integer horarioAulaId : grupoHorarioAula.getHorarioAulaId()) {
									HorarioAula ha = horarioAulaIdToHorarioAulaMap.get(Long.valueOf(horarioAulaId));//HorarioAula.find(Long.valueOf(itemAtendimentoHorarioAula.getHorarioAulaId()),this.instituicaoEnsino);
									if ((menorHorarioAula == null) || (menorHorarioAula.compareTo(ha) > 0)) {
										menorHorarioAula = ha;
									}
								}
							}

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
							atendimentoTatico.setHorarioAula(menorHorarioAula);
							atendimentoTatico.setConfirmada(false);
							
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
	public List<AtendimentoOperacional> generateAtendimentosOperacional(Set<Turno> turnosSelecionados) throws Exception {
		Map<Long,Sala> salaIdToSalaMap = new HashMap<Long,Sala>(); // [SalaId -> Sala]
		Map<Long,Oferta> ofertaIdToOfertaMap = new HashMap<Long,Oferta>(); // [OfertaId -> Oferta]
		Map<Long,Disciplina> disciplinaIdToDisciplinaMap = new HashMap<Long,Disciplina>(); // [DisciplinaId -> Disciplina]
		Map<Long,HorarioAula> horarioAulaIdToHorarioAulaMap = new HashMap<Long,HorarioAula>();// [HorarioAulaId -> Sala]
		Map<String,HorarioDisponivelCenario> horarioDisponivelCenarioIdToHorarioDisponivelCenarioMap = new HashMap<String,HorarioDisponivelCenario>();// [InstituicaoEnsinoId-HorarioAulaId-DiaSemanaId -> HorarioDisponivelCenario]
		preencheMaps(salaIdToSalaMap,ofertaIdToOfertaMap,disciplinaIdToDisciplinaMap,horarioAulaIdToHorarioAulaMap,horarioDisponivelCenarioIdToHorarioDisponivelCenarioMap);
		
		// [ProfessorId -> Professor]
		Map<Long,Professor> professorIdToProfessorMap = new HashMap<Long,Professor>();
		for (Professor professor : this.cenario.getProfessores()) {
			professorIdToProfessorMap.put(professor.getId(),professor);
		}		
		
		// [TitulacaoId -> Titulacao]
		Map<Long,Titulacao> titulacaoIdToTitulacaoMap = new HashMap<Long,Titulacao>();
		List<Titulacao> titulacoes = Titulacao.findAll(instituicaoEnsino);
		for (Titulacao titulacao : titulacoes) {
			titulacaoIdToTitulacaoMap.put(titulacao.getId(),titulacao);
		}
		
		// [TipoContratoId -> TipoContrato]
		Map<Long,TipoContrato> tipoContratoIdToTipoContratoMap = new HashMap<Long,TipoContrato>();
		List<TipoContrato> tiposContrato = TipoContrato.findAll(instituicaoEnsino);
		for (TipoContrato tipoContrato : tiposContrato) {
			tipoContratoIdToTipoContratoMap.put(tipoContrato.getId(),tipoContrato);
		}
		
		// [ProfessorVirtualId -> ProfessorVirtual]
		
		
		Map<Long,ProfessorVirtual> professorVirtualIdToProfessorVirtualMap = new HashMap<Long,ProfessorVirtual>();
		Map<Long, GrupoAlocacoes> professorVirtualIdMapEliminacoesMotivosUsoPar = new HashMap<Long, GrupoAlocacoes>();
		for (ItemProfessorVirtual itemProfessorVirtual : this.triedaOutput.getProfessoresVirtuais().getProfessorVirtual()) {
			Long id = Long.valueOf(-itemProfessorVirtual.getId());
			if (!professorVirtualIdToProfessorVirtualMap.containsKey(id)) {
				ProfessorVirtual pv = new ProfessorVirtual();
				
				pv.setCargaHorariaMax(itemProfessorVirtual.getChMax());
				pv.setCargaHorariaMin(itemProfessorVirtual.getChMin());
				pv.setInstituicaoEnsino(this.instituicaoEnsino);
				pv.setCenario(this.cenario);
				pv.setTipoContrato(tipoContratoIdToTipoContratoMap.get(Long.valueOf(itemProfessorVirtual.getContratoId())));
				pv.setTitulacao(titulacaoIdToTitulacaoMap.get(Long.valueOf(itemProfessorVirtual.getTitulacaoId())));// Titulacao.find(Integer.valueOf(itemProfessorVirtual.getTitulacaoId()).longValue(),this.instituicaoEnsino));
				for (Integer disciplinaId : itemProfessorVirtual.getDisciplinas().getId()) {
					Disciplina disciplina = disciplinaIdToDisciplinaMap.get(Long.valueOf(Math.abs(disciplinaId)));//Disciplina.find(Math.abs(disciplinaId.longValue()),this.instituicaoEnsino);
					if (disciplina != null) {
						pv.getDisciplinas().add(disciplina);
					}
				}
	
				pv.persist();
				
				professorVirtualIdMapEliminacoesMotivosUsoPar.put(id, itemProfessorVirtual.getAlocacoes());
				
				professorVirtualIdToProfessorVirtualMap.put(id,pv);
			}
		}
		
		// [AlunoDemandaId -> AlunoDemanda]
		Map<Long,AlunoDemanda> alunoDemandaIdToAlunoDemandaMap = createAlunoDemandasMap(turnosSelecionados);
		Set<String> aulaKey = new HashSet<String>();
		
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
									atendimentoOperacional.setConfirmada(false);
									
									Set<AlunoDemanda> listAlunoDemanda = atendimentoAlunoDemandaAssoc(itemAtendimentoOferta,alunosDemandaOperacional,atendimentoOperacional,alunoDemandaIdToAlunoDemandaMap);
									atendimentoOperacional.getAlunosDemanda().addAll(listAlunoDemanda);
									
									if (itemAtendimentoHorarioAula.isVirtual())
									{
										if (professorVirtualIdMapEliminacoesMotivosUsoPar.get(Long.valueOf(-itemAtendimentoHorarioAula.getProfessorId())) != null)
										{
											GrupoAlocacoes alocacoes = professorVirtualIdMapEliminacoesMotivosUsoPar.get(Long.valueOf(-itemAtendimentoHorarioAula.getProfessorId()));
											for (ItemAlocacao itemAlocacao : alocacoes.getAlocacao())
											{
												String alocacaoKey = turma + "-" + (disciplinaSubstituta == null ? disciplina.getId().intValue() : disciplinaSubstituta.getId().intValue())
														+ "-" + oferta.getCampus().getId().intValue() + "-" + !creditoTeorico;
												String itemAlocacaoKey = itemAlocacao.getTurma() + "-" + itemAlocacao.getDisciplinaId() + "-" + itemAlocacao.getCampusId() + "-"
														+ itemAlocacao.getPratica();
												
												if (alocacaoKey.equals(itemAlocacaoKey) && !aulaKey.contains(alocacaoKey))
												{
													for (ItemDicaEliminacao dicas : itemAlocacao.getDicasEliminacao().getDicaEliminacao())
													{
														DicaEliminacaoProfessorVirtual newDica = new DicaEliminacaoProfessorVirtual();
														Long profId = Long.valueOf(dicas.getProfRealId());
														newDica.setProfessor(professorIdToProfessorMap.get(profId));
														String dicaString = "";
														for (String dica : dicas.getAlteracoesNecessarias().getAlteracao())
														{
															dicaString += dica + "\n";
														}
														newDica.setDicaEliminacao(dicaString);
														atendimentoOperacional.getDicasEliminacaoProfessorVirtual().add(newDica);
													}
													
													for (ItemMotivoDeUso motivos : itemAlocacao.getMotivosDeUso().getMotivo())
													{
														MotivoUsoProfessorVirtual newMotivo = new MotivoUsoProfessorVirtual();
														Long profId = Long.valueOf(motivos.getProfRealId());
														newMotivo.setProfessor(professorIdToProfessorMap.get(profId));
														String motivoString = "";
														for (String motivo : motivos.getDescricoes().getDescricao())
														{
															motivoString += motivo + "\n";
														}
														newMotivo.setMotivoUso(motivoString);
														atendimentoOperacional.getMotivoUsoProfessorVirtual().add(newMotivo);
													}
													aulaKey.add(alocacaoKey);
												}
											}
										}
									}
									
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
	
	private void preencheMaps(Map<Long,Sala> salaIdToSalaMap, 
			                  Map<Long,Oferta> ofertaIdToOfertaMap,
			                  Map<Long,Disciplina> disciplinaIdToDisciplinaMap,
			                  Map<Long,HorarioAula> horarioAulaIdToHorarioAulaMap,
			                  Map<String,HorarioDisponivelCenario> horarioDisponivelCenarioIdToHorarioDisponivelCenarioMap) {
		// preenche salas e ofertas
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
		
		// preenche disciplinas
		for (Disciplina disciplina : this.cenario.getDisciplinas()) {
			disciplinaIdToDisciplinaMap.put(disciplina.getId(),disciplina);
		}
		
		// preenche horários
		for (Turno turno : this.cenario.getTurnos()) {
			for (HorarioAula horarioAula : turno.getHorariosAula()) {
				horarioAulaIdToHorarioAulaMap.put(horarioAula.getId(),horarioAula);
				for (HorarioDisponivelCenario hdc : horarioAula.getHorariosDisponiveisCenario()) {
					String key = this.instituicaoEnsino.getId() + "-" + horarioAula.getId() + "-" + hdc.getDiaSemana().ordinal();
					horarioDisponivelCenarioIdToHorarioDisponivelCenarioMap.put(key,hdc);
				}
			}
		}
	}

	private Map<Long, AlunoDemanda> createAlunoDemandasMap(Set<Turno> turnosSelecionados) {
		// [AlunoDemandaId -> AlunoDemanda]
		Map<Long,AlunoDemanda> alunoDemandaIdToAlunoDemandaMap = new HashMap<Long,AlunoDemanda>();
		try { AlunoDemanda.entityManager().flush();} catch (TransactionRequiredException e) {} // TODO: consertar
		List<AlunoDemanda> alunosDemanda = AlunoDemanda.findByCampusAndTurno(this.instituicaoEnsino,this.cenario.getCampi(),turnosSelecionados);
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
				throw new Exception("Não foi possível carregar o resultado gerado pelo resolvedor matemático. Abaixo seguem mais detalhes sobre o erro\n" +
						"O ID igual a: " + i + ", que deveria representar um Aluno Demanda, não foi encontrado na base de dados.\n" +
						"Id da disciplina da demanda:" + itemAtendimentoOferta.getDisciplinaId() + "\n" +
						"Id da oferta: " + itemAtendimentoOferta.getOfertaCursoCampiId());
			}
		}
		
		return listAlunoDemanda;
	}

	@Transactional
	public void salvarAtendimentosTatico(Set<Campus> campi, Set<Turno> turnos) {
		removerTodosAtendimentosTaticoJaSalvos(campi,turnos);
		removerTodosAtendimentosOperacionalJaSalvos(campi,turnos);

		boolean atualizouEntidade = false;
		for (AtendimentoTatico at : this.atendimentosTatico) {
			at.detach();
			at.persist();
			atualizouEntidade = true;
		}
		if (atualizouEntidade) {
			AlunoDemanda.entityManager().flush();
		}
		
		List<AlunoDemanda> alunosDemanda = AlunoDemanda.findByCampusAndTurnoFetchAtendimentoTatico(instituicaoEnsino, campi,turnos);
		
		for(AlunoDemanda alunoDemanda : alunosDemanda){
			if(alunosDemandaTatico.containsKey(alunoDemanda)){
				int creditosAtendidos = 0;
				Disciplina disciplinaSubstituta = null;
				alunoDemanda.getAtendimentosTatico().addAll(alunosDemandaTatico.get(alunoDemanda));
				for(AtendimentoTatico tatico : alunosDemandaTatico.get(alunoDemanda)){
					disciplinaSubstituta = tatico.getDisciplinaSubstituta();
					creditosAtendidos += tatico.getCreditosPratico() + tatico.getCreditosTeorico();
				}
				int totalCreditosDemanda = (disciplinaSubstituta != null) ? disciplinaSubstituta.getCreditosTotal() : alunoDemanda.getDemanda().getDisciplina().getCreditosTotal();
				alunoDemanda.setAtendido(totalCreditosDemanda <= creditosAtendidos);
			}
		}
	}

	@Transactional
	public void salvarAtendimentosOperacional(Set<Campus> campi, Set<Turno> turnos) {
		removerTodosAtendimentosTaticoJaSalvos(campi,turnos);
		removerTodosAtendimentosOperacionalJaSalvos(campi,turnos);

		boolean atualizouEntidade = false;
		for (AtendimentoOperacional at : this.atendimentosOperacional) {
			at.detach();
			at.persist();
			atualizouEntidade = true;
		}
		if (atualizouEntidade) {
			try {AtendimentoOperacional.entityManager().flush();} catch (TransactionRequiredException e) {} // TODO: consertar
		}
		
		List<AlunoDemanda> alunosDemanda = AlunoDemanda.findByCampusAndTurnoFetchAtendimentoOperacional(instituicaoEnsino, campi,turnos);
		
		//int countAtendidos = 0;
		//int countTotal = 0;
		for(AlunoDemanda alunoDemanda : alunosDemanda){
			if(alunosDemandaOperacional.containsKey(alunoDemanda)){
				int creditosAtendidos = 0;
				Disciplina disciplinaSubstituta = null;
				alunoDemanda.getAtendimentosOperacional().addAll(alunosDemandaOperacional.get(alunoDemanda));
				for(AtendimentoOperacional operacional : alunosDemandaOperacional.get(alunoDemanda)){
					disciplinaSubstituta = operacional.getDisciplinaSubstituta();
					creditosAtendidos += 1;
				}
				Disciplina disciplinaASerConsiderada = (disciplinaSubstituta != null) ? disciplinaSubstituta : alunoDemanda.getDemanda().getDisciplina();
				int totalCreditosDemanda = disciplinaASerConsiderada.getCreditosTotal();
				alunoDemanda.setAtendido(totalCreditosDemanda <= creditosAtendidos);				
//				if (creditosAtendidos > 0 && !alunoDemanda.getAtendido()) {
//					System.out.println("***Lixo no output = (cred_atend," + creditosAtendidos + ") (ald_id," + alunoDemanda.getId() + ") (aln_matricula," + alunoDemanda.getAluno().getMatricula() + ") (dis_atend," + disciplinaASerConsiderada.getCodigo() + ") (dis_dem," + alunoDemanda.getDemanda().getDisciplina().getCodigo() + ")");
//				} else if (!alunoDemanda.getAtendido()) {
//					System.out.println("***Lixo no output = (cred_atend," + creditosAtendidos + ") (ald_id," + alunoDemanda.getId() + ") (aln_matricula," + alunoDemanda.getAluno().getMatricula() + ") (dis_atend," + disciplinaASerConsiderada.getCodigo() + ") (dis_dem," + alunoDemanda.getDemanda().getDisciplina().getCodigo() + ")");
//				} else if (alunoDemanda.getAtendido()) {
//					System.out.println("OK = (cred_atend," + creditosAtendidos + ") (ald_id," + alunoDemanda.getId() + ") (aln_matricula," + alunoDemanda.getAluno().getMatricula() + ") (dis_atend," + disciplinaASerConsiderada.getCodigo() + ") (dis_dem," + alunoDemanda.getDemanda().getDisciplina().getCodigo() + ")");
//					countAtendidos++;
//				}
//				countTotal++;
//				System.out.println("Demanda Atendida=" + countAtendidos + " Total=" + countTotal);
			}
		}
	}

	@Transactional
	private void removerTodosAtendimentosTaticoJaSalvos(Set<Campus> campi, Set<Turno> turnos) {
		List<AtendimentoTatico> atendimentosTatico = AtendimentoTatico.findAllBy(instituicaoEnsino,campi,turnos);
		for (AtendimentoTatico at : atendimentosTatico) {
			at.remove();
		}
	}

	@Transactional
	private void removerTodosAtendimentosOperacionalJaSalvos(Set<Campus> campi, Set<Turno> turnos) {
		List<AtendimentoOperacional> atendimentosOperacional = AtendimentoOperacional.findAllBy(campi,turnos,instituicaoEnsino);
		Set<ProfessorVirtual> professoresVirtuaisASeremRemovidos = new HashSet<ProfessorVirtual>();
		for (AtendimentoOperacional at : atendimentosOperacional) {
			if (at.getProfessorVirtual() != null) {
				professoresVirtuaisASeremRemovidos.add(at.getProfessorVirtual());
			}
			at.remove();
		}
		
		for (ProfessorVirtual pv : professoresVirtuaisASeremRemovidos) {
			pv.remove();
		}
	}

	@Transactional
	public void atualizarAlunosDemanda(Set<Campus> campi, Set<Turno> turnos){
		List<AlunoDemanda> alunosDemandasBD = AlunoDemanda.findByCampusAndTurno(instituicaoEnsino,campi,turnos);
		for(AlunoDemanda alunoDemandaBD : alunosDemandasBD){
			if(alunoDemandaBD.getAtendido()){
				alunoDemandaBD.setAtendido(false);
				alunoDemandaBD.setMotivoNaoAtendimento(null);
			}
		}
	}
}
