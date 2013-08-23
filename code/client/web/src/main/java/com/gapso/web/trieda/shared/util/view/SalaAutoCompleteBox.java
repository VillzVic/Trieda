package com.gapso.web.trieda.shared.util.view;

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
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class SalaAutoCompleteBox extends ComboBox<SalaDTO> {
	
	private CenarioDTO cenarioDTO;
	
	public SalaAutoCompleteBox(CenarioDTO cenarioDTO) {
		this(cenarioDTO, false);
	}

	public SalaAutoCompleteBox(CenarioDTO cenarioDTO, boolean readOnly) {
		this.cenarioDTO = cenarioDTO;
		configureContentOfComboBox(readOnly, 0, 10);
		configureView(readOnly);
	}

	private void configureContentOfComboBox(boolean readOnly, final int offset, final int limit) {
		if(!readOnly){
			// Configura a store
			
			RpcProxy<ListLoadResult<SalaDTO>> proxy = new RpcProxy<ListLoadResult<SalaDTO>>() {
				// realiza o filtro dos valores do comboBox
				@Override
				public void load(Object loadConfig, AsyncCallback<ListLoadResult<SalaDTO>> callback){
					Services.salas().getAutoCompleteList(cenarioDTO, (BasePagingLoadConfig)loadConfig, callback);
				}
			};
			
			ListLoader<BaseListLoadResult<SalaDTO>> listLoader = new BaseListLoader<BaseListLoadResult<SalaDTO>>(proxy);
			listLoader.addListener(Loader.BeforeLoad, new Listener<LoadEvent> () {
				public void handleEvent(LoadEvent be) {
					be.<ModelData>getConfig().set("offset",offset);
					be.<ModelData>getConfig().set("limit",limit);
				}
			});
			
			setStore(new ListStore<SalaDTO>(listLoader));
		}
		else{
			setStore(new ListStore<SalaDTO>());
		}
	}
	
	private void configureView(boolean readOnly) {
		setDisplayField(SalaDTO.PROPERTY_CODIGO);
		setFieldLabel("Sala");
		setEmptyText("Selecione uma sala");
		setTemplate(getTemplateCB());
		setReadOnly(readOnly);
		setHideTrigger(true);
		setTriggerAction(TriggerAction.QUERY);
		setMinChars(1);
	}

	private native String getTemplateCB() /*-{
		return  [
			'<tpl for=".">',
			'<div class="x-combo-list-item">{codigo} ({numero})</div>',
			'</tpl>'
		].join("");
	}-*/;
}
