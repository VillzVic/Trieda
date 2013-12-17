package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.TitulacaoDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class TitulacaoComboBox extends ComboBox<TitulacaoDTO> {
	private CenarioDTO cenarioDTO;
	public TitulacaoComboBox( CenarioDTO cenario ) {
		this.cenarioDTO = cenario;
		final ListStore<TitulacaoDTO> listStore = new ListStore<TitulacaoDTO>();
		
		Services.professores().getTitulacoesAll(cenarioDTO, new AsyncCallback<ListLoadResult<TitulacaoDTO>>() {
			@Override
			public void onFailure(Throwable caught) {
				caught.printStackTrace();
			}
			@Override
			public void onSuccess(ListLoadResult<TitulacaoDTO> result) {
				listStore.add(result.getData());
			}
		});
		
		setStore(listStore);
		setFieldLabel("Titulação");
		setDisplayField(TitulacaoDTO.PROPERTY_NOME);
		setEditable(false);
		setTriggerAction(TriggerAction.ALL);
	}

}
