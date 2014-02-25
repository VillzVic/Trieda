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
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.shared.dtos.AbstractDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class DisciplinaAutoCompleteBox extends ComboBox<DisciplinaDTO> {
	ComboBox comboDep;
	CenarioDTO cenarioDTO;
	
	public DisciplinaAutoCompleteBox(CenarioDTO cenarioDTO) {
		this(cenarioDTO, false);
	}

	public DisciplinaAutoCompleteBox(CenarioDTO cenarioDTO, boolean readOnly) {
		this.cenarioDTO = cenarioDTO;
		comboDep = null;
		configureContentOfComboBox(readOnly, 0, 10);
		configureView(readOnly);
	}
	
	/* Declaracao que indica que a instancia tem seu filtro
	 * com criterios baseados nos valores de um outro ComboBox
	 */
	public DisciplinaAutoCompleteBox(CenarioDTO cenarioDTO, ComboBox combo) {
		this.cenarioDTO = cenarioDTO;
		comboDep = combo;
		configureContentOfComboBox(false, 0, 10);
		configureView(false);
	}
	
	/*
	 * Esse metodo deve ser sobreescrito sempre que houver necessidade de criar uma DisciplinaComboBox
	 * que possua dependencia de outro comboBox. Vale ressaltar que essa dependencia esta definida
	 * atraves do campo comboDep. 
	 */
	public void loadByCriteria(AbstractDTO abdto, AsyncCallback<ListLoadResult<DisciplinaDTO>> callback){}
	
	@SuppressWarnings("unchecked")
	private void configureContentOfComboBox(boolean readOnly, final int offset, final int limit) {
		if(!readOnly){
			// Configura a store
			
			RpcProxy<ListLoadResult<DisciplinaDTO>> proxy = new RpcProxy<ListLoadResult<DisciplinaDTO>>() {
				// realiza o filtro dos valores do comboBox
				@Override
				public void load(Object loadConfig, AsyncCallback<ListLoadResult<DisciplinaDTO>> callback){
					// se houver um comboDep, chama a funcao que ira filtrar de acordo com os valores
					// dessa dependencia. Caso contrario, filtra de acordo com os valores digitados para
					// o filtro.
					if((comboDep != null) && (!comboDep.getSelection().isEmpty())){
						loadByCriteria((AbstractDTO) comboDep.getSelection().get(0), callback);
					}
					else{
						Services.disciplinas().getAutoCompleteList(cenarioDTO, (BasePagingLoadConfig)loadConfig, callback);
					}
				}
			};
			
			ListLoader<BaseListLoadResult<DisciplinaDTO>> listLoader = new BaseListLoader<BaseListLoadResult<DisciplinaDTO>>(proxy);
			listLoader.addListener(Loader.BeforeLoad, new Listener<LoadEvent> () {
				public void handleEvent(LoadEvent be) {
					be.<ModelData>getConfig().set("offset",offset);
					be.<ModelData>getConfig().set("limit",limit);
				}
			});
			
			setStore(new ListStore<DisciplinaDTO>(listLoader));
			
			if(comboDep != null){
				comboDep.addSelectionChangedListener(new SelectionChangedListener<AbstractDTO>() {
					@Override
					public void selectionChanged(SelectionChangedEvent<AbstractDTO> se) {
						final AbstractDTO abDTO = se.getSelectedItem();
						if(abDTO != null){
							getStore().removeAll();
							setValue(null);
//							getStore().getLoader().load();
						}
					}
				});
			}
		}
		else{
			setStore(new ListStore<DisciplinaDTO>());
		}
	}
	
	private void configureView(boolean readOnly) {
		setDisplayField(DisciplinaDTO.PROPERTY_CODIGO);
		setFieldLabel("Disciplina");
		setEmptyText("Selecione uma disciplina");
		setTemplate(getTemplateCB());
		setReadOnly(readOnly);
		setTriggerAction(TriggerAction.QUERY);
		setMinChars(1);
	}

	private native String getTemplateCB() /*-{
		return  [
			'<tpl for=".">',
			'<div class="x-combo-list-item">{nome} ({codigo})</div>',
			'</tpl>'
		].join("");
	}-*/;
}
