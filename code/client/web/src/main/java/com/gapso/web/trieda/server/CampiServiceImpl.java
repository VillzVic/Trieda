package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.TreeMap;

import org.springframework.transaction.annotation.Transactional;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.DeslocamentoCampus;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.domain.Unidade;
import com.gapso.trieda.misc.Estados;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AbstractDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoTaticoDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DeslocamentoCampusDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.TreeNodeDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.services.CampiService;
import com.gapso.web.trieda.shared.util.TriedaUtil;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
@Transactional
public class CampiServiceImpl extends RemoteServiceServlet implements CampiService {

	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public CampusDTO getCampus(Long id) {
		if(id == null) return null;
		return ConvertBeans.toCampusDTO(Campus.find(id));
	}
	
	@Override
	public PagingLoadResult<HorarioDisponivelCenarioDTO> getHorariosDisponiveis(CampusDTO campusDTO) {
		List<HorarioDisponivelCenario> list = new ArrayList<HorarioDisponivelCenario>(Campus.find(campusDTO.getId()).getHorarios());
		List<HorarioDisponivelCenarioDTO> listDTO = ConvertBeans.toHorarioDisponivelCenarioDTO(list);
		
		Map<String, List<HorarioDisponivelCenarioDTO>> horariosTurnos = new HashMap<String, List<HorarioDisponivelCenarioDTO>>();
		for(HorarioDisponivelCenarioDTO o : listDTO) {
			List<HorarioDisponivelCenarioDTO> horarios = horariosTurnos.get(o.getTurnoString());
			if(horarios == null) {
				horarios = new ArrayList<HorarioDisponivelCenarioDTO>();
				horariosTurnos.put(o.getTurnoString(), horarios);
			}
			horarios.add(o);
		}
		
		for(Entry<String, List<HorarioDisponivelCenarioDTO>> entry : horariosTurnos.entrySet()) {
			Collections.sort(entry.getValue());
		}
		
		Map<Date, List<String>> horariosFinalTurnos = new TreeMap<Date, List<String>>();
		for(Entry<String, List<HorarioDisponivelCenarioDTO>> entry : horariosTurnos.entrySet()) {
			Date ultimoHorario = entry.getValue().get(entry.getValue().size()-1).getHorario();
			List<String> turnos = horariosFinalTurnos.get(ultimoHorario);
			if (turnos == null) {
				turnos = new ArrayList<String>();
				horariosFinalTurnos.put(ultimoHorario,turnos);
			}
			turnos.add(entry.getKey());
		}
		
		listDTO.clear();
		for(Entry<Date, List<String>> entry : horariosFinalTurnos.entrySet()) {
			for (String turno : entry.getValue()) {
				listDTO.addAll(horariosTurnos.get(turno));
			}
		}
		
		return new BasePagingLoadResult<HorarioDisponivelCenarioDTO>(listDTO);
	}
	
	@Override
	public void saveHorariosDisponiveis(CampusDTO campusDTO, List<HorarioDisponivelCenarioDTO> listDTO) {
		List<HorarioDisponivelCenario> listSelecionados = ConvertBeans.toHorarioDisponivelCenario(listDTO);
		Campus campus = Campus.find(campusDTO.getId());
		List<Unidade> unidades = Unidade.findByCampus(campus);
		List<Sala> salas = new ArrayList<Sala>();
		for(Unidade unidade : unidades) {
			salas.addAll(Sala.findByUnidade(unidade)); 
		}
		
		List<HorarioDisponivelCenario> removerList = new ArrayList<HorarioDisponivelCenario> (campus.getHorarios());
		removerList.removeAll(listSelecionados);
		for(HorarioDisponivelCenario o : removerList) {
			o.getCampi().remove(campus);
			o.getUnidades().removeAll(unidades);
			o.getSalas().removeAll(salas);
			o.merge();
		}
		
		List<HorarioDisponivelCenario> adicionarList = new ArrayList<HorarioDisponivelCenario> (listSelecionados);
		adicionarList.removeAll(campus.getHorarios());
		for(HorarioDisponivelCenario o : adicionarList) {
			o.getCampi().add(campus);
			o.getUnidades().addAll(unidades);
			o.getSalas().addAll(salas);
			o.merge();
		}
	}
	
