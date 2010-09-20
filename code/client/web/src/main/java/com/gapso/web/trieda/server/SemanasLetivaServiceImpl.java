package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.TreeMap;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.web.trieda.client.mvp.model.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.client.mvp.model.SemanaLetivaDTO;
import com.gapso.web.trieda.client.services.SemanasLetivaService;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class SemanasLetivaServiceImpl extends RemoteServiceServlet implements SemanasLetivaService {

	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public PagingLoadResult<SemanaLetivaDTO> getList(PagingLoadConfig config) {
		
		List<SemanaLetivaDTO> list = new ArrayList<SemanaLetivaDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		
		for(SemanaLetiva semanaLetiva : SemanaLetiva.find(config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toSemanaLetivaDTO(semanaLetiva));
		}
		
		BasePagingLoadResult<SemanaLetivaDTO> result = new BasePagingLoadResult<SemanaLetivaDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(SemanaLetiva.count());
		return result;
	}
	
	@Override
	public ListLoadResult<SemanaLetivaDTO> getList(BasePagingLoadConfig loadConfig) {
		return getBuscaList(loadConfig.get("query").toString(), null, loadConfig);
	}
	
	@Override
	public PagingLoadResult<SemanaLetivaDTO> getBuscaList(String codigo, String descricao, PagingLoadConfig config) {
		List<SemanaLetivaDTO> list = new ArrayList<SemanaLetivaDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		for(SemanaLetiva semanaLetiva : SemanaLetiva.findByCodigoLikeAndDescricaoLike(codigo, descricao, config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toSemanaLetivaDTO(semanaLetiva));
		}
		BasePagingLoadResult<SemanaLetivaDTO> result = new BasePagingLoadResult<SemanaLetivaDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(SemanaLetiva.count());
		return result;
	}
	
	@Override
	public ListLoadResult<SemanaLetivaDTO> getList() {
		List<SemanaLetivaDTO> list = new ArrayList<SemanaLetivaDTO>();
		for(SemanaLetiva semanaLetiva : SemanaLetiva.findAll()) {
			list.add(ConvertBeans.toSemanaLetivaDTO(semanaLetiva));
		}
		return new BaseListLoadResult<SemanaLetivaDTO>(list);
	}

	@Override
	public void save(SemanaLetivaDTO semanaLetivaDTO) {
		SemanaLetiva semanaLetiva = ConvertBeans.toSemanaLetiva(semanaLetivaDTO);
		if(semanaLetiva.getId() != null && semanaLetiva.getId() > 0) {
			semanaLetiva.merge();
		} else {
			semanaLetiva.persist();
		}
	}
	
	@Override
	public void remove(List<SemanaLetivaDTO> semanaLetivaDTOList) {
		for(SemanaLetivaDTO semanaLetivaDTO : semanaLetivaDTOList) {
			ConvertBeans.toSemanaLetiva(semanaLetivaDTO).remove();
		}
	}
	
	@Override
	public PagingLoadResult<HorarioDisponivelCenarioDTO> getHorariosDisponiveisCenario(SemanaLetivaDTO semanaLetivaDTO) {
		SemanaLetiva semanaLetiva = SemanaLetiva.find(semanaLetivaDTO.getId());
		List<HorarioDisponivelCenarioDTO> list = new ArrayList<HorarioDisponivelCenarioDTO>();
		for(HorarioAula o : semanaLetiva.getHorariosAula()) {
			list.add(ConvertBeans.toHorarioDisponivelCenarioDTO(o));
		}
		
		Map<String, List<HorarioDisponivelCenarioDTO>> horariosTurnos = new HashMap<String, List<HorarioDisponivelCenarioDTO>>();
		for(HorarioDisponivelCenarioDTO o : list) {
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
		
		list.clear();
		for(Entry<Date, List<String>> entry : horariosFinalTurnos.entrySet()) {
			for (String turno : entry.getValue()) {
				list.addAll(horariosTurnos.get(turno));
			}
		}
		
		BasePagingLoadResult<HorarioDisponivelCenarioDTO> result = new BasePagingLoadResult<HorarioDisponivelCenarioDTO>(list);
		result.setOffset(0);
		result.setTotalLength(list.size());
		return result;
	}
	
	@Override
	public void saveHorariosDisponiveisCenario(List<HorarioDisponivelCenarioDTO> listDTO) {
		List<HorarioDisponivelCenario> list = ConvertBeans.toHorarioDisponivelCenario(listDTO);
		HorarioAula horarioAula = HorarioAula.find(listDTO.get(0).getHorarioDeAulaId());
		
		Set<HorarioDisponivelCenario> listOrig = horarioAula.getHorariosDisponiveisCenario();
		for(HorarioDisponivelCenario o1 : list) {
			if(!listOrig.contains(o1)) {
				listOrig.add(o1);
			}
		}
		
//		List<HorarioDisponivelCenario> remover = new ArrayList<HorarioDisponivelCenario>();
//		for(HorarioDisponivelCenario o : listOrig) {
//			if(!list.contains(o)) {
//				remover.add(o);
//			}
//		}
//		for(HorarioDisponivelCenario o : remover) {
//			o.remove();
//		}
		
		horarioAula.merge();
	}
	
}
