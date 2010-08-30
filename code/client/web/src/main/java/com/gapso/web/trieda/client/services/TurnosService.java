package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("turnos")
public interface TurnosService extends RemoteService {
	
	void save(TurnoDTO turnoDTO);
	void remove(List<TurnoDTO> turnoDTOList);
	PagingLoadResult<TurnoDTO> getList(PagingLoadConfig config);
	
}
