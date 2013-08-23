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
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class ProfessorCpfComboBox extends ComboBox<ProfessorDTO>{

	private CenarioDTO cenarioDTO;
	
	public ProfessorCpfComboBox(CenarioDTO cenarioDTO) {
		this(cenarioDTO, false);
	}

	public ProfessorCpfComboBox(CenarioDTO cenarioDTO, boolean readOnly) {
		this.cenarioDTO = cenarioDTO;
		configureContentOfComboBox(readOnly, 0, 10);
		configureView(readOnly);
	}

	private void configureContentOfComboBox(boolean readOnly, final int offset, final int limit) {
		if(!readOnly){
			// Configura a store
			
			RpcProxy<ListLoadResult<ProfessorDTO>> proxy = new RpcProxy<ListLoadResult<ProfessorDTO>>() {
				// realiza o filtro dos valores do comboBox
				@Override
				public void load(Object loadConfig, AsyncCallback<ListLoadResult<ProfessorDTO>> callback){
					Services.professores().getAutoCompleteList(cenarioDTO, (BasePagingLoadConfig)loadConfig,  ProfessorDTO.PROPERTY_CPF, callback);
				}
			};
			
			ListLoader<BaseListLoadResult<ProfessorDTO>> listLoader = new BaseListLoader<BaseListLoadResult<ProfessorDTO>>(proxy);
			listLoader.addListener(Loader.BeforeLoad, new Listener<LoadEvent> () {
				public void handleEvent(LoadEvent be) {
					be.<ModelData>getConfig().set("offset",offset);
					be.<ModelData>getConfig().set("limit",limit);
				}
			});
			
			setStore(new ListStore<ProfessorDTO>(listLoader));
		}
		else{
			setStore(new ListStore<ProfessorDTO>());
		}
	}
	
	private void configureView(boolean readOnly) {
		setDisplayField(ProfessorDTO.PROPERTY_CPF);
		setFieldLabel("CPF");
		setEmptyText("Digite o cpf do professor");
		setTemplate(getTemplateCB());
		setReadOnly(readOnly);
		setHideTrigger(true);
		setTriggerAction(TriggerAction.QUERY);
		setMinChars(1);
	}

	private native String getTemplateCB() /*-{
		return  [
			'<tpl for=".">',
			'<div class="x-combo-list-item">{cpf} ({nome})</div>',
			'</tpl>'
		].join("");
	}-*/;
	
}

