package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.ListLoader;
import com.extjs.gxt.ui.client.data.LoadEvent;
import com.extjs.gxt.ui.client.data.Loader;
import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.client.mvp.model.TipoDisciplinaDTO;
import com.gapso.web.trieda.client.services.DisciplinasServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class TipoDisciplinaComboBox extends ComboBox<TipoDisciplinaDTO> {

	private ListStore<TipoDisciplinaDTO> store;
	
	public TipoDisciplinaComboBox() {
		final DisciplinasServiceAsync service = Services.disciplinas();
		RpcProxy<ListLoadResult<TipoDisciplinaDTO>> proxy = new RpcProxy<ListLoadResult<TipoDisciplinaDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<TipoDisciplinaDTO>> callback) {
				service.getTipoDisciplinaList(callback);
			}
		};
		ListLoader<BaseListLoadResult<TipoDisciplinaDTO>> load = new BaseListLoader<BaseListLoadResult<TipoDisciplinaDTO>>(proxy);
		load.addListener(Loader.BeforeLoad, new Listener<LoadEvent>() {
			public void handleEvent(LoadEvent be) {
				be.<ModelData> getConfig().set("offset", 0);
				be.<ModelData> getConfig().set("limit", 10);
			}
		});
		store = new ListStore<TipoDisciplinaDTO>(load);
		setDisplayField("nome");
		setStore(store);
		setHideTrigger(true);  
		setTriggerAction(TriggerAction.QUERY);
		setTemplate(getTemplateCB());
		setMinChars(1);
	}

	private native String getTemplateCB() /*-{
		return  [
			'<tpl for=".">',
			'<div class="x-combo-list-item">{nome}</div>',
			'</tpl>'
		].join("");
	}-*/;
	
}
