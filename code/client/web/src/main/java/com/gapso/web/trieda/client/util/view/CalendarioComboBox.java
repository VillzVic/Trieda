package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.ListLoader;
import com.extjs.gxt.ui.client.data.LoadEvent;
import com.extjs.gxt.ui.client.data.Loader;
import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.Listener;
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
				service.getList((BasePagingLoadConfig)loadConfig, callback);
			}
		};
		ListLoader<BaseListLoadResult<CalendarioDTO>> load = new BaseListLoader<BaseListLoadResult<CalendarioDTO>>(proxy);
		load.addListener(Loader.BeforeLoad, new Listener<LoadEvent>() {
			public void handleEvent(LoadEvent be) {
				
				be.<ModelData> getConfig().set("offset", 0);
				be.<ModelData> getConfig().set("limit", 10);
			}
		});
		store = new ListStore<CalendarioDTO>(load);
		setDisplayField("codigo");
		setStore(store);
		setHideTrigger(true);  
		setTriggerAction(TriggerAction.QUERY);
		setTemplate(getTemplateCB());
	}

	private native String getTemplateCB() /*-{
	return  [
		'<tpl for=".">',
		'<div class="x-combo-list-item">{codigo}</div>',
		'</tpl>'
	].join("");
}-*/;
	
}
