package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.PeriodoDeAulaDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("periodosDeAula")
public interface PeriodosDeAulaService extends RemoteService {
	
	PagingLoadResult<PeriodoDeAulaDTO> getList(PagingLoadConfig config);
	
	void save(PeriodoDeAulaDTO periodoDeAula);
	void remove(List<PeriodoDeAulaDTO> periodoDeAulaList);
	
}
