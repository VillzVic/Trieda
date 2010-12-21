package com.gapso.web.trieda.client.util.view;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.client.mvp.model.TipoDisciplinaDTO;
import com.gapso.web.trieda.client.services.DisciplinasServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class TipoDisciplinaComboBox extends ComboBox<TipoDisciplinaDTO> {

	public TipoDisciplinaComboBox() {
		
		final DisciplinasServiceAsync service = Services.disciplinas();
		service.getTipoDisciplinaList(new AsyncCallback<ListLoadResult<TipoDisciplinaDTO>>() {
			@Override
			public void onFailure(Throwable caught) {
				caught.printStackTrace();
			}
			@Override
			public void onSuccess(ListLoadResult<TipoDisciplinaDTO> result) {
				populate(result.getData());
			}
		});
		
		setStore(new ListStore<TipoDisciplinaDTO>());
		setFieldLabel("Tipo de Disciplina");
		setDisplayField("nome");
		setTemplate(getTemplateCB());
		setEditable(false);
	}

	private void populate(List<TipoDisciplinaDTO> list) {
		getStore().removeAll();
		getStore().add(list);
	}
	
	private native String getTemplateCB() /*-{
		return  [
			'<tpl for=".">',
			'<div class="x-combo-list-item">{nome}</div>',
			'</tpl>'
		].join("");
	}-*/;
	
}
