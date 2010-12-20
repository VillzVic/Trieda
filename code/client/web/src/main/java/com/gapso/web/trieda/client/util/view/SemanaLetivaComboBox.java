package com.gapso.web.trieda.client.util.view;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.client.mvp.model.SemanaLetivaDTO;
import com.gapso.web.trieda.client.services.SemanasLetivaServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class SemanaLetivaComboBox extends ComboBox<SemanaLetivaDTO> {

	public SemanaLetivaComboBox() {
		final SemanasLetivaServiceAsync service = Services.semanasLetiva();
		service.getList(new AsyncCallback<ListLoadResult<SemanaLetivaDTO>>() {
			@Override
			public void onFailure(Throwable caught) {
				caught.printStackTrace();
			}
			@Override
			public void onSuccess(ListLoadResult<SemanaLetivaDTO> result) {
				populate(result.getData());
			}
		});

		setStore(new ListStore<SemanaLetivaDTO>());
		
		setFieldLabel("Semana Letíva");
		setDisplayField("codigo");
		setTemplate(getTemplateCB());
		setEditable(false);
	}

	private void populate(List<SemanaLetivaDTO> list) {
		getStore().removeAll();
		getStore().add(list);
	}
	
	private native String getTemplateCB() /*-{
	return  [
		'<tpl for=".">',
		'<div class="x-combo-list-item">{codigo}</div>',
		'</tpl>'
	].join("");
}-*/;
	
}
