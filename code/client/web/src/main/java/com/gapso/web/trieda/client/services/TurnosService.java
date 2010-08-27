package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("turnos")
public interface TurnosService extends RemoteService {
	
	public PagingLoadResult<TurnoDTO> getList();
	public boolean save(TurnoDTO turnoDTO);
	boolean remove(List<TurnoDTO> turnoDTOList);
	
}
