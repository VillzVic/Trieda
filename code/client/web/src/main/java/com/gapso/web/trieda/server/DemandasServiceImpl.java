package com.gapso.web.trieda.server;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import org.springframework.transaction.annotation.Transactional;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Aluno;
import com.gapso.trieda.domain.AlunoDemanda;
import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.ParametroGeracaoDemanda;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.util.Atendimento;
import com.gapso.web.trieda.server.util.Atendimento.TipoCredito;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportFileWriter;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportListReader;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportListWriter;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportReader;
import com.gapso.web.trieda.server.util.progressReport.ProgressReportWriter;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDemandaDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.ParametroGeracaoDemandaDTO;
import com.gapso.web.trieda.shared.dtos.QuintetoDTO;
import com.gapso.web.trieda.shared.dtos.ResumoMatriculaDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.services.DemandasService;
import com.gapso.web.trieda.shared.util.view.TriedaException;

public class DemandasServiceImpl
	extends RemoteService implements DemandasService
{
	private static final long serialVersionUID = 5250776996542788849L;
	private ProgressReportWriter progressReport;
	
	public ParDTO<Map<Demanda,ParDTO<Integer,Map<Disciplina,Integer>>>,Integer> calculaQuantidadeDeNaoAtendimentosPorDemanda(Collection<Oferta> ofertas) {
		// preenche estruturas auxiliares
		List<Demanda> demandas = new ArrayList<Demanda>();
		Map<String,List<Atendimento>> ofertaIdDisciplinaIdToAtendimentosMap = new HashMap<String,List<Atendimento>>();
		for (Oferta oferta : ofertas) {
			demandas.addAll(oferta.getDemandas());
			// atendimentos táticos
			for (AtendimentoTatico atendimentoTatico : oferta.getAtendimentosTaticos()) {
				Atendimento atendimento = new Atendimento(atendimentoTatico);
				String key = oferta.getId() + "-" + atendimento.getDisciplina().getId();
				List<Atendimento> atendimentos = ofertaIdDisciplinaIdToAtendimentosMap.get(key);
				if (atendimentos == null) {
					atendimentos = new ArrayList<Atendimento>();
					ofertaIdDisciplinaIdToAtendimentosMap.put(key,atendimentos);
				}
				atendimentos.add(atendimento);
			}
			// atendimentos operacionais
			List<AtendimentoOperacional> todosAtendimentos = AtendimentoOperacional.getAtendimentosByOferta(getInstituicaoEnsinoUser(), oferta);
			for (AtendimentoOperacional atendimentoOperacional : todosAtendimentos) {
				Atendimento atendimento = new Atendimento(atendimentoOperacional);
				String key = oferta.getId() + "-" + atendimento.getDisciplina().getId();
				List<Atendimento> atendimentos = ofertaIdDisciplinaIdToAtendimentosMap.get(key);
				if (atendimentos == null) {
					atendimentos = new ArrayList<Atendimento>();
					ofertaIdDisciplinaIdToAtendimentosMap.put(key,atendimentos);
				}
				atendimentos.add(atendimento);
			}
		}
		
		// calcula map de demandas por quantidade de não atendimentos
		Integer qtdAlunosNaoAtendidosTotal = 0;
		Map<Demanda,ParDTO<Integer,Map<Disciplina,Integer>>> demandaToQtdAlunosNaoAtendidosMap = new HashMap<Demanda,ParDTO<Integer,Map<Disciplina,Integer>>>();
		for (Demanda demanda : demandas) {
			// obtém a lista de atendimentos relacionados com a demanda em questão
			String key = demanda.getOferta().getId() + "-" + demanda.getDisciplina().getId();
			List<Atendimento> atendimentosDaDemanda = ofertaIdDisciplinaIdToAtendimentosMap.get(key);
			if (atendimentosDaDemanda == null) {
				atendimentosDaDemanda = Collections.<Atendimento> emptyList();
			}
			
			Set<Long> alunosDemandasAtendidos = new HashSet<Long>();
			Map<Disciplina,Set<Long>> disciplinaSubstitutaToAlunosDemandasAtendidos = new HashMap<Disciplina, Set<Long>>();
			for (Atendimento atendimento : atendimentosDaDemanda) {
				// coleta pares aluno demanda atendidos
				Set<Long> alunosDemandasAtendidosLocal = new HashSet<Long>();
				for (AlunoDemanda alunoDemanda : atendimento.getAlunosDemandas()) {
					if (alunoDemanda.getAtendido() != null && alunoDemanda.getAtendido()) {
						alunosDemandasAtendidosLocal.add(alunoDemanda.getId());
					}
				}
				alunosDemandasAtendidos.addAll(alunosDemandasAtendidosLocal);
				
				// coleta disciplinas substitutas utilizadas
				if (atendimento.getDisciplinaSubstituta() != null) {
					Set<Long> alunosDemandasAtendidosDaSubstituta = disciplinaSubstitutaToAlunosDemandasAtendidos.get(atendimento.getDisciplinaSubstituta());
					if (alunosDemandasAtendidosDaSubstituta == null) {
						alunosDemandasAtendidosDaSubstituta = new HashSet<Long>();
						disciplinaSubstitutaToAlunosDemandasAtendidos.put(atendimento.getDisciplinaSubstituta(), alunosDemandasAtendidosDaSubstituta);
					}
					alunosDemandasAtendidosDaSubstituta.addAll(alunosDemandasAtendidosLocal);
				}
			}
			
			Map<Disciplina,Integer> disciplinaSubstitutaToQtdAlunosDemandasAtendidos = new HashMap<Disciplina, Integer>();
			for (Entry<Disciplina, Set<Long>> e : disciplinaSubstitutaToAlunosDemandasAtendidos.entrySet()) {
				disciplinaSubstitutaToQtdAlunosDemandasAtendidos.put(e.getKey(), e.getValue().size());
			}
			
			int qtdAlunosNaoAtendidosDemanda = demanda.getQuantidade() - alunosDemandasAtendidos.size();
			demandaToQtdAlunosNaoAtendidosMap.put(demanda,ParDTO.create(qtdAlunosNaoAtendidosDemanda,disciplinaSubstitutaToQtdAlunosDemandasAtendidos));
			qtdAlunosNaoAtendidosTotal += qtdAlunosNaoAtendidosDemanda;
		}
		
		return ParDTO.create(demandaToQtdAlunosNaoAtendidosMap,qtdAlunosNaoAtendidosTotal);
	}

	public ParDTO<Map<Demanda,ParDTO<Integer,Disciplina>>,Integer> calculaQuantidadeDeNaoAtendimentosPorDemanda_(Collection<Oferta> ofertas) {
		// preenche estruturas auxiliares
		List<Demanda> demandas = new ArrayList<Demanda>();
		Map<String,List<Atendimento>> ofertaIdDisciplinaIdToAtendimentosMap = new HashMap<String,List<Atendimento>>();
		for (Oferta oferta : ofertas) {
			demandas.addAll(oferta.getDemandas());
			// atendimentos táticos
			for (AtendimentoTatico atendimentoTatico : oferta.getAtendimentosTaticos()) {
				Atendimento atendimento = new Atendimento(atendimentoTatico);
				String key = oferta.getId() + "-" + atendimento.getDisciplina().getId();
				List<Atendimento> atendimentos = ofertaIdDisciplinaIdToAtendimentosMap.get(key);
				if (atendimentos == null) {
					atendimentos = new ArrayList<Atendimento>();
					ofertaIdDisciplinaIdToAtendimentosMap.put(key,atendimentos);
				}
				atendimentos.add(atendimento);
			}
			// atendimentos operacionais
			List<AtendimentoOperacional> todosAtendimentos = AtendimentoOperacional.getAtendimentosByOferta(getInstituicaoEnsinoUser(), oferta);
			for (AtendimentoOperacional atendimentoOperacional : todosAtendimentos) {
				Atendimento atendimento = new Atendimento(atendimentoOperacional);
				String key = oferta.getId() + "-" + atendimento.getDisciplina().getId();
				List<Atendimento> atendimentos = ofertaIdDisciplinaIdToAtendimentosMap.get(key);
				if (atendimentos == null) {
					atendimentos = new ArrayList<Atendimento>();
					ofertaIdDisciplinaIdToAtendimentosMap.put(key,atendimentos);
				}
				atendimentos.add(atendimento);
			}
		}
		
		// calcula map de demandas por quantidade de não atendimentos
		Integer qtdAlunosNaoAtendidosTotal = 0;
		Map<Demanda,ParDTO<Integer,Disciplina>> demandaToQtdAlunosNaoAtendidosMap = new HashMap<Demanda,ParDTO<Integer,Disciplina>>();
		for (Demanda demanda : demandas) {
			// obtém a lista de atendimentos relacionados com a demanda em questão
			String key = demanda.getOferta().getId() + "-" + demanda.getDisciplina().getId();
			List<Atendimento> atendimentosDaDemanda = ofertaIdDisciplinaIdToAtendimentosMap.get(key);
			if (atendimentosDaDemanda == null) {
				atendimentosDaDemanda = Collections.<Atendimento> emptyList();
			} 
			
			// calcula 
			Disciplina disciplinaASerConsiderada = demanda.getDisciplina();
			// [TipoCrédito -> [Turma -> List<ParDTO<QtdAlunosAtendidos,QtdCréditos>>]]
			Map<TipoCredito,Map<String,List<ParDTO<Integer,Integer>>>> tipoCredToTurmaToPares_QtdAtendidos_CreditosMap = new HashMap<TipoCredito,Map<String,List<ParDTO<Integer,Integer>>>>();
			Set<Disciplina> disciplinasSubstitutas = new HashSet<Disciplina>();
			for (Atendimento atendimento : atendimentosDaDemanda) {
				if (atendimento.getDisciplinaSubstituta() != null) {
					// NESTE CONJUNTO DEVE HAVER, NO MÁXIMO, APENAS UMA DISCIPLINA, ISTO É, ESPERA-SE QUE O CONJUNTO TENHA TAMANHOS ZERO OU UM
					disciplinasSubstitutas.add(atendimento.getDisciplinaSubstituta());
					disciplinaASerConsiderada = atendimento.getDisciplinaSubstituta();
				}
				
				boolean temCreditosTeoricosEPraticos = (disciplinaASerConsiderada.getCreditosTeorico() > 0 && disciplinaASerConsiderada.getCreditosPratico() > 0);
				boolean consideraTotalCreditosDisciplina = temCreditosTeoricosEPraticos && !disciplinaASerConsiderada.getLaboratorio();
				
				Map<String,List<ParDTO<Integer,Integer>>> turmaToPares_QtdAtendidos_CreditosMap = tipoCredToTurmaToPares_QtdAtendidos_CreditosMap.get(atendimento.getTipoCredito());
				if (turmaToPares_QtdAtendidos_CreditosMap == null) {
					turmaToPares_QtdAtendidos_CreditosMap = new HashMap<String,List<ParDTO<Integer,Integer>>>();
					tipoCredToTurmaToPares_QtdAtendidos_CreditosMap.put(atendimento.getTipoCredito(),turmaToPares_QtdAtendidos_CreditosMap);
				}
				
				List<ParDTO<Integer,Integer>> pares_QtdAtendidos_Creditos = turmaToPares_QtdAtendidos_CreditosMap.get(atendimento.getTurma());
				if (pares_QtdAtendidos_Creditos == null) {
					pares_QtdAtendidos_Creditos = new ArrayList<ParDTO<Integer,Integer>>();
					turmaToPares_QtdAtendidos_CreditosMap.put(atendimento.getTurma(),pares_QtdAtendidos_Creditos);
				}
				
				if (pares_QtdAtendidos_Creditos.isEmpty()) {
					pares_QtdAtendidos_Creditos.add(ParDTO.create(atendimento.getQuantidadeAlunos(),atendimento.getTotalCreditos()));
				} else {
					// garante que a estrutura de alunos atendidos por créditos esteja ordenada pela qtd de créditos
					Collections.sort(pares_QtdAtendidos_Creditos,new Comparator<ParDTO<Integer,Integer>>() {
						@Override
						public int compare(ParDTO<Integer,Integer> o1, ParDTO<Integer,Integer> o2) {
							Integer totalCreditos1 = o1.getSegundo();
							Integer totalCreditos2 = o2.getSegundo();
							int ret = -totalCreditos1.compareTo(totalCreditos2);
							if (ret == 0) {
								Integer qtdAlunos1 = o1.getPrimeiro();
								Integer qtdAlunos2 = o2.getPrimeiro();
								ret = qtdAlunos1.compareTo(qtdAlunos2);
							}
							return ret;
						}
					});						
					
					// atualiza estrutura de alunos atendidos por créditos
					ParDTO<Integer,Integer> parDoAtendimento = ParDTO.create(atendimento.getQuantidadeAlunos(),atendimento.getTotalCreditos());
					for (ParDTO<Integer,Integer> parDaLista : pares_QtdAtendidos_Creditos) {
						Integer parDaListaQtdAlunos = parDaLista.getPrimeiro();
						Integer parDaListaTotalCreditos = parDaLista.getSegundo();
						Integer parDoAtendimentoQtdAlunos = parDoAtendimento.getPrimeiro();
						Integer parDoAtendimentoTotalCreditos = parDoAtendimento.getSegundo();
						// verifica se o par (QtdAlunos,TotalCreditos) já está fechado ou ainda há chance do mesmo ser atualizado
						//    - um par (QtdAlunos,TotalCreditos) é considerado fechado caso o total de créditos do par seja igual ao total de créditos da demanda
					    //    - um par (QtdAlunos,TotalCreditos) tem chance de ser atualizado caso o total de créditos do par seja menor que o total de créditos da demanda
						int disciplinaTotalCreditosPorTipo = consideraTotalCreditosDisciplina ? (disciplinaASerConsiderada.getTotalCreditos()) : (TipoCredito.TEORICO.equals(atendimento.getTipoCredito()) ? disciplinaASerConsiderada.getCreditosTeorico() : disciplinaASerConsiderada.getCreditosPratico());
						if (parDaListaTotalCreditos < disciplinaTotalCreditosPorTipo) {
							if (parDaListaQtdAlunos > parDoAtendimentoQtdAlunos) {
								parDaLista.setPrimeiro(parDoAtendimentoQtdAlunos);
								parDaLista.setSegundo(parDaListaTotalCreditos+parDoAtendimentoTotalCreditos);
								
								parDoAtendimento.setPrimeiro(parDaListaQtdAlunos-parDoAtendimentoQtdAlunos);
								parDoAtendimento.setSegundo(parDaListaTotalCreditos);
								break;
							} else {
								parDaLista.setSegundo(parDaListaTotalCreditos+parDoAtendimentoTotalCreditos);
								
								parDoAtendimento.setPrimeiro(parDoAtendimentoQtdAlunos-parDaListaQtdAlunos);
								if (parDoAtendimento.getPrimeiro() == 0) {
									break;
								}
							}
						}
					}

					if (parDoAtendimento.getPrimeiro() > 0) {
						pares_QtdAtendidos_Creditos.add(parDoAtendimento);
					}
				}
			}
			
			// verifica se há disciplina substituta nos atendimentos
			Disciplina disciplinaSubstituta = null;
			if (!disciplinasSubstitutas.isEmpty()) {
				disciplinaSubstituta = disciplinasSubstitutas.iterator().next();
				if (disciplinasSubstitutas.size() > 1) {
					String disciplinasStr = "";
					for (Disciplina disciplina : disciplinasSubstitutas) {
						disciplinasStr += disciplina.getCodigo() + ", ";
					}
					disciplinasStr = disciplinasStr.substring(0,disciplinasStr.length()-2);
					System.out.println("*** ERRO NO SOLVER: na demanda [" + demanda.getNaturalKeyString() + "] a disciplina [" + demanda.getDisciplina().getCodigo() + "] foi substituida por mais de uma disciplina, no caso [" + disciplinasStr + "]");
				}
			}
			
			// calcula quantidade de alunos não atendidos
			int qtdAlunosNaoAtendidosDemanda = 0;
			if (atendimentosDaDemanda.isEmpty()) {
				qtdAlunosNaoAtendidosDemanda = demanda.getQuantidade();
			} else {
				boolean temCreditosTeoricosEPraticos = (disciplinaASerConsiderada.getCreditosTeorico() > 0 && disciplinaASerConsiderada.getCreditosPratico() > 0);
				boolean consideraTotalCreditosDisciplina = temCreditosTeoricosEPraticos && !disciplinaASerConsiderada.getLaboratorio();
				int qtdAlunosAtendidosDemandaPorTipoCredito[] = {0,0};
				// para cada tipo de crédito
				for (Entry<TipoCredito,Map<String,List<ParDTO<Integer,Integer>>>> e : tipoCredToTurmaToPares_QtdAtendidos_CreditosMap.entrySet()) {
					TipoCredito tipoCredito = e.getKey();
					Map<String,List<ParDTO<Integer,Integer>>> turmaToPares_QtdAtendidos_CreditosMap = e.getValue();
					
					// insere em uma mesma lista informações de atendimentos de mesma qtde de créditos
					Map<Integer,List<ParDTO<Integer,Integer>>> qtdCreditosToPares_QtdAtendidos_CreditosMap = new HashMap<Integer,List<ParDTO<Integer,Integer>>>();
					for (Entry<String,List<ParDTO<Integer,Integer>>> entry : turmaToPares_QtdAtendidos_CreditosMap.entrySet()) {
						for (ParDTO<Integer,Integer> par : entry.getValue()) {
							int parTotalCreditos = par.getSegundo();
							
							List<ParDTO<Integer,Integer>> paresDeMesmaQtdCreditos = qtdCreditosToPares_QtdAtendidos_CreditosMap.get(parTotalCreditos);
							if (paresDeMesmaQtdCreditos == null) {
								paresDeMesmaQtdCreditos = new ArrayList<ParDTO<Integer,Integer>>();
								qtdCreditosToPares_QtdAtendidos_CreditosMap.put(parTotalCreditos,paresDeMesmaQtdCreditos);
							}
							paresDeMesmaQtdCreditos.add(par);
						}
					}
					
					// soma qtde de alunos atendidos de mesma qtde de créditos
					List<ParDTO<Integer,Integer>> pares_QtdAtendidos_Creditos = new ArrayList<ParDTO<Integer,Integer>>();
					for (Entry<Integer,List<ParDTO<Integer,Integer>>> entry : qtdCreditosToPares_QtdAtendidos_CreditosMap.entrySet()) {
						int totalCreditos = entry.getKey();
						int totalAlunosAtendidos = 0;
						for (ParDTO<Integer,Integer> par : entry.getValue()) {
							int parQtdAlunos = par.getPrimeiro();
							totalAlunosAtendidos += parQtdAlunos;
						}
						pares_QtdAtendidos_Creditos.add(ParDTO.create(totalAlunosAtendidos,totalCreditos));
					}
					
					int disciplinaTotalCreditosPorTipo = consideraTotalCreditosDisciplina ? (disciplinaASerConsiderada.getTotalCreditos()) : (TipoCredito.TEORICO.equals(tipoCredito) ? disciplinaASerConsiderada.getCreditosTeorico() : disciplinaASerConsiderada.getCreditosPratico());
					int qtdAlunosAtendidosDemanda = 0;
					for (ParDTO<Integer,Integer> par : pares_QtdAtendidos_Creditos) {
						int parQtdAlunos = par.getPrimeiro();
						int parTotalCreditos = par.getSegundo();
						// considera como atendidos somente os alunos que cursaram a quantidade total de créditos da demanda
						if (parTotalCreditos == disciplinaTotalCreditosPorTipo) {
							qtdAlunosAtendidosDemanda += parQtdAlunos;
						}
					}
					
					qtdAlunosAtendidosDemandaPorTipoCredito[tipoCredito.ordinal()] = qtdAlunosAtendidosDemanda;
				}
				
				int qtdAlunosAtendidosDemanda = 0;
				if (temCreditosTeoricosEPraticos) {
					// a disciplina da demanda tem créditos teóricos e práticos
					if (disciplinaASerConsiderada.getLaboratorio()) {
						// disciplina exige laboratório para créditos práticos
						qtdAlunosAtendidosDemanda = Math.min(
							qtdAlunosAtendidosDemandaPorTipoCredito[TipoCredito.TEORICO.ordinal()],
							qtdAlunosAtendidosDemandaPorTipoCredito[TipoCredito.PRATICO.ordinal()]
						);
					} else {
						// disciplina não exige laboratório para créditos práticos, neste caso, o solver gera a solução
						// como se todos os créditos fossem teóricos
						qtdAlunosAtendidosDemanda = qtdAlunosAtendidosDemandaPorTipoCredito[TipoCredito.TEORICO.ordinal()];
					}
				} else {
					if (disciplinaASerConsiderada.getCreditosTeorico() > 0) {
						qtdAlunosAtendidosDemanda = qtdAlunosAtendidosDemandaPorTipoCredito[TipoCredito.TEORICO.ordinal()];
					} else {
						qtdAlunosAtendidosDemanda = qtdAlunosAtendidosDemandaPorTipoCredito[TipoCredito.PRATICO.ordinal()];
					}
				}
				
				qtdAlunosNaoAtendidosDemanda = demanda.getQuantidade() - qtdAlunosAtendidosDemanda;
			}
			
			demandaToQtdAlunosNaoAtendidosMap.put(demanda,ParDTO.create(qtdAlunosNaoAtendidosDemanda,disciplinaSubstituta));
			qtdAlunosNaoAtendidosTotal += qtdAlunosNaoAtendidosDemanda;
		}
		
		return ParDTO.create(demandaToQtdAlunosNaoAtendidosMap,qtdAlunosNaoAtendidosTotal);
	}
	
	public Map<Demanda,Map<AtendimentoTatico,List<Aluno>>> alocaAlunosNosAtendimentos(Collection<Oferta> ofertas) {
		Map<Demanda,Map<AtendimentoTatico,List<Aluno>>> demandaToAlunosPorAtendimentosMap = new HashMap<Demanda,Map<AtendimentoTatico,List<Aluno>>>();
		
		//
		Map<Long,List<AlunoDemanda>> demandaIdToAlunosDemandasMap = new HashMap<Long,List<AlunoDemanda>>();
		List<AlunoDemanda> alunosDemandas = AlunoDemanda.findAll(getInstituicaoEnsinoUser());
		if (alunosDemandas != null && !alunosDemandas.isEmpty()) {
			for (AlunoDemanda alunoDemanda : alunosDemandas) {
				List<AlunoDemanda> listaAlunosDemandas = demandaIdToAlunosDemandasMap.get(alunoDemanda.getDemanda().getId());
				if (listaAlunosDemandas == null) {
					listaAlunosDemandas = new ArrayList<AlunoDemanda>();
					demandaIdToAlunosDemandasMap.put(alunoDemanda.getDemanda().getId(),listaAlunosDemandas);
				}
				listaAlunosDemandas.add(alunoDemanda);
			}
			
			// Preenche estruturas auxiliares
			List<Demanda> demandas = new ArrayList<Demanda>();
			List<AtendimentoTatico> atendimentosTaticos = new ArrayList<AtendimentoTatico>();
			Map<String,List<AtendimentoTatico>> ofertaIdDisciplinaIdToAtendimentosTaticoMap = new HashMap<String,List<AtendimentoTatico>>();
			for (Oferta oferta : ofertas) {
				demandas.addAll(oferta.getDemandas());
				atendimentosTaticos.addAll(oferta.getAtendimentosTaticos());
				for (AtendimentoTatico atendimento : oferta.getAtendimentosTaticos()) {
					String key = oferta.getId() + "-" + atendimento.getDisciplina().getId();
					List<AtendimentoTatico> atendimentos = ofertaIdDisciplinaIdToAtendimentosTaticoMap.get(key);
					if (atendimentos == null) {
						atendimentos = new ArrayList<AtendimentoTatico>();
						ofertaIdDisciplinaIdToAtendimentosTaticoMap.put(key,atendimentos);
					}
					atendimentos.add(atendimento);
				}
			}
	
			// Calcula map de demandas por quantidade de não atendimentos
			for (Demanda demanda : demandas) {
				// obtém a lista de atendimentos relacionados com a demanda em questão
				String key = demanda.getOferta().getId() + "-" + demanda.getDisciplina().getId();
				List<AtendimentoTatico> atendimentosDaDemanda = ofertaIdDisciplinaIdToAtendimentosTaticoMap.get(key);
				if (atendimentosDaDemanda == null) {
					atendimentosDaDemanda = Collections.<AtendimentoTatico> emptyList();
				}
				
				// para cada atendimento, calcula o total a ser descontado da demanda calculada em termos de créditos
				int totalASerDescontadoDaDemandaAlunosCreditosT = 0;
				int totalASerDescontadoDaDemandaAlunosCreditosP = 0;
				int totalASerDescontadoDaDemandaAlunosCreditosTotal = 0;
				Set<Disciplina> disciplinasSubstitutas = new HashSet<Disciplina>();
				Disciplina disciplinaSubstituta = null;
				for (AtendimentoTatico atendimento : atendimentosDaDemanda) {
					totalASerDescontadoDaDemandaAlunosCreditosT += (atendimento.getCreditosTeorico() * atendimento.getQuantidadeAlunos());
					totalASerDescontadoDaDemandaAlunosCreditosP += (atendimento.getCreditosPratico() * atendimento.getQuantidadeAlunos());
					totalASerDescontadoDaDemandaAlunosCreditosTotal += (atendimento.getTotalCreditos() * atendimento.getQuantidadeAlunos());
					// NESTA LISTA DEVE HAVER, NO MÁXIMO, APENAS UMA DISCIPLINA, ISTO É, ESPERA-SE QUE A LISTA TENHA TAMANHOS ZERO OU UM
					if (atendimento.getDisciplinaSubstituta() != null) {
						disciplinasSubstitutas.add(atendimento.getDisciplinaSubstituta());
					}
				}
				
				if (!disciplinasSubstitutas.isEmpty()) {
					disciplinaSubstituta = disciplinasSubstitutas.iterator().next();
					if (disciplinasSubstitutas.size() > 1) {
						String disciplinasStr = "";
						for (Disciplina disciplina : disciplinasSubstitutas) {
							disciplinasStr += disciplina.getCodigo() + ", ";
						}
						disciplinasStr = disciplinasStr.substring(0,disciplinasStr.length()-2);
						System.out.println("*** ERRO NO SOLVER: na demanda [" + demanda.getNaturalKeyString() + "] a disciplina [" + demanda.getDisciplina().getCodigo() + "] foi substituida por mais de uma disciplina, no caso [" + disciplinasStr + "]");
					}
				}
				Disciplina disciplinaASerConsiderada = (disciplinaSubstituta != null) ? disciplinaSubstituta : demanda.getDisciplina();
	
				// calcula a demanda em termos de créditos teóricos, práticos e total
				int demandaAlunosCreditosT = (disciplinaASerConsiderada.getCreditosTeorico() * demanda.getQuantidade());
				int demandaAlunosCreditosP = (disciplinaASerConsiderada.getCreditosPratico() * demanda.getQuantidade());
				int demandaAlunosCreditosTotal = (demandaAlunosCreditosT + demandaAlunosCreditosP);
	
				// para cada atendimento, desconta o total atendimento da demanda calculada em termos de créditos 
				demandaAlunosCreditosT -= totalASerDescontadoDaDemandaAlunosCreditosT;
				demandaAlunosCreditosP -= totalASerDescontadoDaDemandaAlunosCreditosP;
				demandaAlunosCreditosTotal -= totalASerDescontadoDaDemandaAlunosCreditosTotal;
	
				// calcula a quantidade de alunos não atendidos
				int qtdAlunosNaoAtendidosDemanda = 0;
				if (atendimentosDaDemanda.isEmpty()) {
					qtdAlunosNaoAtendidosDemanda = demanda.getQuantidade();
				} else {
					// calcula a quantidade de alunos não atendidos de acordo com as situações específicas
					
					// verifica se a demanda possui créditos práticos não atendidos, créditos estes que não são exigidos em laboratório
					if ((demandaAlunosCreditosP > 0) && !disciplinaASerConsiderada.getLaboratorio()) {
						if (demandaAlunosCreditosTotal > 0) {
							qtdAlunosNaoAtendidosDemanda = demandaAlunosCreditosTotal / disciplinaASerConsiderada.getTotalCreditos();
						}
					} else {
						// verifica se a demanda possui créditos teóricos e práticos não atendidos
						if ((demandaAlunosCreditosT > 0) && (demandaAlunosCreditosP > 0)) {
							if (demandaAlunosCreditosT > demandaAlunosCreditosP) {
								qtdAlunosNaoAtendidosDemanda = demandaAlunosCreditosT / disciplinaASerConsiderada.getCreditosTeorico();
							} else {
								qtdAlunosNaoAtendidosDemanda = demandaAlunosCreditosP / disciplinaASerConsiderada.getCreditosPratico();
							}
						} else {
							// verifica se a demanda possui créditos teóricos não atendidos
							if (demandaAlunosCreditosT > 0) {
								qtdAlunosNaoAtendidosDemanda = demandaAlunosCreditosT / disciplinaASerConsiderada.getCreditosTeorico();
							}
							
							// verifica se a demanda possui créditos práticos não atendidos
							if (demandaAlunosCreditosP > 0) {
								qtdAlunosNaoAtendidosDemanda = demandaAlunosCreditosP / disciplinaASerConsiderada.getCreditosPratico();
							}
						}
					}
				}
				
				int qtdeAlunosAtendidosDemanda = demanda.getQuantidade() - qtdAlunosNaoAtendidosDemanda;
				if (qtdeAlunosAtendidosDemanda > 0) {
					List<AlunoDemanda> alunosDaDemanda = demandaIdToAlunosDemandasMap.get(demanda.getId());
					// ordena os alunos pela matricula para garantir que a alocação seja determinística
					Collections.<AlunoDemanda>sort(alunosDaDemanda,new Comparator<AlunoDemanda>() {
						@Override
						public int compare(AlunoDemanda o1, AlunoDemanda o2) {
							return o1.getAluno().getMatricula().compareTo(o2.getAluno().getMatricula());
						}
					});
					
					// seleciona o N primeiros alunos da demanda onde N igual a quantidade de alunos atendidos
					List<AlunoDemanda> alunosAtendidosDaDemanda = new ArrayList<AlunoDemanda>();
					for (int i = 0; i < qtdeAlunosAtendidosDemanda; i++) {
						alunosAtendidosDaDemanda.add(alunosDaDemanda.get(i));
					}
					
					// organiza os atendimentos por dia da semana e por turma e tipo de crédito
					// [DiaSemana -> [Turma-TipoCredito -> List<Atendimentos>]]
					Map<Semanas,Map<String,List<AtendimentoTatico>>> diaDaSemanaToAtendimentosMap = new HashMap<Semanas,Map<String,List<AtendimentoTatico>>>();
					for (AtendimentoTatico atendimento : atendimentosDaDemanda) {
						// [Turma-TipoCredito -> List<Atendimentos>]
						Map<String,List<AtendimentoTatico>> turmaTipoCreditoToAtendimentosMap = diaDaSemanaToAtendimentosMap.get(atendimento.getSemana());
						if (turmaTipoCreditoToAtendimentosMap == null) {
							turmaTipoCreditoToAtendimentosMap = new HashMap<String,List<AtendimentoTatico>>();
							diaDaSemanaToAtendimentosMap.put(atendimento.getSemana(),turmaTipoCreditoToAtendimentosMap);
						}
						
						// os atendimentos da mesma turma e mesmo tipo de crédito no mesmo dia são os que tem os alunos diferentes
						String turmaTipoCreditoStr = atendimento.getTurma() + "-" + ((atendimento.getCreditosPratico() > 0) ? "P" : "T");
						List<AtendimentoTatico> atendimentos = turmaTipoCreditoToAtendimentosMap.get(turmaTipoCreditoStr);
						if (atendimentos == null) {
							atendimentos = new ArrayList<AtendimentoTatico>();
							turmaTipoCreditoToAtendimentosMap.put(turmaTipoCreditoStr,atendimentos);
						}
						atendimentos.add(atendimento);
					}
					
					// associa os alunos aos atendimentos
					Map<AtendimentoTatico,List<Aluno>> atendimentoToAlunosMap = new HashMap<AtendimentoTatico,List<Aluno>>();
					demandaToAlunosPorAtendimentosMap.put(demanda,atendimentoToAlunosMap);
					for (Entry<Semanas,Map<String,List<AtendimentoTatico>>> entryDiaSemana : diaDaSemanaToAtendimentosMap.entrySet()) {
						for (Entry<String,List<AtendimentoTatico>> entryTurmaTipoCredito : entryDiaSemana.getValue().entrySet()) {
							int indiceInicial = 0;
							for (AtendimentoTatico atendimento : entryTurmaTipoCredito.getValue()) {
								List<Aluno> alunos = atendimentoToAlunosMap.get(atendimento);
								if (alunos == null) {
									alunos = new ArrayList<Aluno>();
									atendimentoToAlunosMap.put(atendimento,alunos);
								}
								// associa os alunos
								for (int i = 0; i < atendimento.getQuantidadeAlunos(); i++) {
									//AlunoDemanda alunoDemanda = alunosAtendidosDaDemanda.get(indiceInicial+i);
									AlunoDemanda alunoDemanda = alunosDaDemanda.get(indiceInicial+i);
									alunos.add(alunoDemanda.getAluno());
								}
								indiceInicial += atendimento.getQuantidadeAlunos();
							}
						}
					}
				}
			}
		}
		
		return demandaToAlunosPorAtendimentosMap;
	}

	@Override
	public PagingLoadResult< DemandaDTO > getBuscaList( CenarioDTO cenarioDTO,
		CampusDTO campusDTO, CursoDTO cursoDTO, CurriculoDTO curriculoDTO,
		TurnoDTO turnoDTO, DisciplinaDTO disciplinaDTO, PagingLoadConfig config )
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		List< DemandaDTO > list = new ArrayList< DemandaDTO >();
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

		Campus campus = null;
		if ( campusDTO != null )
		{
			campus = ConvertBeans.toCampus( campusDTO );
		}
		
		Curso curso = null;
		if ( cursoDTO != null )
		{
			curso = ConvertBeans.toCurso( cursoDTO );
		}

		Curriculo curriculo	= null;
		if ( curriculoDTO != null )
		{
			curriculo = ConvertBeans.toCurriculo( curriculoDTO );
		}

		Turno turno = null;
		if ( turnoDTO != null )
		{
			turno = ConvertBeans.toTurno( turnoDTO );
		}

		Disciplina disciplina = null;
		if ( disciplinaDTO != null )
		{
			disciplina = ConvertBeans.toDisciplina( disciplinaDTO );
		}

		List< Demanda > listDomains = Demanda.findBy( getInstituicaoEnsinoUser(),
			cenario, campus, curso, curriculo, turno, disciplina,
			config.getOffset(), config.getLimit(), orderBy );

		for ( Demanda demanda : listDomains )
		{
			list.add( ConvertBeans.toDemandaDTO( demanda ) );
		}

		BasePagingLoadResult< DemandaDTO > result
			= new BasePagingLoadResult< DemandaDTO >( list );

		result.setOffset( config.getOffset() );
		result.setTotalLength( Demanda.count( getInstituicaoEnsinoUser(),
			cenario, campus, curso, curriculo, turno, disciplina ) );

		return result;
	}

	@Override
	@Transactional
	public void save( DemandaDTO demandaDTO, List<DisciplinaDemandaDTO> disciplinas, Integer periodo) throws TriedaException
	{
		try {
			Oferta oferta = Oferta.find(demandaDTO.getOfertaId(), getInstituicaoEnsinoUser());
			Collections.sort(disciplinas, new Comparator<DisciplinaDemandaDTO>() {
				@Override
				public int compare(DisciplinaDemandaDTO o1, DisciplinaDemandaDTO o2) {
					return o1.getNovaDemanda().compareTo(o2.getNovaDemanda());
				}
			});
			
			List<Aluno> alunos = new ArrayList<Aluno>();
			for (DisciplinaDemandaDTO disciplinaDemanda : disciplinas)
			{
				if (disciplinaDemanda.getNovaDemanda() > 0)
				{
					Disciplina disciplina = Disciplina.find(disciplinaDemanda.getDisciplinaId(), getInstituicaoEnsinoUser());
					Demanda demanda = Demanda.findbyOfertaAndDisciplina(getInstituicaoEnsinoUser(), oferta, disciplina);
					if(demanda == null)
					{
						demanda = new Demanda();
						demanda.setDisciplina(disciplina);
						demanda.setOferta(oferta);
						demanda.persist();
					}
					
					int alunosCriados = alunos.size();
					for (int i = alunosCriados; i < disciplinaDemanda.getNovaDemanda(); i++)
					{
						Aluno novoAluno = new Aluno();
						novoAluno.setCenario(getCenario());
						novoAluno.setInstituicaoEnsino(getInstituicaoEnsinoUser());
						novoAluno.setNome("Aluno Virtual " + i);
						novoAluno.setMatricula("Aluno Virtual " + i);
						novoAluno.setFormando(false);
						novoAluno.setVirtual(true);
						novoAluno.setCriadoTrieda(true);
						
						novoAluno.persist();
						
						alunos.add(novoAluno);
					}
	
					for (int i = 0; i < disciplinaDemanda.getNovaDemanda(); i++)
					{
						AlunoDemanda novoAlunoDemanda = new AlunoDemanda();
						novoAlunoDemanda.setAluno(alunos.get(i));
						novoAlunoDemanda.setDemanda(demanda);
						novoAlunoDemanda.setPeriodo(periodo);
						novoAlunoDemanda.setPrioridade(1);
						novoAlunoDemanda.setAtendido(false);
						
						novoAlunoDemanda.persist();
					}
				}
			}
			for (Aluno aluno : alunos)
			{
				aluno.setNome("Aluno Virtual " + aluno.getId() + " " + oferta.getCurriculo().getCurso().getNome() + " " 
						+ oferta.getCurriculo().getCodigo() + " " + periodo );
				aluno.setMatricula( aluno.getId().toString() );
				aluno.merge();
			}
		} catch (Exception e) {
			e.printStackTrace();
			throw new TriedaException(e);
		}
	}

	@Override
	public void remove( List< DemandaDTO > demandaDTOList )
	{
		for ( DemandaDTO demandaDTO : demandaDTOList )
		{
			Demanda demanda = ConvertBeans.toDemanda( demandaDTO );

			if ( demanda != null )
			{
				demanda.remove();
			}
		}
	}
	
	@Override
	public Integer findPeriodo(DemandaDTO demandaDTO) throws TriedaException {
		Integer periodo = null;
		
		try {
			InstituicaoEnsino instituicaoEnsino = InstituicaoEnsino.find(demandaDTO.getInstituicaoEnsinoId());
			Curriculo curriculo = Curriculo.find(demandaDTO.getCurriculoId(),instituicaoEnsino);
			Disciplina disciplina = Disciplina.find(demandaDTO.getDisciplinaId(),instituicaoEnsino);
			
			periodo = curriculo.getPeriodo(disciplina);
		} catch (Exception e) {
			e.printStackTrace();
			throw new TriedaException(e);
		}
		
		return periodo; 
	}
	
	@Override
	public ParametroGeracaoDemandaDTO getParametroGeracaoDemanda( CenarioDTO cenarioDTO )
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		List< Campus > listCampus = Campus.findByCenario( getInstituicaoEnsinoUser(), cenario );
		List< Turno > listTurnos = Turno.findAll( getInstituicaoEnsinoUser(), cenario );
		
		ParametroGeracaoDemanda parametroGeracaoDemanda = new ParametroGeracaoDemanda();
		parametroGeracaoDemanda.setTurno( ( listTurnos == null || listTurnos.size() == 0 ? null : listTurnos.get( 0 ) ) );
		parametroGeracaoDemanda.setCampus( ( listCampus == null || listCampus.size() == 0 ? null : listCampus.get( 0 ) ) );

		return ConvertBeans.toParametroGeracaoDemandaDTO(parametroGeracaoDemanda);
	}
	
	@Override
	public void calculaPrioridadesParaDisciplinasNaoCursadasPorAluno( CenarioDTO cenarioDTO, ParametroGeracaoDemandaDTO parametroGeracaoDemandaDTO )
	{
		Map<Curriculo, Oferta> ofertasCurriculosMap = new HashMap<Curriculo, Oferta>();
		Map<CurriculoDisciplina, Integer> demandas = new HashMap<CurriculoDisciplina, Integer>();
		Map<CurriculoDisciplina, Set<Aluno>> disciplinasMapAlunos = new HashMap<CurriculoDisciplina, Set<Aluno>>();
		ParametroGeracaoDemanda parametroGeracaoDemanda = ConvertBeans.toParametroGeracaoDemanda(parametroGeracaoDemandaDTO);
		Campus campus = parametroGeracaoDemanda.getCampus();
		Turno turno = parametroGeracaoDemanda.getTurno();
		
		initProgressReport("chaveGeracaoDemanda");
		getProgressReport().setInitNewPartial("Iniciando geração das demandas");

		List<Aluno> todosAlunos = Aluno.findAlunosComDisciplinasCursadas(getInstituicaoEnsinoUser());
		Map<Curriculo, Map<Integer, List< CurriculoDisciplina >>> curriculoToPeriodosMapToDisciplinasMap =
				montaCurriculoToPeriodosMapToDisciplinasMap(cenarioDTO);
		for (Aluno aluno : todosAlunos)
		{
			Curriculo curriculoAluno = encontraCurriculo(aluno);
			Map<Integer, List<CurriculoDisciplina>> periodoToAlunoDisciplinasNaoCursadasMap = encontraDisciplinasNaoCursadas(
					curriculoToPeriodosMapToDisciplinasMap, curriculoAluno, aluno);
			int periodoDoAluno = aluno.getPeriodo() == null ? -1 : aluno.getPeriodo();
			boolean alunoTemDisciplinasAtrasadas = false;
			for (List<CurriculoDisciplina> disciplinasNaoCursadas : periodoToAlunoDisciplinasNaoCursadasMap.values() )
			{
				for (CurriculoDisciplina disciplinaNaoCursada : disciplinasNaoCursadas)
				{
					if(disciplinaNaoCursada.getPeriodo() < periodoDoAluno)
					{
						alunoTemDisciplinasAtrasadas = true;
					}
				}
			}
			
			int maxCreditosSemanais = calculaMaxCreditosSemanaisParaOAluno(curriculoToPeriodosMapToDisciplinasMap,
					curriculoAluno, periodoDoAluno, turno, alunoTemDisciplinasAtrasadas, parametroGeracaoDemanda);
			
			Set<Disciplina> disciplinasP1 = new HashSet<Disciplina>();
			int somaCreditosP1 = 0;
			
			List<Integer> periodos = curriculoAluno.getPeriodos();
			Collections.sort(periodos);
			for (Integer periodo : periodos)
			{
				List<CurriculoDisciplina> disciplinasNaoCursadasPeriodo = periodoToAlunoDisciplinasNaoCursadasMap.get(periodo);
				if(disciplinasNaoCursadasPeriodo != null){
					for (CurriculoDisciplina disciplina : disciplinasNaoCursadasPeriodo)
					{
						if (!disciplinasP1.contains(disciplina.getDisciplina()))
						{
							boolean disciplinaPossuiPreRequisitoAindaNaoCursadoPeloAluno = false;
							if (parametroGeracaoDemanda.getConsiderarPreRequisitos())
							{
								disciplinaPossuiPreRequisitoAindaNaoCursadoPeloAluno = temPreRequisitoAindaNaoCursadoPeloAluno(disciplina, curriculoAluno,
										periodoToAlunoDisciplinasNaoCursadasMap);
							}
							Set<CurriculoDisciplina> coRequisitosAindaNaoCursadosENaoOferecidos = new HashSet<CurriculoDisciplina>();
							if (parametroGeracaoDemanda.getConsiderarCoRequisitos())
							{
								coRequisitosAindaNaoCursadosENaoOferecidos = retornaCoRequisitosAindaNaoCursadosENaoOferecidos(curriculoAluno,
										periodo, disciplina, periodoToAlunoDisciplinasNaoCursadasMap, maxCreditosSemanais, disciplinasP1);
							}
							
							int prioridadeCalculada = calculaPrioridade(curriculoAluno, periodo, disciplina.getDisciplina().getTotalCreditos(),
									periodoDoAluno, disciplinaPossuiPreRequisitoAindaNaoCursadoPeloAluno, coRequisitosAindaNaoCursadosENaoOferecidos,
									periodoToAlunoDisciplinasNaoCursadasMap, somaCreditosP1, maxCreditosSemanais,
									parametroGeracaoDemanda.getDistanciaMaxEmPeriodosParaPrioridade2());
							if (prioridadeCalculada == 1)
							{
								somaCreditosP1 += disciplina.getDisciplina().getTotalCreditos();
								disciplinasP1.add(disciplina.getDisciplina());
								if(coRequisitosAindaNaoCursadosENaoOferecidos != null)
								{
									for (CurriculoDisciplina disciplinaCoRequisito : coRequisitosAindaNaoCursadosENaoOferecidos)
									{
										if (!disciplinasP1.contains(disciplinaCoRequisito.getDisciplina()))
										{
											disciplinasP1.add(disciplinaCoRequisito.getDisciplina());
											somaCreditosP1 += disciplinaCoRequisito.getDisciplina().getTotalCreditos();
											if(demandas.get(disciplinaCoRequisito) == null)
											{
												demandas.put(disciplinaCoRequisito, 1);
											}
											else
											{
												demandas.put(disciplinaCoRequisito, demandas.get(disciplinaCoRequisito)+1);
											}
											if(disciplinasMapAlunos.get(disciplinaCoRequisito) == null)
											{
												Set<Aluno> alunos = new HashSet<Aluno>();
												alunos.add(aluno);
												disciplinasMapAlunos.put(disciplinaCoRequisito, alunos);
											}
											else
											{
												disciplinasMapAlunos.get(disciplinaCoRequisito).add(aluno);
											}
										}
									}
								}
								if(demandas.get(disciplina) == null)
								{
									demandas.put(disciplina, 1);
								}
								else
								{
									demandas.put(disciplina, demandas.get(disciplina)+1);
								}
								if(disciplinasMapAlunos.get(disciplina) == null)
								{
									Set<Aluno> alunos = new HashSet<Aluno>();
									alunos.add(aluno);
									disciplinasMapAlunos.put(disciplina, alunos);
								}
								else
								{
									disciplinasMapAlunos.get(disciplina).add(aluno);
								}
							}					
						}
					}
				}
			}
			Oferta novaOferta = new Oferta();
			novaOferta.setCampus(campus);
			novaOferta.setCurriculo(curriculoAluno);
			novaOferta.setTurno(turno);
			novaOferta.setCurso(curriculoAluno.getCurso());
			ofertasCurriculosMap.put(curriculoAluno, novaOferta);
		}
		getProgressReport().setPartial("Etapa concluída");
		
		getProgressReport().setInitNewPartial("Inserindo demandas no banco");
		createOfertasDemandasEAlunosDemanda(ofertasCurriculosMap, demandas, disciplinasMapAlunos);
		getProgressReport().setPartial("Etapa concluída");
		getProgressReport().finish();
	}

	@Transactional
	private void createOfertasDemandasEAlunosDemanda(Map<Curriculo, Oferta> ofertasCurriculosMap,
			Map<CurriculoDisciplina, Integer> demandas,	Map<CurriculoDisciplina, Set<Aluno>> alunosDemanda) {
		int i = 0;
		for (Entry<CurriculoDisciplina, Integer> demanda : demandas.entrySet())
		{
			if (demanda.getValue() > 0)
			{
				Demanda novaDemanda = new Demanda();
				novaDemanda.setDisciplina(demanda.getKey().getDisciplina());
				novaDemanda.setOferta(ofertasCurriculosMap.get(demanda.getKey().getCurriculo()));
				novaDemanda.persist();
				for (Aluno aluno : alunosDemanda.get(demanda.getKey()))
				{
					AlunoDemanda novoAlunoDemanda = new AlunoDemanda();
					novoAlunoDemanda.setAluno(aluno);
					novoAlunoDemanda.setDemanda(novaDemanda);
					novoAlunoDemanda.setPeriodo(demanda.getKey().getPeriodo());
					novoAlunoDemanda.setPrioridade(1);
					novoAlunoDemanda.persist();
					i++;
					if (i%1000 == 0) {
						System.out.println("Inseriu: " + i + " alunosDemanda");
					}
				}
			}
		}
		System.out.println(i + " Demandas por aluno inseridas");
		
	}

	private Curriculo encontraCurriculo( Aluno aluno )
	{
		Curriculo curriculo = null;
		Map<Curriculo, Integer> curriculoToQuantidadeMap = new HashMap<Curriculo, Integer>();
		for (CurriculoDisciplina disciplinasCursadas : aluno.getCursou())
		{
			int maiorNumCurriculos = 0;
			if (curriculoToQuantidadeMap.get(disciplinasCursadas.getCurriculo()) == null)
			{
				curriculoToQuantidadeMap.put(disciplinasCursadas.getCurriculo(), 1);
			}
			else
			{
				curriculoToQuantidadeMap.put(disciplinasCursadas.getCurriculo(),
						curriculoToQuantidadeMap.get(disciplinasCursadas.getCurriculo())+1);
			}
			
			if (curriculoToQuantidadeMap.get(disciplinasCursadas.getCurriculo()) > maiorNumCurriculos)
			{
				curriculo = disciplinasCursadas.getCurriculo();
				maiorNumCurriculos = curriculoToQuantidadeMap.get(disciplinasCursadas.getCurriculo());
			}
		}
		
		return curriculo;
	}
	
	private Map<Curriculo, Map<Integer, List<CurriculoDisciplina>>> montaCurriculoToPeriodosMapToDisciplinasMap(CenarioDTO cenarioDTO)
	{
		List<CurriculoDisciplina> todosCurriculosDisciplinas =
				CurriculoDisciplina.findByCenario(getInstituicaoEnsinoUser(), ConvertBeans.toCenario(cenarioDTO));
		
		Map<Curriculo, Map<Integer, List<CurriculoDisciplina>>> curriculoMapToPeriodosMapToDisciplinas =
				new HashMap<Curriculo, Map<Integer, List<CurriculoDisciplina>>>();
		
		for (CurriculoDisciplina curriculoDisciplina : todosCurriculosDisciplinas)
		{
			if (curriculoMapToPeriodosMapToDisciplinas.get(curriculoDisciplina.getCurriculo()) == null)
			{					
				List<CurriculoDisciplina> novoCurriculoDisciplina = new ArrayList<CurriculoDisciplina>();
				novoCurriculoDisciplina.add(curriculoDisciplina);
				Map<Integer, List<CurriculoDisciplina>> periodoMapToDisciplinas = 
						new HashMap<Integer, List<CurriculoDisciplina>>();
				periodoMapToDisciplinas.put(curriculoDisciplina.getPeriodo(), novoCurriculoDisciplina);
				curriculoMapToPeriodosMapToDisciplinas.put(curriculoDisciplina.getCurriculo(), periodoMapToDisciplinas);
			}
			else
			{
				if (curriculoMapToPeriodosMapToDisciplinas.get(curriculoDisciplina.getCurriculo())
						.get(curriculoDisciplina.getPeriodo()) == null)
				{
					List<CurriculoDisciplina> novoCurriculoDisciplina = new ArrayList<CurriculoDisciplina>();
					novoCurriculoDisciplina.add(curriculoDisciplina);
					curriculoMapToPeriodosMapToDisciplinas.get(curriculoDisciplina.getCurriculo())
						.put(curriculoDisciplina.getPeriodo(), novoCurriculoDisciplina);
				}
				else
				{
					curriculoMapToPeriodosMapToDisciplinas.get(curriculoDisciplina.getCurriculo())
						.get(curriculoDisciplina.getPeriodo()).add(curriculoDisciplina);
				}
			}
		}
		
		return curriculoMapToPeriodosMapToDisciplinas;
	}

	private Map<Integer, List<CurriculoDisciplina>> encontraDisciplinasNaoCursadas(
			Map<Curriculo, Map<Integer, List<CurriculoDisciplina>>> curriculoToPeriodosMapToDisciplinasMap,
			Curriculo curriculo, Aluno aluno)
	{
		Map<Integer, List<CurriculoDisciplina>> periodoToDisciplinasNaoCursadasMap = new HashMap<Integer, List<CurriculoDisciplina>>();

		for (List<CurriculoDisciplina> disciplinas : curriculoToPeriodosMapToDisciplinasMap.get(curriculo).values())
		{
			for (CurriculoDisciplina disciplina : disciplinas)
			{
				if (!disciplina.getCursadoPor().contains(aluno))
				{
					if (periodoToDisciplinasNaoCursadasMap.get(disciplina.getPeriodo()) == null)
					{
						List<CurriculoDisciplina> disciplinasNaoCursadas = new ArrayList<CurriculoDisciplina>();
						disciplinasNaoCursadas.add(disciplina);
						periodoToDisciplinasNaoCursadasMap.put(disciplina.getPeriodo(), disciplinasNaoCursadas);
					}
					else
					{
						periodoToDisciplinasNaoCursadasMap.get(disciplina.getPeriodo()).add(disciplina);
					}
				}
			}
		}

		return periodoToDisciplinasNaoCursadasMap;
	}
	
	private int calculaMaxCreditosSemanaisParaOAluno(Map<Curriculo, Map<Integer, List<CurriculoDisciplina>>> curriculoToPeriodosMapToDisciplinasMap,
			Curriculo curriculo, Integer periodoAluno, Turno turno, boolean alunoTemDisciplinasAtrasadas, ParametroGeracaoDemanda parametroGeracaoDemanda
			)
	{
		int maxCreditosSemanais = 0;
		
		if(parametroGeracaoDemanda.getMaxCreditosPeriodo())
		{
			if (periodoAluno != -1 || curriculoToPeriodosMapToDisciplinasMap.get(curriculo).get(periodoAluno) != null)
			{
				maxCreditosSemanais = calculaTotalCreditosDasDisciplinas(curriculoToPeriodosMapToDisciplinasMap.get(curriculo).get(periodoAluno));
			}
			else
			{
				maxCreditosSemanais = calculaMaxCreditosCurriculo(curriculoToPeriodosMapToDisciplinasMap.get(curriculo));
			}
		}
		else if(parametroGeracaoDemanda.getMaxCreditosManual() != null)
		{
			maxCreditosSemanais = parametroGeracaoDemanda.getMaxCreditosManual();
			if (maxCreditosSemanais > curriculo.getSemanaLetiva().calcTotalCreditosSemanais(turno))
			{
				maxCreditosSemanais = curriculo.getSemanaLetiva().calcTotalCreditosSemanais(turno);
			}
		}
		
		if (parametroGeracaoDemanda.getAumentaMaxCreditosParaAlunosComDisciplinasAtrasadas())
		{
			maxCreditosSemanais = (int) Math.round((double) maxCreditosSemanais * (1 + parametroGeracaoDemanda.getFatorDeAumentoDeMaxCreditos()/100));
		}
		
		return maxCreditosSemanais;
	}

	private boolean temPreRequisitoAindaNaoCursadoPeloAluno( CurriculoDisciplina disciplina, Curriculo curriculoAluno,
			Map<Integer, List<CurriculoDisciplina>> periodoToAlunoDisciplinasNaoCursadasMap)
	{
		for (Disciplina preRequisito : disciplina.getPreRequisitos())
		{
			for (List<CurriculoDisciplina> disciplinasNaoCursadas : periodoToAlunoDisciplinasNaoCursadasMap.values())
			{
				for (CurriculoDisciplina disciplinaNaoCursada : disciplinasNaoCursadas)
				if (disciplinaNaoCursada.getDisciplina().equals(preRequisito))
				{
					return true;
				}
			}
		}
		
		return false;
	}
	
	private boolean temPreRequisitoAindaNaoCursadoPeloAluno( Set<CurriculoDisciplina> disciplinas, Curriculo curriculoAluno,
			Map<Integer, List<CurriculoDisciplina>> periodoToAlunoDisciplinasNaoCursadasMap) {
		for (CurriculoDisciplina disciplina : disciplinas)
		{
			if (temPreRequisitoAindaNaoCursadoPeloAluno(disciplina, curriculoAluno, periodoToAlunoDisciplinasNaoCursadasMap))
			{
				return true;
			}
		}
		return false;
	}
	
	private Set<CurriculoDisciplina> retornaCoRequisitosAindaNaoCursadosENaoOferecidos( Curriculo curriculoAluno, Integer periodo,
			CurriculoDisciplina disciplina,	Map<Integer, List<CurriculoDisciplina>> periodoToAlunoDisciplinasNaoCursadasMap,
			int maxCreditosSemanais, Set<Disciplina> disciplinasP1)
	{
		Set<CurriculoDisciplina> coRequisitosAindaNaoCursados = new HashSet<CurriculoDisciplina>();
		
		for (Disciplina coRequisito : disciplina.getCoRequisitos())
		{
			for (List<CurriculoDisciplina> disciplinasNaoCursadas : periodoToAlunoDisciplinasNaoCursadasMap.values())
			{
				for (CurriculoDisciplina disciplinaNaoCursada : disciplinasNaoCursadas)
				if (disciplinaNaoCursada.getDisciplina().equals(coRequisito) && !disciplinasP1.contains(coRequisito))
				{
					coRequisitosAindaNaoCursados.add(disciplinaNaoCursada);
				}
			}
		}
		
		return coRequisitosAindaNaoCursados;
	}
	
	private int calculaPrioridade(Curriculo curriculoAluno, Integer periodo, Integer totalCreditos,
			Integer periodoAluno, boolean disciplinaPossuiPreRequisitoAindaNaoCursadoPeloAluno,
			Set<CurriculoDisciplina> coRequisitosAindaNaoCursadosENaoOferecidos,
			Map<Integer, List<CurriculoDisciplina>> periodoToAlunoDisciplinasNaoCursadasMap,
			int somaCreditosP1, int maxCreditosSemanais, int distanciaMaxEmPeriodosParaPrioridade2)
	{
		int prioridadeCalculada = 1;
		if (!disciplinaPossuiPreRequisitoAindaNaoCursadoPeloAluno)
		{
            if (!coRequisitosAindaNaoCursadosENaoOferecidos.isEmpty())
            {
            	if (temPreRequisitoAindaNaoCursadoPeloAluno(coRequisitosAindaNaoCursadosENaoOferecidos,
            			curriculoAluno, periodoToAlunoDisciplinasNaoCursadasMap))
            	{
            		prioridadeCalculada = -2;
            	}
            	else
            	{
            		int totalCreditosDisciplinaECoRequisitos = totalCreditos + calculaTotalCreditosDasDisciplinas(coRequisitosAindaNaoCursadosENaoOferecidos);
            		prioridadeCalculada = defineEntrePrioridades1ou2ou3(totalCreditosDisciplinaECoRequisitos, somaCreditosP1, maxCreditosSemanais,
            				periodoAluno, periodo, distanciaMaxEmPeriodosParaPrioridade2);
            	}
            }
            else
            {
        		prioridadeCalculada = defineEntrePrioridades1ou2ou3(totalCreditos, somaCreditosP1, maxCreditosSemanais,
        				periodoAluno, periodo, distanciaMaxEmPeriodosParaPrioridade2);
            }
		}
		else
		{
			prioridadeCalculada = -1;
		}
		
		
		return prioridadeCalculada;
	}

	private int defineEntrePrioridades1ou2ou3( int totalCreditosASerTestado, int somaCreditosP1, int maxCreditosSemanais,
			Integer periodoAluno, Integer periodo,	int distanciaMaxEmPeriodosParaPrioridade2) {
		  if ((somaCreditosP1 + totalCreditosASerTestado) > maxCreditosSemanais)
          {
              // verifica se existe a informação do período atual do aluno
              if (periodoAluno != -1)
              {
                  if (periodo > (periodoAluno + distanciaMaxEmPeriodosParaPrioridade2))
                  {
                      return 3; // ou seja, esta disciplina não cursada pelo aluno poderia fazer parte das ofertas candidatas para o próximo semestre do aluno, porém, não fará
                  }
                  else
                  {
                      return 2; // ou seja, esta disciplina não cursada pelo aluno fará parte das ofertas candidatas para o próximo semestre do aluno, porém, com prioridade 2
                  }
              }
              else
              {
                  return 3; // ou seja, esta disciplina não cursada pelo aluno poderia fazer parte das ofertas candidatas para o próximo semestre do aluno, porém, não fará
              }
          }

          return 1;
	}

	private Integer calculaTotalCreditosDasDisciplinas(
			Collection<CurriculoDisciplina> disciplinas)
	{
		int totalCreditos = 0;
		for (CurriculoDisciplina disciplina : disciplinas)
		{
			totalCreditos += disciplina.getDisciplina().getTotalCreditos();
		}
		return totalCreditos;
	}
	
	private int calculaMaxCreditosCurriculo(
			Map<Integer, List<CurriculoDisciplina>> periodoMapToDisciplinas)
	{
		int maxCreditos = 0;
		for(Entry<Integer, List<CurriculoDisciplina>> disciplinasDoPeriodo : periodoMapToDisciplinas.entrySet())
		{
			int totalCreditosPeriodo = 0;
			for (CurriculoDisciplina disciplina : disciplinasDoPeriodo.getValue())
			{
				totalCreditosPeriodo += disciplina.getDisciplina().getTotalCreditos();
			}
			if (totalCreditosPeriodo > maxCreditos)
			{
				maxCreditos = totalCreditosPeriodo;
			}
		}

		return maxCreditos;
	}
	
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
	
	public void setProgressReport(List<String> fbl){
		progressReport = new ProgressReportListWriter(fbl);
	}
	
	public void setProgressReport(File f) throws IOException{
		progressReport = new ProgressReportFileWriter(f);
	}
	
	public ProgressReportWriter getProgressReport(){
		return progressReport;
	}
	
	@Override
	public QuintetoDTO<CampusDTO, List<DemandaDTO>, DisciplinaDTO, Integer, Integer> getDemandaDTO(CenarioDTO cenarioDTO, ResumoMatriculaDTO resumoMatriculaDTO)
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Campus campus = Campus.find(resumoMatriculaDTO.getCampusId(), getInstituicaoEnsinoUser());
		Disciplina disciplina = Disciplina.find(resumoMatriculaDTO.getDisciplinaId(), getInstituicaoEnsinoUser());
		
		List<Demanda> demandas = Demanda.findBy(getInstituicaoEnsinoUser(), cenario, campus, disciplina);
		Set<AlunoDemanda> alunos = new HashSet<AlunoDemanda>();
		for(Demanda demanda : demandas)
		{
			alunos.addAll(demanda.getAlunosDemanda());
		}
		
		int planejada  = 0;
		int naoPlanejada = 0;
		for(AlunoDemanda aluno : alunos)
		{
			boolean confirmada = false;
			List<AtendimentoOperacional> atendimentos = AtendimentoOperacional.findAllBy(aluno, getInstituicaoEnsinoUser());
			for (AtendimentoOperacional atendimento : atendimentos)
			{
				if (atendimento.getConfirmada() == true)
				{
					confirmada = true;
				}
			}
			if (aluno.getAtendido())
			{
				if (confirmada)
				{
					planejada++;
				}
				else
				{
					naoPlanejada++;
				}
			}
		}
		
		return QuintetoDTO.create(ConvertBeans.toCampusDTO(campus), ConvertBeans.toListDemandaDTO(demandas), ConvertBeans.toDisciplinaDTO(disciplina), planejada, naoPlanejada);
	}

}
