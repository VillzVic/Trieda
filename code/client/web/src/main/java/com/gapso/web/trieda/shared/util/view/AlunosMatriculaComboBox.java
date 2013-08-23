package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.ListLoader;
import com.extjs.gxt.ui.client.data.LoadEvent;
import com.extjs.gxt.ui.client.data.Loader;
import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.shared.dtos.AlunoDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class AlunosMatriculaComboBox	extends ComboBox<AlunoDTO>{
	private ListStore<AlunoDTO> store;
	private CenarioDTO cenarioDTO;
	
	public AlunosMatriculaComboBox(CenarioDTO cenarioDTO){
		this(cenarioDTO, false);
	}

	public AlunosMatriculaComboBox(CenarioDTO cenarioDTO, boolean readOnly) {
		this.cenarioDTO = cenarioDTO;
		configureContentOfComboBox(readOnly, 0, 10);
		configureView(readOnly);
	}

	private void configureContentOfComboBox(boolean readOnly, final int offset, final int limit) {
		if(!readOnly){
			// Configura a store
			
			RpcProxy<ListLoadResult<AlunoDTO>> proxy = new RpcProxy<ListLoadResult<AlunoDTO>>() {
				// realiza o filtro dos valores do comboBox
				@Override
				public void load(Object loadConfig, AsyncCallback<ListLoadResult<AlunoDTO>> callback){
					Services.alunos().getAutoCompleteList(cenarioDTO, (BasePagingLoadConfig)loadConfig, AlunoDTO.PROPERTY_ALUNO_MATRICULA, callback);
				}
			};
			
			ListLoader<BaseListLoadResult<SalaDTO>> listLoader = new BaseListLoader<BaseListLoadResult<SalaDTO>>(proxy);
			listLoader.addListener(Loader.BeforeLoad, new Listener<LoadEvent> () {
				public void handleEvent(LoadEvent be) {
					be.<ModelData>getConfig().set("offset",offset);
					be.<ModelData>getConfig().set("limit",limit);
				}
			});
			
			setStore(new ListStore<AlunoDTO>(listLoader));
		}
		else{
			setStore(new ListStore<AlunoDTO>());
		}
	}
	
	private void configureView(boolean readOnly) {
		setDisplayField(AlunoDTO.PROPERTY_ALUNO_MATRICULA);
		setFieldLabel("Matricula");
		setEmptyText("Digite o número da matricula");
		setTemplate(getTemplateCB());
		setReadOnly(readOnly);
		setHideTrigger(true);
		setTriggerAction(TriggerAction.QUERY);
		setMinChars(1);
	}

	private native String getTemplateCB() /*-{
		return  [
			'<tpl for=".">',
			'<div class="x-combo-list-item">{matricula} ({nome})</div>',
			'</tpl>'
		].join("");
	}-*/;

}
