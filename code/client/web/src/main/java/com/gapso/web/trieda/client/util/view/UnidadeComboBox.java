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
import com.gapso.web.trieda.client.mvp.model.UnidadeDTO;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.services.UnidadesServiceAsync;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class UnidadeComboBox extends ComboBox<UnidadeDTO> {

	private ListStore<UnidadeDTO> store;
	private Long campusId;
	
	public UnidadeComboBox() {
		final UnidadesServiceAsync service = Services.unidades();
		RpcProxy<ListLoadResult<UnidadeDTO>> proxy = new RpcProxy<ListLoadResult<UnidadeDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<UnidadeDTO>> callback) {
				BasePagingLoadConfig bplc = (BasePagingLoadConfig)loadConfig;
				bplc.set("campusId", getCampusId());
				service.getList(bplc, callback);
			}
		};
		ListLoader<BaseListLoadResult<UnidadeDTO>> load = new BaseListLoader<BaseListLoadResult<UnidadeDTO>>(proxy);
		load.addListener(Loader.BeforeLoad, new Listener<LoadEvent>() {
			public void handleEvent(LoadEvent be) {
				be.<ModelData> getConfig().set("offset", 0);
				be.<ModelData> getConfig().set("limit", 10);
			}
		});
		store = new ListStore<UnidadeDTO>(load);
		setDisplayField("nome");
		setFieldLabel("Unidade");
		setStore(store);
		setHideTrigger(true);  
		setTriggerAction(TriggerAction.QUERY);
		setTemplate(getTemplateCB());
		setMinChars(1);
	}

	public Long getCampusId() {
		return campusId;
	}

	public void setCampusId(Long campusId) {
		this.campusId = campusId;
	}

	private native String getTemplateCB() /*-{
		return [
			'<tpl for=".">',
			'<div class="x-combo-list-item">{nome} ({codigo})</div>',
			'</tpl>'
		].join("");
	}-*/;
	
}
