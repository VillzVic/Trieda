package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class CampusComboBox extends ComboBox<CampusDTO> {

	public CampusComboBox() {
		RpcProxy<ListLoadResult<CampusDTO>> proxy = new RpcProxy<ListLoadResult<CampusDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<CampusDTO>> callback) {
				Services.campi().getListAll(callback);
			}
		};
		
		setStore(new ListStore<CampusDTO>(new BaseListLoader<BaseListLoadResult<CampusDTO>>(proxy)));
		
		setDisplayField(CampusDTO.PROPERTY_CODIGO);
		setFieldLabel("Campus");
		setEmptyText("Selecione o campus");
		setSimpleTemplate("{"+CampusDTO.PROPERTY_NOME+"} ({"+CampusDTO.PROPERTY_CODIGO+"})");
		setEditable(false);
		setTriggerAction(TriggerAction.ALL);
	}
	
}
