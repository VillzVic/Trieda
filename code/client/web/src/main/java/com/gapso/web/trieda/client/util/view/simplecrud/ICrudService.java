package com.gapso.web.trieda.client.util.view.simplecrud;

import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface ICrudService {

	public void getList(AsyncCallback<PagingLoadResult<ModelData>> callback);
	
}
