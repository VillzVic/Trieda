package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.store.StoreEvent;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.TipoDisciplinaDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class TipoDisciplinaComboBox extends ComboBox<TipoDisciplinaDTO> {

	private CenarioDTO cenarioDTO;
	
	public TipoDisciplinaComboBox( CenarioDTO cenario ) {
		this.cenarioDTO = cenario;
		final ListStore<TipoDisciplinaDTO> listStore = new ListStore<TipoDisciplinaDTO>();
		
		Services.disciplinas().getTipoDisciplinaList(cenarioDTO, new AsyncCallback<ListLoadResult<TipoDisciplinaDTO>>() {
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
	
    @Override
    public void onLoad(StoreEvent<TipoDisciplinaDTO> se) {
        super.onLoad(se);
        if(getStore().getModels().isEmpty())
        {
			MessageBox.alert("Aviso!","Não existem Tipos de Disciplina cadastrados", null);
        }
    }
}
