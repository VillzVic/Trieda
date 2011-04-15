package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.services.Services;
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
		
		setDisplayField(UnidadeDTO.PROPERTY_DISPLAY_TEXT);
		setFieldLabel("Unidade");
		setEmptyText("Selecione a unidade");
		setSimpleTemplate("{"+UnidadeDTO.PROPERTY_NOME+"} ({"+UnidadeDTO.PROPERTY_CODIGO+"})");
		setEditable(false);
		setEnabled(campusComboBox.getValue() != null);
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
	
}
