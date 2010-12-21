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
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.gapso.web.trieda.client.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class TurnoComboBox extends ComboBox<TurnoDTO> {

	private CampusComboBox campusComboBox;
	
	public TurnoComboBox() {
		this(null);
	}
	public TurnoComboBox(CampusComboBox campusCB) {
		this.campusComboBox = campusCB;
		
		RpcProxy<ListLoadResult<TurnoDTO>> proxy = new RpcProxy<ListLoadResult<TurnoDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<TurnoDTO>> callback) {
				if(campusComboBox != null) {
					Services.turnos().getListByCampus(campusComboBox.getValue(), callback);
				} else {
					Services.turnos().getList(callback);
				}
			}
		};
		
		setStore(new ListStore<TurnoDTO>(new BaseListLoader<BaseListLoadResult<TurnoDTO>>(proxy)));
		
		if(campusComboBox != null) {
			setEnabled(false);
			addListeners();
		}
		
		setDisplayField("nome");
		setFieldLabel("Turno");
		setEmptyText("Selecione o turno");
		setSimpleTemplate("{nome} ({tempo}min)");
		setEditable(false);
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
