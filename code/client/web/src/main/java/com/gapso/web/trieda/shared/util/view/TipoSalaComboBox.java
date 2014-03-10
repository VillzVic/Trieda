package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.store.StoreEvent;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.TipoSalaDTO;
import com.gapso.web.trieda.shared.services.SalasServiceAsync;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class TipoSalaComboBox extends ComboBox<TipoSalaDTO> {
	private CenarioDTO cenarioDTO;
	public TipoSalaComboBox(CenarioDTO cenario) {
		this.cenarioDTO = cenario;
		final SalasServiceAsync service = Services.salas();
		RpcProxy<ListLoadResult<TipoSalaDTO>> proxy = new RpcProxy<ListLoadResult<TipoSalaDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<TipoSalaDTO>> callback) {
				service.getTipoSalaList(cenarioDTO, callback);
			}
		};
		setStore(new ListStore<TipoSalaDTO>(new BaseListLoader<BaseListLoadResult<TipoSalaDTO>>(proxy)));
		setFieldLabel("Tipo de Sala");
		setDisplayField(TipoSalaDTO.PROPERTY_NOME);
		setEditable(false);
	}

    @Override
    public void onLoad(StoreEvent<TipoSalaDTO> se) {
        super.onLoad(se);
        if(getStore().getModels().isEmpty())
        {
			MessageBox.alert("Aviso!","Não existem Tipos de Sala cadastrados", null);
        }
    }
}
