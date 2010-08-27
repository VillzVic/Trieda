package com.gapso.web.trieda.client.util.view.simplecrud;

import com.extjs.gxt.ui.client.data.BaseModel;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface ISimpleGridService {

	public void getList(AsyncCallback<PagingLoadResult<BaseModel>> callback);
	
}