	@Override
	public ListLoadResult<CampusDTO> getListAll() {
		List<CampusDTO> campiDTO = new ArrayList<CampusDTO>();
		List<Campus> campi = Campus.findAll();
		for(Campus c : campi) {
			campiDTO.add(ConvertBeans.toCampusDTO(c));
		}
		return new BaseListLoadResult<CampusDTO>(campiDTO);
	}	
	
	@Override
	public PagingLoadResult<CampusDTO> getList(PagingLoadConfig config) {
		List<CampusDTO> list = new ArrayList<CampusDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		for(Campus campus : Campus.find(config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toCampusDTO(campus));
		}
		BasePagingLoadResult<CampusDTO> result = new BasePagingLoadResult<CampusDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Campus.count());
		return result;
	}

	@Override
	public ListLoadResult<CampusDTO> getList(BasePagingLoadConfig loadConfig) {
		// TODO
		CenarioDTO cenarioDTO = ConvertBeans.toCenarioDTO(Cenario.findMasterData());
		return getBuscaList(cenarioDTO, null, loadConfig.get("query").toString(), null, null, null, loadConfig);
	}

	@Override
	public PagingLoadResult<CampusDTO> getBuscaList(CenarioDTO cenarioDTO, String nome, String codigo, String estadoString, String municipio, String bairro, PagingLoadConfig config) {
		Cenario cenario = Cenario.find(cenarioDTO.getId());
		
		List<CampusDTO> list = new ArrayList<CampusDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		Estados estadoDomain = null;
		if(estadoString != null) {
			for(Estados estado : Estados.values()) {
				if(estado.name().equals(estadoString)) {
					estadoDomain = estado;
					break;
				}
			}
		}
		List<Campus> campi = Campus.findBy(cenario, nome, codigo, estadoDomain, municipio, bairro, config.getOffset(), config.getLimit(), orderBy);
		for(Campus campus : campi) {
			list.add(ConvertBeans.toCampusDTO(campus));
		}
		BasePagingLoadResult<CampusDTO> result = new BasePagingLoadResult<CampusDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Campus.count(cenario, nome, codigo, estadoDomain, municipio, bairro));
		return result;
	}
	
	@Override
	public ListLoadResult<CampusDTO> getList() {
		List<CampusDTO> list = new ArrayList<CampusDTO>();
		for(Campus campus : Campus.findAll()) {
			list.add(ConvertBeans.toCampusDTO(campus));
		}
		return new BaseListLoadResult<CampusDTO>(list);
	}
	
	@Override
	public void save(CampusDTO campusDTO) {
		Campus campus = ConvertBeans.toCampus(campusDTO);
		if(campus.getId() != null && campus.getId() > 0) {
			campus.merge();
		} else {
			campus.persist();
		}
	}
	
	@Override
	public void remove(List<CampusDTO> campusDTOList) {
		for(CampusDTO campusDTO : campusDTOList) {
			ConvertBeans.toCampus(campusDTO).remove();
		}
	}
	
	@Override
	public List<DeslocamentoCampusDTO> getDeslocamentos() {
		List<DeslocamentoCampusDTO> list = new ArrayList<DeslocamentoCampusDTO>();
		List<Campus> listCampi = Campus.findAll();
		for(Campus unidade : listCampi) {
			list.add(ConvertBeans.toDeslocamentoCampusDTO(unidade, listCampi));
		}
		Collections.sort(list, new Comparator<DeslocamentoCampusDTO>() {
			@Override
			public int compare(DeslocamentoCampusDTO o1, DeslocamentoCampusDTO o2) {
				return o1.get("origemString").toString().compareToIgnoreCase(o2.get("origemString").toString());
			}
		});
		return list;
	}
	
	@Override
	public void saveDeslocamento(CenarioDTO cenarioDTO, List<DeslocamentoCampusDTO> list) {
		List<DeslocamentoCampus> deslocamentos = DeslocamentoCampus.findAllByCampus(Cenario.find(cenarioDTO.getId()));
		for(DeslocamentoCampus deslocamento : deslocamentos) {
			deslocamento.remove();
		}
		for(DeslocamentoCampusDTO deslocamentoCampusDTO : list) {
			List<DeslocamentoCampus> deslCamList = ConvertBeans.toDeslocamentoCampus(deslocamentoCampusDTO);
			for(DeslocamentoCampus desl : deslCamList) {
				desl.persist();
			}
		}
	}
	
	@Override
	public List<TreeNodeDTO> getResumos(CenarioDTO cenarioDTO, TreeNodeDTO currentNode) {
		List<TreeNodeDTO> list = new ArrayList<TreeNodeDTO>();
		
		if (currentNode == null) {
			Cenario cenario = Cenario.find(cenarioDTO.getId());
			List<Campus> campi = new ArrayList<Campus>(cenario.getCampi());
			Collections.sort(campi);
			for (Campus campus : campi) {
				CampusDTO campusDTO = ConvertBeans.toCampusDTO(campus);
				TreeNodeDTO nodeDTO = new TreeNodeDTO(campusDTO);
				nodeDTO.setLeaf(true);
				list.add(nodeDTO);
			}
		} else {
			AbstractDTO<?> contentCurrentNode = currentNode.getContent();
			Campus campus = Campus.find(((CampusDTO)contentCurrentNode).getId());
			Double custoCredito = (campus.getValorCredito() != null) ? campus.getValorCredito() : 0.0;
			Integer qtdTurma = AtendimentoTatico.countTurma(campus);
			
			List<Demanda> demandas = Demanda.findAllByCampus(campus);
			Integer qtdAlunosAtendidos = 0;
			Integer qtdAlunosNaoAtendidos = 0;
			for(Demanda demanda : demandas) {
//				System.out.println("DEMANDA: "+demanda.getOferta().getCampus().getNome()+"-"+demanda.getOferta().getCurriculo().getCurso().getNome()+"-"+demanda.getOferta().getCurriculo().getDescricao());// TODO: retirar
//				System.out.println("  discp: "+demanda.getDisciplina().getNome()+" ("+demanda.getQuantidade()+")");// TODO: retirar
				List<AtendimentoTatico> atendimentos = AtendimentoTatico.findAllByDemanda(demanda);
				int demandaAlunosCreditosT = demanda.getDisciplina().getCreditosTeorico()*demanda.getQuantidade();
				int demandaAlunosCreditosP = demanda.getDisciplina().getCreditosPratico()*demanda.getQuantidade();
				int demandaAlunosCreditosNeutros = demandaAlunosCreditosT + demandaAlunosCreditosP; 
				for(AtendimentoTatico atendimento : atendimentos) {
//					System.out.println("  +at: Turma:"+atendimento.getTurma()+" CP:"+atendimento.getCreditosPratico()+" CT:"+atendimento.getCreditosTeorico()+" qtd:"+atendimento.getQuantidadeAlunos());// TODO: retirar
					demandaAlunosCreditosT -= atendimento.getCreditosTeorico()*atendimento.getQuantidadeAlunos();
					demandaAlunosCreditosP -= atendimento.getCreditosPratico()*atendimento.getQuantidadeAlunos();
					demandaAlunosCreditosNeutros -= atendimento.getTotalCreditos()*atendimento.getQuantidadeAlunos();
				}
//				System.out.println("  discp creds: "+demanda.getDisciplina().getTotalCreditos());// TODO: retirar
				if (atendimentos.isEmpty()) {
					qtdAlunosNaoAtendidos += demanda.getQuantidade();
//					System.out.println("  nao atendidos: "+demanda.getQuantidade());// TODO: retirar
				} else {
					if ((demandaAlunosCreditosP > 0) && !demanda.getDisciplina().getLaboratorio()) {
						if(demandaAlunosCreditosNeutros > 0) {
							qtdAlunosNaoAtendidos += demandaAlunosCreditosNeutros/demanda.getDisciplina().getTotalCreditos();
//							System.out.println("  nao atendidosN: "+(demandaAlunosCreditosNeutros/demanda.getDisciplina().getTotalCreditos()));// TODO: retirar
						} else if (demandaAlunosCreditosNeutros < 0) {
//							System.out.println("  sobra: "+demandaAlunosCreditosNeutros);// TODO: retirar
						}
					} else {
						if(demandaAlunosCreditosT > 0) {
							qtdAlunosNaoAtendidos += demandaAlunosCreditosT/demanda.getDisciplina().getCreditosTeorico();
//							System.out.println("  nao atendidosT: "+(demandaAlunosCreditosT/demanda.getDisciplina().getCreditosTeorico()));// TODO: retirar
						} else if (demandaAlunosCreditosT < 0) {
//							System.out.println("  sobra: "+demandaAlunosCreditosT);// TODO: retirar
						}
						if(demandaAlunosCreditosP > 0) {
							qtdAlunosNaoAtendidos += demandaAlunosCreditosP/demanda.getDisciplina().getCreditosPratico();
//							System.out.println("  nao atendidosP: "+(demandaAlunosCreditosP/demanda.getDisciplina().getCreditosPratico()));// TODO: retirar
						} else if (demandaAlunosCreditosT < 0) {
//							System.out.println("  sobra: "+demandaAlunosCreditosP);// TODO: retirar
						}
					}
				}
//				System.out.println("  total nao atendidos: "+qtdAlunosNaoAtendidos);// TODO: retirar
			}
			qtdAlunosAtendidos = Demanda.sumDemanda(campus) - qtdAlunosNaoAtendidos;
			
			List<AtendimentoTatico> atendimentoTaticoList = AtendimentoTatico.findAllByCampus(campus);
			Set<Sala> salas = new HashSet<Sala>();
			Set<Turno> turnos = new HashSet<Turno>();

			for(AtendimentoTatico atendimentoTatico : atendimentoTaticoList) {
				salas.add(atendimentoTatico.getSala());
				turnos.add(atendimentoTatico.getOferta().getTurno());
			}
			Collection<SalaDTO> salasDTO = ConvertBeans.toSalaDTO(salas);
			Collection<TurnoDTO> turnosDTO = ConvertBeans.toTurnoDTO(turnos);
			
			double numeradorSala = 0.0;
			double denominadorSala = 0.0;
			double numeradorLab = 0.0;
			double denominadorLab = 0.0;
			Integer qtdCreditos = 0;
			AtendimentosServiceImpl atService = new AtendimentosServiceImpl();
			for(TurnoDTO turnoDTO : turnosDTO) {
				for(SalaDTO salaDTO : salasDTO) {
					List<AtendimentoTaticoDTO> atendimentosDTO = atService.getBusca(salaDTO, turnoDTO);
					for(AtendimentoTaticoDTO atendimentoDTO : atendimentosDTO) {
						if(!salaDTO.isLaboratorio()) {
							numeradorSala += atendimentoDTO.getQuantidadeAlunos();
							denominadorSala += salaDTO.getCapacidade();
						} else { 
							numeradorLab += atendimentoDTO.getQuantidadeAlunos();
							denominadorLab += salaDTO.getCapacidade();
						}
						qtdCreditos += atendimentoDTO.getTotalCreditos();
					}
				}
			}
			double mediaSalaDeAula = TriedaUtil.round(numeradorSala / denominadorSala * 100.0, 2);
			double mediaLaboratorio = TriedaUtil.round(numeradorLab / denominadorLab * 100.0, 2);
			
			Double mediaCreditoTurma = (qtdTurma == 0)? 0.0 : qtdCreditos/qtdTurma;
			Double custoDocenteSemestral = qtdCreditos * custoCredito * 4.5 * 6.0;
			
			list.add(new TreeNodeDTO("Turmas abertas: <b>"+qtdTurma+"</b>"));
			list.add(new TreeNodeDTO("Total de Cr&eacute;ditos semanais: <b>"+qtdCreditos+"</b>"));
			list.add(new TreeNodeDTO("M&eacute;dia de cr&eacute;ditos por turma: <b>"+mediaCreditoTurma+"</b>"));
			list.add(new TreeNodeDTO("Custo m&eacute;dio do cr&eacute;dito: <b>R$ "+custoCredito+"</b>"));
			list.add(new TreeNodeDTO("Custo docente semestral estimado: <b>R$ "+custoDocenteSemestral+"</b>"));
			list.add(new TreeNodeDTO("Utiliza&ccedil;&atilde;o m&eacute;dia das salas de aula: <b>"+mediaSalaDeAula+"%</b>"));
			list.add(new TreeNodeDTO("Utiliza&ccedil;&atilde;o m&eacute;dia dos laborat&oacute;rios: <b>"+mediaLaboratorio+"%</b>"));
//			list.add(new TreeNodeDTO("Custo docente por curso"));
//			list.add(new TreeNodeDTO("Custo docente por disciplina"));
			list.add(new TreeNodeDTO("Total de alunos atendidos: <b>"+qtdAlunosAtendidos+"</b>"));
			list.add(new TreeNodeDTO("Total de alunos n&atilde;o atendidos: <b>"+qtdAlunosNaoAtendidos+"</b>"));
//			list.add(new TreeNodeDTO("Exportar resultados para excel"));
		}
		return list;
	}
}
