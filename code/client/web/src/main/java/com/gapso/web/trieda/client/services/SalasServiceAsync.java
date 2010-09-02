package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.SalaDTO;
import com.gapso.web.trieda.client.mvp.model.TipoSalaDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface SalasServiceAsync {

	void getList(PagingLoadConfig config, AsyncCallback<PagingLoadResult<SalaDTO>> callback);
	void getList(AsyncCallback<ListLoadResult<SalaDTO>> callback);
	void save(SalaDTO salaDTO, AsyncCallback<Void> callback);
	void remove(List<SalaDTO> salaDTOList, AsyncCallback<Void> callback);
	void getTipoSalaList(AsyncCallback<ListLoadResult<TipoSalaDTO>> callback);
	
}
