package com.gapso.web.trieda.main.client.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.main.client.services.SalasServiceAsync;
import com.gapso.web.trieda.main.client.services.Services;
import com.gapso.web.trieda.shared.dtos.TipoSalaDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class TipoSalaComboBox extends ComboBox<TipoSalaDTO> {

	public TipoSalaComboBox() {
		final SalasServiceAsync service = Services.salas();
		RpcProxy<ListLoadResult<TipoSalaDTO>> proxy = new RpcProxy<ListLoadResult<TipoSalaDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<TipoSalaDTO>> callback) {
				service.getTipoSalaList(callback);
			}
		};
		setStore(new ListStore<TipoSalaDTO>(new BaseListLoader<BaseListLoadResult<TipoSalaDTO>>(proxy)));
		setFieldLabel("Tipo de Sala");
		setDisplayField(TipoSalaDTO.PROPERTY_NOME);
		setEditable(false);
	}

}
