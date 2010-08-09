package com.gapso.web.trieda.client.services;

import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.util.view.simplecrud.ICrudService;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface PeriodosDeAulasServiceAsync extends ICrudService {
	public void getList(AsyncCallback<PagingLoadResult<ModelData>> callback);
}
