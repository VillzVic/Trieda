package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.UnidadeDTO;
import com.gapso.web.trieda.client.util.view.simplecrud.ISimpleGridService;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface UnidadesServiceAsync extends ISimpleGridService {

	void getList(PagingLoadConfig config, AsyncCallback<PagingLoadResult<UnidadeDTO>> callback);
	void getList(AsyncCallback<ListLoadResult<UnidadeDTO>> callback);
	void save(UnidadeDTO unidadeDTO, AsyncCallback<Void> callback);
	void remove(List<UnidadeDTO> unidadeDTOList, AsyncCallback<Void> callback);
	
}
