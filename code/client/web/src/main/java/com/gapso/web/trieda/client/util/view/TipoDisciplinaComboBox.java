package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.shared.dtos.TipoDisciplinaDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class TipoDisciplinaComboBox extends ComboBox<TipoDisciplinaDTO> {

	public TipoDisciplinaComboBox() {
		
		final ListStore<TipoDisciplinaDTO> listStore = new ListStore<TipoDisciplinaDTO>();
		
		Services.disciplinas().getTipoDisciplinaList(new AsyncCallback<ListLoadResult<TipoDisciplinaDTO>>() {
			@Override
			public void onFailure(Throwable caught) {
				caught.printStackTrace();
			}
			@Override
			public void onSuccess(ListLoadResult<TipoDisciplinaDTO> result) {
				listStore.add(result.getData());
			}
		});
		
		setStore(listStore);
		
		setFieldLabel("Tipo de Disciplina");
		setEmptyText("Selecione o tipo de disciplina");
		setDisplayField(TipoDisciplinaDTO.PROPERTY_NOME);
		setEditable(false);
	}
	
}
