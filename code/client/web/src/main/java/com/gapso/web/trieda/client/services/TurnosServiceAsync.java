package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.gapso.web.trieda.client.util.view.simplecrud.ISimpleGridService;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface TurnosServiceAsync extends ISimpleGridService {
	
	void getList(AsyncCallback<PagingLoadResult<TurnoDTO>> callback);
	void save(TurnoDTO turnoDTO, AsyncCallback<Boolean> callback);
	void remove(List<TurnoDTO> turnoDTOList, AsyncCallback<Boolean> callback);
	
}
