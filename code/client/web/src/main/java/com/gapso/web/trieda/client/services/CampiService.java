package com.gapso.web.trieda.client.services;

import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("campi")
public interface CampiService extends RemoteService {
	
	public PagingLoadResult<ModelData> getList();
	
}