package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.ListLoader;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.services.CampiServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class CampusComboBox extends ComboBox<CampusDTO> {

	private ListStore<CampusDTO> store;
	
	public CampusComboBox() {
		final CampiServiceAsync service = Services.campi();
		RpcProxy<ListLoadResult<CampusDTO>> proxy = new RpcProxy<ListLoadResult<CampusDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<CampusDTO>> callback) {
				service.getList(callback);
			}
		};
		ListLoader<BaseListLoadResult<CampusDTO>> load = new BaseListLoader<BaseListLoadResult<CampusDTO>>(proxy);
		store = new ListStore<CampusDTO>(load);
		setDisplayField("codigo");
		setStore(store);
	}

}
