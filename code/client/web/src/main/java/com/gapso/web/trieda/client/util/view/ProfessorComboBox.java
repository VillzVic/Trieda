package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.client.mvp.model.ProfessorDTO;
import com.gapso.web.trieda.client.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class ProfessorComboBox extends ComboBox<ProfessorDTO> {

	public ProfessorComboBox() {
		RpcProxy<ListLoadResult<ProfessorDTO>> proxy = new RpcProxy<ListLoadResult<ProfessorDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<ProfessorDTO>> callback) {
				Services.professores().getList(callback);
			}
		};
		
		setStore(new ListStore<ProfessorDTO>(new BaseListLoader<BaseListLoadResult<ProfessorDTO>>(proxy)));
		
		setDisplayField("nome");
		setFieldLabel("Professor");
		setEmptyText("Selecione o professor");
		setSimpleTemplate("{nome} ({cpf})");
		setEditable(false);
		setTriggerAction(TriggerAction.ALL);
	}
	
}
