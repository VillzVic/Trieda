package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.CalendarioDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface CalendariosServiceAsync {
	
	void getList(PagingLoadConfig config, AsyncCallback<PagingLoadResult<CalendarioDTO>> callback);
	void getList(AsyncCallback<ListLoadResult<CalendarioDTO>> callback);
	void save(CalendarioDTO calendarioDTO, AsyncCallback<Void> callback);
	void remove(List<CalendarioDTO> calendarioDTOList, AsyncCallback<Void> callback);
	
}
