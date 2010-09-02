package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.SalaDTO;
import com.gapso.web.trieda.client.mvp.model.TipoSalaDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("salas")
public interface SalasService extends RemoteService {
	
	PagingLoadResult<SalaDTO> getList(PagingLoadConfig config);
	ListLoadResult<SalaDTO> getList();
	void save(SalaDTO salaDTO);
	void remove(List<SalaDTO> salaDTOList);
	ListLoadResult<TipoSalaDTO> getTipoSalaList();
	
}
