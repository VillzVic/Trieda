package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.gapso.web.trieda.client.services.TurnosService;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class TurnosServiceImpl extends RemoteServiceServlet implements TurnosService {

	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public TurnoDTO getTurno(Long id) {
		return ConvertBeans.toTurnoDTO(Turno.find(id));
	}
	
	@Override
	public PagingLoadResult<TurnoDTO> getBuscaList(String nome, Integer tempo, PagingLoadConfig config) {
		List<TurnoDTO> list = new ArrayList<TurnoDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		for(Turno turno : Turno.findBy(nome, tempo, config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toTurnoDTO(turno));
		}
		BasePagingLoadResult<TurnoDTO> result = new BasePagingLoadResult<TurnoDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Turno.count(nome, tempo));
		return result;
	}
	
	@Override
	public ListLoadResult<TurnoDTO> getListByCampus(CampusDTO campusDTO) {
		List<TurnoDTO> list = new ArrayList<TurnoDTO>();
		Campus campus = Campus.find(campusDTO.getId());
		List<Turno> turnos = Turno.findBy(campus);
		for(Turno turno : turnos) {
			list.add(ConvertBeans.toTurnoDTO(turno));
		}
		return new BaseListLoadResult<TurnoDTO>(list);
	}

	@Override
	public ListLoadResult<TurnoDTO> getList() {
		List<TurnoDTO> list = new ArrayList<TurnoDTO>();
		for(Turno turno : Turno.findAll()) {
			list.add(ConvertBeans.toTurnoDTO(turno));
		}
		return new BaseListLoadResult<TurnoDTO>(list);
	}

	@Override
	public ListLoadResult<TurnoDTO> getList(BasePagingLoadConfig loadConfig) {
		CampusDTO campusDTO = loadConfig.get("campusDTO");
		ListLoadResult<TurnoDTO> list = null;
		if(campusDTO == null) {
			list = getBuscaList(loadConfig.get("query").toString(), null, loadConfig);
		} else {
			Campus campus = Campus.find(campusDTO.getId());
			List<Turno> turnos = Turno.findBy(campus);
			List<TurnoDTO> turnosDTO = new ArrayList<TurnoDTO>();
			for(Turno turno : turnos) {
				turnosDTO.add(ConvertBeans.toTurnoDTO(turno));
			}
			list = new BasePagingLoadResult<TurnoDTO>(turnosDTO);
		}
		return list;
	}
	
	@Override
	public void save(TurnoDTO turnoDTO) {
		Turno turno = ConvertBeans.toTurno(turnoDTO);
		if(turno.getId() != null && turno.getId() > 0) {
			turno.merge();
		} else {
			turno.persist();
		}
	}
	
	@Override
	public void remove(List<TurnoDTO> turnoDTOList) {
		for(TurnoDTO turnoDTO : turnoDTOList) {
			ConvertBeans.toTurno(turnoDTO).remove();
		}
	}

}
