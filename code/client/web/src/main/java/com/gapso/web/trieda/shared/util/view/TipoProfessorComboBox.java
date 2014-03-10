package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.store.StoreEvent;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.shared.dtos.TipoProfessorDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class TipoProfessorComboBox extends ComboBox<TipoProfessorDTO> {
	
	public TipoProfessorComboBox() {
		
		final ListStore<TipoProfessorDTO> listStore = new ListStore<TipoProfessorDTO>();
		
		Services.professores().getTiposProfessor(new AsyncCallback<ListLoadResult<TipoProfessorDTO>>() {
			@Override
			public void onFailure(Throwable caught) {
				caught.printStackTrace();
			}
			@Override
			public void onSuccess(ListLoadResult<TipoProfessorDTO> result) {
				listStore.add(result.getData());
			}
		});
		
		setStore(listStore);
		setFieldLabel("Professor");
		setDisplayField(TipoProfessorDTO.PROPERTY_NOME);
		setEditable(false);
		setTriggerAction(TriggerAction.ALL);
	}

    @Override
    public void onLoad(StoreEvent<TipoProfessorDTO> se) {
        super.onLoad(se);
        if(getStore().getModels().isEmpty())
        {
			MessageBox.alert("Aviso!","Não existem Tipos de Professor cadastrados", null);
        }
    }
}