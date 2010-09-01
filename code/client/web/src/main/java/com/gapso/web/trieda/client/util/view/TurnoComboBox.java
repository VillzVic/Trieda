package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.ListLoader;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.services.TurnosServiceAsync;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class TurnoComboBox extends ComboBox<TurnoDTO> {

	private ListStore<TurnoDTO> store;
	
	public TurnoComboBox() {
		final TurnosServiceAsync service = Services.turnos();
		RpcProxy<ListLoadResult<TurnoDTO>> proxy = new RpcProxy<ListLoadResult<TurnoDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<TurnoDTO>> callback) {
				service.getList(callback);
			}
		};
		ListLoader<BaseListLoadResult<TurnoDTO>> load = new BaseListLoader<BaseListLoadResult<TurnoDTO>>(proxy);
		store = new ListStore<TurnoDTO>(load);
		setDisplayField("nome");
		setStore(store);
	}

}
