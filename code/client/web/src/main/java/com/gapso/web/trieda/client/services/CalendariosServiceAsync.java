package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.CalendarioDTO;
import com.gapso.web.trieda.client.util.view.simplecrud.ISimpleGridService;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface CalendariosServiceAsync extends ISimpleGridService {
	
	void getList(PagingLoadConfig config, AsyncCallback<PagingLoadResult<CalendarioDTO>> callback);
	void save(CalendarioDTO calendarioDTO, AsyncCallback<Void> callback);
	void remove(List<CalendarioDTO> calendarioDTOList, AsyncCallback<Void> callback);
}
