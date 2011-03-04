package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.client.mvp.model.TitulacaoDTO;
import com.gapso.web.trieda.client.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class TitulacaoComboBox extends ComboBox<TitulacaoDTO> {
	
	public TitulacaoComboBox() {
		RpcProxy<ListLoadResult<TitulacaoDTO>> proxy = new RpcProxy<ListLoadResult<TitulacaoDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<TitulacaoDTO>> callback) {
				Services.professores().getTitulacoesAll(callback);
			}
		};
		
		setStore(new ListStore<TitulacaoDTO>(new BaseListLoader<BaseListLoadResult<TitulacaoDTO>>(proxy)));
		setFieldLabel("Titulação");
		setDisplayField(TitulacaoDTO.PROPERTY_NOME);
		setEditable(false);
		setTriggerAction(TriggerAction.ALL);
	}

}
