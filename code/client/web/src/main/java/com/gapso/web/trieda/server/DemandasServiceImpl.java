package com.gapso.web.trieda.server;

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
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.util.Atendimento;
import com.gapso.web.trieda.server.util.Atendimento.TipoCredito;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.services.DemandasService;
import com.gapso.web.trieda.shared.util.view.TriedaException;

public class DemandasServiceImpl
	extends RemoteService implements DemandasService
{
	private static final long serialVersionUID = 5250776996542788849L;
	
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
			for (AtendimentoOperacional atendimentoOperacional : oferta.getAtendimentosOperacionais()) {
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
			for (AtendimentoOperacional atendimentoOperacional : oferta.getAtendimentosOperacionais()) {
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
	public void save( DemandaDTO demandaDTO ) throws TriedaException
	{
		try {
			Demanda d = ConvertBeans.toDemanda( demandaDTO );
	
			if ( d.getId() != null && d.getId() > 0 )
			{
				d.merge();
			}
			else
			{
				List< Demanda > demandas = Demanda.findBy( getInstituicaoEnsinoUser(),
					d.getOferta().getCampus().getCenario(),	d.getOferta().getCampus(), d.getOferta().getCurriculo().getCurso(),
					d.getOferta().getCurriculo(), d.getOferta().getTurno(), d.getDisciplina(), 0, 1, null );
	
				if ( !demandas.isEmpty() )
				{
					Integer qtd = d.getQuantidade();
	
					d = demandas.get( 0 );
					d.setQuantidade( qtd );
	
					d.merge();
				}
				else
				{
					d.persist();
				}
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
}
