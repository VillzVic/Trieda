package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.CalendarioDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("calendarios")
public interface CalendariosService extends RemoteService {
	
	PagingLoadResult<CalendarioDTO> getList(PagingLoadConfig config);
	ListLoadResult<CalendarioDTO> getList();
	ListLoadResult<CalendarioDTO> getList(BasePagingLoadConfig loadConfig);
	PagingLoadResult<CalendarioDTO> getBuscaList(String codigo, String descricao, PagingLoadConfig config);
	void save(CalendarioDTO calendarioDTO);
	void remove(List<CalendarioDTO> calendarioDTOList);
	
}
