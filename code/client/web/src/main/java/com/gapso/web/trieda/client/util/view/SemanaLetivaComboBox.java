package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.client.mvp.model.SemanaLetivaDTO;
import com.gapso.web.trieda.client.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class SemanaLetivaComboBox extends ComboBox<SemanaLetivaDTO> {

	public SemanaLetivaComboBox() {
		RpcProxy<ListLoadResult<SemanaLetivaDTO>> proxy = new RpcProxy<ListLoadResult<SemanaLetivaDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<SemanaLetivaDTO>> callback) {
				Services.semanasLetiva().getList(callback);
			}
		};
		
		setStore(new ListStore<SemanaLetivaDTO>(new BaseListLoader<BaseListLoadResult<SemanaLetivaDTO>>(proxy)));
		
		setFieldLabel("Semana Letiva");
		setEmptyText("Selecione a semana letiva");
		setDisplayField(SemanaLetivaDTO.PROPERTY_CODIGO);
		setEditable(false);
		setTriggerAction(TriggerAction.ALL);
	}

}
