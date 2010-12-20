package com.gapso.web.trieda.client.util.view;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.services.CampiServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class CampusComboBox extends ComboBox<CampusDTO> {

	public CampusComboBox() {
		final CampiServiceAsync service = Services.campi();
		
		service.getListAll(new AsyncCallback<ListLoadResult<CampusDTO>>() {
			@Override
			public void onFailure(Throwable caught) {
				caught.printStackTrace();
			}
			@Override
			public void onSuccess(ListLoadResult<CampusDTO> result) {
				populate(result.getData());
			}
		});
		
		setStore(new ListStore<CampusDTO>());
		
		setFieldLabel("Campus");
		setDisplayField("codigo");
		setTemplate(getTemplateCB());
		setEditable(false);
	}

	private void populate(List<CampusDTO> campusList) {
		getStore().removeAll();
		getStore().add(campusList);
	}
	
	private native String getTemplateCB() /*-{
		return  [
			'<tpl for=".">',
			'<div class="x-combo-list-item">{nome} ({codigo})</div>',
			'</tpl>'
		].join("");
	}-*/;
	
}
