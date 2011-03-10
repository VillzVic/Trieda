package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
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
		
		setDisplayField(ProfessorDTO.PROPERTY_NOME);
		setFieldLabel("Professor");
		setEmptyText("Selecione o professor");
		setSimpleTemplate("{"+ProfessorDTO.PROPERTY_NOME+"} ({"+ProfessorDTO.PROPERTY_CPF+"})");
		setEditable(false);
		setTriggerAction(TriggerAction.ALL);
	}
}