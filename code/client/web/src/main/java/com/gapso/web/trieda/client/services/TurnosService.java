package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("turnos")
public interface TurnosService extends RemoteService {
	
	TurnoDTO getTurno(Long id);
	ListLoadResult<TurnoDTO> getList();
	ListLoadResult<TurnoDTO> getList(BasePagingLoadConfig loadConfig);
	void save(TurnoDTO turnoDTO);
	void remove(List<TurnoDTO> turnoDTOList);
	PagingLoadResult<TurnoDTO> getBuscaList(String nome, Integer tempo, PagingLoadConfig config);
	ListLoadResult<TurnoDTO> getListByCampus(CampusDTO campusDTO);
	
}
