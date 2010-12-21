package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.UnidadeDTO;
import com.gapso.web.trieda.client.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class UnidadeComboBox extends ComboBox<UnidadeDTO> {

	private CampusComboBox campusComboBox;
	
	public UnidadeComboBox(CampusComboBox campusCB) {
		this.campusComboBox = campusCB;
		addListeners();
		
		RpcProxy<ListLoadResult<UnidadeDTO>> proxy = new RpcProxy<ListLoadResult<UnidadeDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<UnidadeDTO>> callback) {
				Services.unidades().getListByCampus(campusComboBox.getValue(), callback);
			}
		};
		
		setStore(new ListStore<UnidadeDTO>(new BaseListLoader<BaseListLoadResult<UnidadeDTO>>(proxy)));
		
		setDisplayField("nome");
		setFieldLabel("Unidade");
		setEmptyText("Selecione a unidade");
		setTemplate(getTemplateCB());
		setEditable(false);
		setEnabled(false);
		setTriggerAction(TriggerAction.ALL);
	}

	private void addListeners() {
		campusComboBox.addSelectionChangedListener(new SelectionChangedListener<CampusDTO>(){
			@Override
			public void selectionChanged(SelectionChangedEvent<CampusDTO> se) {
				final CampusDTO campusDTO = se.getSelectedItem();
				getStore().removeAll();
				setValue(null);
				setEnabled(campusDTO != null);
				if(campusDTO != null) {
					getStore().getLoader().load();
				}
			}
		});
	}
	
	private native String getTemplateCB() /*-{
		return [
			'<tpl for=".">',
			'<div class="x-combo-list-item">{codigo} ({nome})</div>',
			'</tpl>'
		].join("");
	}-*/;
	
}
