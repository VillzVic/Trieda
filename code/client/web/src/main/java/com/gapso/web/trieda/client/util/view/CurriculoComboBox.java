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
import com.gapso.web.trieda.client.mvp.model.CurriculoDTO;
import com.gapso.web.trieda.client.services.CurriculosServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class CurriculoComboBox extends ComboBox<CurriculoDTO> {

	private ListStore<CurriculoDTO> store;
	
	public CurriculoComboBox() {
		final CurriculosServiceAsync service = Services.curriculos();
		RpcProxy<ListLoadResult<CurriculoDTO>> proxy = new RpcProxy<ListLoadResult<CurriculoDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<CurriculoDTO>> callback) {
				service.getList((BasePagingLoadConfig)loadConfig, callback);
			}
		};
		ListLoader<BaseListLoadResult<CurriculoDTO>> load = new BaseListLoader<BaseListLoadResult<CurriculoDTO>>(proxy);
		load.addListener(Loader.BeforeLoad, new Listener<LoadEvent>() {
			public void handleEvent(LoadEvent be) {
				be.<ModelData> getConfig().set("offset", 0);
				be.<ModelData> getConfig().set("limit", 10);
			}
		});
		store = new ListStore<CurriculoDTO>(load);
		setFieldLabel("Matriz Curricular");
		setDisplayField("codigo");
		setStore(store);
		setHideTrigger(true);  
		setTriggerAction(TriggerAction.QUERY);
		setTemplate(getTemplateCB());
		setMinChars(1);
	}

	private native String getTemplateCB() /*-{
		return  [
			'<tpl for=".">',
			'<div class="x-combo-list-item">{codigo} ({cursoString})</div>',
			'</tpl>'
		].join("");
	}-*/;
	
}
