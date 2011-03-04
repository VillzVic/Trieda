package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.shared.dtos.TipoDisciplinaDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class TipoDisciplinaComboBox extends ComboBox<TipoDisciplinaDTO> {

	public TipoDisciplinaComboBox() {
		RpcProxy<ListLoadResult<TipoDisciplinaDTO>> proxy = new RpcProxy<ListLoadResult<TipoDisciplinaDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<TipoDisciplinaDTO>> callback) {
				Services.disciplinas().getTipoDisciplinaList(callback);
			}
		};
		
		setStore(new ListStore<TipoDisciplinaDTO>(new BaseListLoader<BaseListLoadResult<TipoDisciplinaDTO>>(proxy)));
		
		setFieldLabel("Tipo de Disciplina");
		setEmptyText("Selecione o tipo de disciplina");
		setDisplayField(TipoDisciplinaDTO.PROPERTY_NOME);
		setEditable(false);
		setTriggerAction(TriggerAction.ALL);
	}
	
}
