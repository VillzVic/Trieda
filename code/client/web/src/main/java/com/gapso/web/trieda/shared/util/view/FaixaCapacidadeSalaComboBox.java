package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.FaixaCapacidadeSalaDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class FaixaCapacidadeSalaComboBox extends ComboBox<FaixaCapacidadeSalaDTO> {
	
	public FaixaCapacidadeSalaComboBox(CenarioDTO cenarioDTO) {
		
		final ListStore<FaixaCapacidadeSalaDTO> listStore = new ListStore<FaixaCapacidadeSalaDTO>();
		
		Services.salas().getFaixasCapacidadeSala(cenarioDTO, 10, new AsyncCallback<ListLoadResult<FaixaCapacidadeSalaDTO>>() {
			@Override
			public void onFailure(Throwable caught) {
				caught.printStackTrace();
			}
			@Override
			public void onSuccess(ListLoadResult<FaixaCapacidadeSalaDTO> result) {
				listStore.add(result.getData());
			}
		});
		
		setStore(listStore);
		setFieldLabel("Capacidade");
		setDisplayField(FaixaCapacidadeSalaDTO.PROPERTY_NOME);
		setEditable(false);
		setTriggerAction(TriggerAction.ALL);
	}

}