package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.shared.dtos.TipoContratoDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class TipoContratoComboBox extends ComboBox<TipoContratoDTO> {
	
	public TipoContratoComboBox() {
		RpcProxy<ListLoadResult<TipoContratoDTO>> proxy = new RpcProxy<ListLoadResult<TipoContratoDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<TipoContratoDTO>> callback) {
				Services.professores().getTiposContratoAll(callback);
			}
		};
		
		setStore(new ListStore<TipoContratoDTO>(new BaseListLoader<BaseListLoadResult<TipoContratoDTO>>(proxy)));
		setFieldLabel("Tipo de Contrato");
		setDisplayField(TipoContratoDTO.PROPERTY_NOME);
		setEmptyText("Selecione tipo de contrato");
		setEditable(false);
		setTriggerAction(TriggerAction.ALL);
	}

}
