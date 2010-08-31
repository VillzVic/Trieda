package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.PeriodoDeAulaDTO;
import com.gapso.web.trieda.client.util.view.simplecrud.ISimpleGridService;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface PeriodosDeAulaServiceAsync extends ISimpleGridService {
	
	void getList(PagingLoadConfig config, AsyncCallback<PagingLoadResult<PeriodoDeAulaDTO>> callback);
	void save(PeriodoDeAulaDTO periodoDeAulaDTO, AsyncCallback<Void> callback);
	void remove(List<PeriodoDeAulaDTO> periodoDeAulaDTOList, AsyncCallback<Void> callback);
	
}
