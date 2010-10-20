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
import com.gapso.web.trieda.client.mvp.model.TipoCursoDTO;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.services.TiposCursosServiceAsync;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class TipoCursoComboBox extends ComboBox<TipoCursoDTO> {

	private ListStore<TipoCursoDTO> store;
	
	public TipoCursoComboBox() {
		final TiposCursosServiceAsync service = Services.tiposCursos();
		RpcProxy<ListLoadResult<TipoCursoDTO>> proxy = new RpcProxy<ListLoadResult<TipoCursoDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<TipoCursoDTO>> callback) {
				service.getList((BasePagingLoadConfig)loadConfig, callback);
			}
		};
		ListLoader<BaseListLoadResult<TipoCursoDTO>> load = new BaseListLoader<BaseListLoadResult<TipoCursoDTO>>(proxy);
		load.addListener(Loader.BeforeLoad, new Listener<LoadEvent>() {
			public void handleEvent(LoadEvent be) {
				be.<ModelData> getConfig().set("offset", 0);
				be.<ModelData> getConfig().set("limit", 10);
			}
		});
		store = new ListStore<TipoCursoDTO>(load);
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
			'<div class="x-combo-list-item">{codigo}</div>',
			'</tpl>'
		].join("");
	}-*/;
	
}
