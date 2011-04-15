package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.shared.dtos.GrupoSalaDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class GrupoSalaComboBox extends ComboBox<GrupoSalaDTO> {

	private UnidadeComboBox unidadeComboBox;
	
	public GrupoSalaComboBox(UnidadeComboBox unidadeCB) {
		this.unidadeComboBox = unidadeCB;
		addListeners();
		
		RpcProxy<ListLoadResult<GrupoSalaDTO>> proxy = new RpcProxy<ListLoadResult<GrupoSalaDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<GrupoSalaDTO>> callback) {
				Services.gruposSalas().getListByUnidade(unidadeComboBox.getValue(), callback);
			}
		};
		
		setStore(new ListStore<GrupoSalaDTO>(new BaseListLoader<BaseListLoadResult<GrupoSalaDTO>>(proxy)));
		
		setDisplayField(GrupoSalaDTO.PROPERTY_NOME);
		setFieldLabel("Grupo de Sala");
		setEmptyText("Selecione o grupo de sala");
		setTemplate(getTemplateCB());
		setEditable(false);
		setEnabled(false);
		setTriggerAction(TriggerAction.ALL);
	}

	private void addListeners() {
		unidadeComboBox.addSelectionChangedListener(new SelectionChangedListener<UnidadeDTO>(){
			@Override
			public void selectionChanged(SelectionChangedEvent<UnidadeDTO> se) {
				final UnidadeDTO unidadeDTO = se.getSelectedItem();
				getStore().removeAll();
				setValue(null);
				setEnabled(unidadeDTO != null);
				if(unidadeDTO != null) {
					getStore().getLoader().load();
				}
			}
		});
	}
	
	private native String getTemplateCB() /*-{
		return [
			'<tpl for=".">',
			'<div class="x-combo-list-item">{nome} ({codigo})</div>',
			'</tpl>'
		].join("");
	}-*/;
	
}
