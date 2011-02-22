package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.client.mvp.model.AtendimentoTaticoDTO;
import com.gapso.web.trieda.client.mvp.model.CurriculoDTO;
import com.gapso.web.trieda.client.mvp.model.SalaDTO;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.gapso.web.trieda.client.services.AtendimentosService;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class AtendimentosServiceImpl extends RemoteServiceServlet implements AtendimentosService {

	private static final long serialVersionUID = -1505176338607927637L;

	@Override
	public PagingLoadResult<AtendimentoTaticoDTO> getList() {
		List<AtendimentoTaticoDTO> list = new ArrayList<AtendimentoTaticoDTO>();
		List<AtendimentoTatico> atendimentosTatico = AtendimentoTatico.findAll();
		for(AtendimentoTatico atendimentoTatico : atendimentosTatico) {
			list.add(ConvertBeans.toAtendimentoTaticoDTO(atendimentoTatico));
		}
		BasePagingLoadResult<AtendimentoTaticoDTO> result = new BasePagingLoadResult<AtendimentoTaticoDTO>(list);
		result.setOffset(0);
		result.setTotalLength(100);
		return result;
	}

	@Override
	public List<AtendimentoTaticoDTO> getBusca(SalaDTO salaDTO, TurnoDTO turnoDTO) {
		Sala sala = Sala.find(salaDTO.getId());
		Turno turno = Turno.find(turnoDTO.getId());
		List<AtendimentoTaticoDTO> list = new ArrayList<AtendimentoTaticoDTO>();
		List<AtendimentoTatico> atendimentosTatico = AtendimentoTatico.findBySalaAndTurno(sala, turno);
		for(AtendimentoTatico atendimentoTatico : atendimentosTatico) {
			list.add(ConvertBeans.toAtendimentoTaticoDTO(atendimentoTatico));
		}
		
		return montaListaParaVisaoSala(list);
	}

	private List<AtendimentoTaticoDTO> montaListaParaVisaoSala(List<AtendimentoTaticoDTO> list) {
		// Agrupa os DTOS pela chave [Disciplina-Turma-DiaSemana] 
		Map<String, List<AtendimentoTaticoDTO>> atendimentoTaticoDTOMap = new HashMap<String, List<AtendimentoTaticoDTO>>();
		for (AtendimentoTaticoDTO dto : list) {
			String key = dto.getDisciplinaString() + "-" + dto.getTurma() + "-" + dto.getSemana();
			List<AtendimentoTaticoDTO> dtoList = atendimentoTaticoDTOMap.get(key);
			if (dtoList == null) {
				dtoList = new ArrayList<AtendimentoTaticoDTO>();
				atendimentoTaticoDTOMap.put(key,dtoList);
			}
			dtoList.add(dto);
		}
		
		// Quando há mais de um DTO por chave [Disciplina-Turma-DiaSemana], concatena as informações
		// de todos em um único DTO.
		List<AtendimentoTaticoDTO> processedList = new ArrayList<AtendimentoTaticoDTO>();
		for (Entry<String,List<AtendimentoTaticoDTO>> entry : atendimentoTaticoDTOMap.entrySet()) {
			if (entry.getValue().size() == 1) {
				processedList.addAll(entry.getValue());
			} else {
				AtendimentoTaticoDTO dtoMain = entry.getValue().get(0);
				for (int i = 1; i < entry.getValue().size(); i++) {
					AtendimentoTaticoDTO dtoCurrent = entry.getValue().get(i);
					dtoMain.concatenateVisaoSala(dtoCurrent);
				}
				processedList.add(dtoMain);
			}
		}
		return processedList;
	}
	
	@Override
	public List<AtendimentoTaticoDTO> getBusca(CurriculoDTO curriculoDTO, Integer periodo, TurnoDTO turnoDTO) {
		Curriculo curriculo = Curriculo.find(curriculoDTO.getId());
		Turno turno = Turno.find(turnoDTO.getId());
		List<AtendimentoTaticoDTO> list = new ArrayList<AtendimentoTaticoDTO>();
		List<AtendimentoTatico> atendimentosTatico = AtendimentoTatico.findByCurriculoAndPeriodoAndTurno(curriculo, periodo, turno);
		for(AtendimentoTatico atendimentoTatico : atendimentosTatico) {
			list.add(ConvertBeans.toAtendimentoTaticoDTO(atendimentoTatico));
		}
		
		return montaListaParaVisaoCurso(turnoDTO,list);
	}

	private List<AtendimentoTaticoDTO> montaListaParaVisaoCurso(TurnoDTO turnoDTO, List<AtendimentoTaticoDTO> list) {
		// Agrupa os DTOS pelo dia da semana 
		Map<Integer,List<AtendimentoTaticoDTO>> diaSemanaToAtendimentoTaticoDTOMap = new HashMap<Integer, List<AtendimentoTaticoDTO>>();
		for (AtendimentoTaticoDTO dto : list) {
			List<AtendimentoTaticoDTO> dtoList = diaSemanaToAtendimentoTaticoDTOMap.get(dto.getSemana());
			if (dtoList == null) {
				dtoList = new ArrayList<AtendimentoTaticoDTO>();
				diaSemanaToAtendimentoTaticoDTOMap.put(dto.getSemana(),dtoList);
			}
			dtoList.add(dto);
		}
		
//		List<Integer> diaSemanaTamanhoList = new ArrayList<Integer>(8); // TODO: continuar código para mostrar + de 1 retangulo por dia
//		Collections.addAll(diaSemanaTamanhoList, 1, 1, 1, 1, 1, 1, 1, 1, 1); // TODO: continuar código para mostrar + de 1 retangulo por dia
		List<AtendimentoTaticoDTO> finalProcessedList = new ArrayList<AtendimentoTaticoDTO>();
		
		// para cada dia da semana ...
		for (Entry<Integer,List<AtendimentoTaticoDTO>> entry : diaSemanaToAtendimentoTaticoDTOMap.entrySet()) {
			List<List<AtendimentoTaticoDTO>> listListDTO = new ArrayList<List<AtendimentoTaticoDTO>>();
			// verifica se o dia da semana extrapola a quantidade máxima de créditos
			if (AtendimentoTaticoDTO.countListDTOsCreditos(entry.getValue()) > turnoDTO.getMaxCreditos()) {
				// executa abordagem 1
				listListDTO = agrupaAtendimentosAbordagem1(entry);
				// verifica se o dia da semana continua extrapolando a quantidade máxima de créditos após a execução da abordagem 1
				if (AtendimentoTaticoDTO.countListListDTOsCreditos(listListDTO) > turnoDTO.getMaxCreditos()) {
					// executa abordagem 2
					listListDTO.clear();
					listListDTO = agrupaAtendimentosAbordagem2(entry);
				}
				
				// Quando há mais de um DTO nas listas internas, concatena as informações
				// de todos em um único DTO.
				for (List<AtendimentoTaticoDTO> listDTOs : listListDTO) {
					AtendimentoTaticoDTO dtoMain = listDTOs.get(0);
					for (int i = 1; i < listDTOs.size(); i++) {
						AtendimentoTaticoDTO dtoCurrent = listDTOs.get(i);
						dtoMain.concatenateVisaoCurso(dtoCurrent);
					}
					finalProcessedList.add(dtoMain);
				}
			} else {
				finalProcessedList.addAll(entry.getValue());
			}
			
//			int size = 1;  // TODO: continuar código para mostrar + de 1 retangulo por dia
//			for (List<AtendimentoTaticoDTO> listDTO : listListDTO) {
//				if (listDTO.size() > size) {
//					size = listDTO.size();
//				}
//			}			
//			diaSemanaTamanhoList.add(entry.getKey(),size); // TODO: continuar código para mostrar + de 1 retangulo por dia
		}
		
		return finalProcessedList;
	}

	private List<List<AtendimentoTaticoDTO>> agrupaAtendimentosAbordagem2(Entry<Integer, List<AtendimentoTaticoDTO>> entry) {
		List<List<AtendimentoTaticoDTO>> listListDTO = new ArrayList<List<AtendimentoTaticoDTO>>();
		List<AtendimentoTaticoDTO> sortedDTOs = new ArrayList<AtendimentoTaticoDTO>(entry.getValue());
		Collections.sort(sortedDTOs, new Comparator<AtendimentoTaticoDTO> () {
			@Override
			public int compare(AtendimentoTaticoDTO o1, AtendimentoTaticoDTO o2) {
				return o1.getTurma().compareTo(o2.getTurma());
			}});
		for (AtendimentoTaticoDTO currentDTO : sortedDTOs) {
			if (listListDTO.isEmpty()) {
				listListDTO.add(new ArrayList<AtendimentoTaticoDTO>());
				listListDTO.get(0).add(currentDTO);
			} else {
				boolean wasDTOProcessed = false;
				for (List<AtendimentoTaticoDTO> listDTO : listListDTO) {
					boolean wasDTORejected = false;
					for (AtendimentoTaticoDTO dto : listDTO) {
						if (!AtendimentoTaticoDTO.compatibleByApproach2(currentDTO,dto)) {
							wasDTORejected = true;
							break;
						}
					}
					if (!wasDTORejected) {
						listDTO.add(currentDTO);
						wasDTOProcessed = true;
						break;
					}
				}
				if (!wasDTOProcessed) {
					listListDTO.add(new ArrayList<AtendimentoTaticoDTO>());
					listListDTO.get(listListDTO.size()-1).add(currentDTO);
				}
			}
		}
		return listListDTO;
	}

	private List<List<AtendimentoTaticoDTO>> agrupaAtendimentosAbordagem1(Entry<Integer, List<AtendimentoTaticoDTO>> entry) {
		List<List<AtendimentoTaticoDTO>> listListDTO = new ArrayList<List<AtendimentoTaticoDTO>>();
		for (AtendimentoTaticoDTO currentDTO : entry.getValue()) {
			if (listListDTO.isEmpty()) {
				listListDTO.add(new ArrayList<AtendimentoTaticoDTO>());
				listListDTO.get(0).add(currentDTO);
			} else {
				boolean wasDTOProcessed = false;
				for (List<AtendimentoTaticoDTO> listDTO : listListDTO) {
					boolean wasDTORejected = false;
					for (AtendimentoTaticoDTO dto : listDTO) {
						if (!AtendimentoTaticoDTO.compatibleByApproach1(currentDTO,dto)) {
							wasDTORejected = true;
							break;
						}
					}
					if (!wasDTORejected) {
						listDTO.add(currentDTO);
						wasDTOProcessed = true;
					}
				}
				if (!wasDTOProcessed) {
					listListDTO.add(new ArrayList<AtendimentoTaticoDTO>());
					listListDTO.get(listListDTO.size()-1).add(currentDTO);
				}
			}
		}
		return listListDTO;
	}
	
}
