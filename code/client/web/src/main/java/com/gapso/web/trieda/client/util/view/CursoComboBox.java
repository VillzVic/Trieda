package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class CursoComboBox extends ComboBox<CursoDTO> {

	public CursoComboBox() {
		RpcProxy<ListLoadResult<CursoDTO>> proxy = new RpcProxy<ListLoadResult<CursoDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<CursoDTO>> callback) {
				Services.cursos().getListAll(callback);
			}
		};
		
		setStore(new ListStore<CursoDTO>(new BaseListLoader<BaseListLoadResult<CursoDTO>>(proxy)));
		
		setDisplayField(CursoDTO.PROPERTY_DISPLAY_TEXT);
		setFieldLabel("Curso");
		setEmptyText("Selecione o curso");
		setSimpleTemplate("{"+CursoDTO.PROPERTY_NOME+"} ({"+CursoDTO.PROPERTY_CODIGO+"})");
		setEditable(false);
		setTriggerAction(TriggerAction.ALL);
	}
}