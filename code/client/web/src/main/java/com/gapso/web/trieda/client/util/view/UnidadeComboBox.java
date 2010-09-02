package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.ListLoader;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.client.mvp.model.UnidadeDTO;
import com.gapso.web.trieda.client.services.UnidadesServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class UnidadeComboBox extends ComboBox<UnidadeDTO> {

	private ListStore<UnidadeDTO> store;
	
	public UnidadeComboBox() {
		final UnidadesServiceAsync service = Services.unidades();
		RpcProxy<ListLoadResult<UnidadeDTO>> proxy = new RpcProxy<ListLoadResult<UnidadeDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<UnidadeDTO>> callback) {
				service.getList(callback);
			}
		};
		ListLoader<BaseListLoadResult<UnidadeDTO>> load = new BaseListLoader<BaseListLoadResult<UnidadeDTO>>(proxy);
		store = new ListStore<UnidadeDTO>(load);
		setDisplayField("codigo");
		setStore(store);
	}

}
