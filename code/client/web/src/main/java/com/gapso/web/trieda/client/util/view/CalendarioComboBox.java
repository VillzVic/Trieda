package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.ListLoader;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.client.mvp.model.CalendarioDTO;
import com.gapso.web.trieda.client.services.CalendariosServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class CalendarioComboBox extends ComboBox<CalendarioDTO> {

	private ListStore<CalendarioDTO> store;
	
	public CalendarioComboBox() {
		final CalendariosServiceAsync service = Services.calendarios();
		RpcProxy<ListLoadResult<CalendarioDTO>> proxy = new RpcProxy<ListLoadResult<CalendarioDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<CalendarioDTO>> callback) {
				service.getList(callback);
			}
		};
		ListLoader<BaseListLoadResult<CalendarioDTO>> load = new BaseListLoader<BaseListLoadResult<CalendarioDTO>>(proxy);
		store = new ListStore<CalendarioDTO>(load);
		setDisplayField("codigo");
		setStore(store);
	}

}
