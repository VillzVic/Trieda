package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.services.Services;
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
		
		setDisplayField("codigo");
		setFieldLabel("Campus");
		setEmptyText("Selecione o campus");
		setTemplate(getTemplateCB());
		setEditable(false);
		setTriggerAction(TriggerAction.ALL);
	}

	private native String getTemplateCB() /*-{
		return  [
			'<tpl for=".">',
			'<div class="x-combo-list-item">{nome} ({codigo})</div>',
			'</tpl>'
		].join("");
	}-*/;
	
}
