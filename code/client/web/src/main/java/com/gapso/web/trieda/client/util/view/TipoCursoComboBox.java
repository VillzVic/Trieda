package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.client.mvp.model.TipoCursoDTO;
import com.gapso.web.trieda.client.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class TipoCursoComboBox extends ComboBox<TipoCursoDTO> {
	
	public TipoCursoComboBox() {
		RpcProxy<ListLoadResult<TipoCursoDTO>> proxy = new RpcProxy<ListLoadResult<TipoCursoDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<TipoCursoDTO>> callback) {
				Services.tiposCursos().getList(callback);
			}
		};
		
		setStore(new ListStore<TipoCursoDTO>(new BaseListLoader<BaseListLoadResult<TipoCursoDTO>>(proxy)));
		setFieldLabel("Tipo de Curso");
		setDisplayField(TipoCursoDTO.PROPERTY_CODIGO);
		setEditable(false);
		setTriggerAction(TriggerAction.ALL);
	}

}
