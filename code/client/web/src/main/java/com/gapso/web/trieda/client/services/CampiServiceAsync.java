package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.util.view.simplecrud.ISimpleGridService;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface CampiServiceAsync extends ISimpleGridService {
	
	void getList(PagingLoadConfig config, AsyncCallback<PagingLoadResult<CampusDTO>> callback);
	void getList(AsyncCallback<ListLoadResult<CampusDTO>> callback);
	void save(CampusDTO campusDTO, AsyncCallback<Void> callback);
	void remove(List<CampusDTO> campusDTOList, AsyncCallback<Void> callback);
	
}
