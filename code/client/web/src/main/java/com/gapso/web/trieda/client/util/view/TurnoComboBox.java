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
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.services.TurnosServiceAsync;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class TurnoComboBox extends ComboBox<TurnoDTO> {

	private ListStore<TurnoDTO> store;
	private CampusDTO campusDTO;
	
	public TurnoComboBox() {
		final TurnosServiceAsync service = Services.turnos();
		RpcProxy<ListLoadResult<TurnoDTO>> proxy = new RpcProxy<ListLoadResult<TurnoDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<TurnoDTO>> callback) {
				BasePagingLoadConfig bplc = (BasePagingLoadConfig)loadConfig;
				bplc.set("campusDTO", getCampusDTO());
				service.getList((BasePagingLoadConfig)loadConfig, callback);
			}
		};
		ListLoader<BaseListLoadResult<TurnoDTO>> load = new BaseListLoader<BaseListLoadResult<TurnoDTO>>(proxy);
		load.addListener(Loader.BeforeLoad, new Listener<LoadEvent>() {
			public void handleEvent(LoadEvent be) {
				
				be.<ModelData> getConfig().set("offset", 0);
				be.<ModelData> getConfig().set("limit", 10);
			}
		});
		store = new ListStore<TurnoDTO>(load);
		setDisplayField("nome");
		setStore(store);
		setHideTrigger(true);  
		setTriggerAction(TriggerAction.QUERY);
		setTemplate(getTemplateCB());
		setMinChars(1);
	}

	public CampusDTO getCampusDTO() {
		return campusDTO;
	}
	public void setCampusDTO(CampusDTO campusDTO) {
		this.campusDTO = campusDTO;
	}

	private native String getTemplateCB() /*-{
		return  [
			'<tpl for=".">',
			'<div class="x-combo-list-item">{nome} ({tempo}min)</div>',
			'</tpl>'
		].join("");
	}-*/;
}
