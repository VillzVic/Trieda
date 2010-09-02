package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.ListLoader;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.client.mvp.model.TipoSalaDTO;
import com.gapso.web.trieda.client.services.SalasServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class TipoSalaComboBox extends ComboBox<TipoSalaDTO> {

	private ListStore<TipoSalaDTO> store;
	
	public TipoSalaComboBox() {
		final SalasServiceAsync service = Services.salas();
		RpcProxy<ListLoadResult<TipoSalaDTO>> proxy = new RpcProxy<ListLoadResult<TipoSalaDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<TipoSalaDTO>> callback) {
				service.getTipoSalaList(callback);
			}
		};
		ListLoader<BaseListLoadResult<TipoSalaDTO>> load = new BaseListLoader<BaseListLoadResult<TipoSalaDTO>>(proxy);
		store = new ListStore<TipoSalaDTO>(load);
		setDisplayField("nome");
		setStore(store);
	}

}
