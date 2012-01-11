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
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.OfertaDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class DisciplinaComboBox extends ComboBox<DisciplinaDTO> {
	public DisciplinaComboBox() {
		this(false);
	}

	public DisciplinaComboBox(boolean readOnly) {
		configureContentOfComboBox(readOnly,0,10,null);
		configureView(readOnly);
	}
	
	public DisciplinaComboBox(OfertaComboBox ofertaCB) {
		configureContentOfComboBox(false,0,10,ofertaCB);
		configureView(false);
	}
	
	private void configureContentOfComboBox(boolean readOnly, final int offset, final int limit, OfertaComboBox ofertaCB) {
		if (!readOnly || (ofertaCB != null)) {
			// Configura a store
			final OfertaComboBox finalOfertaCB = ofertaCB;
			RpcProxy<ListLoadResult<DisciplinaDTO>> proxy = new RpcProxy<ListLoadResult<DisciplinaDTO>>() {
				@Override
				public void load(Object loadConfig, AsyncCallback<ListLoadResult<DisciplinaDTO>> callback) {
					if ((finalOfertaCB != null) && (!finalOfertaCB.getSelection().isEmpty())) {
						OfertaDTO ofertaDTO = finalOfertaCB.getSelection().get(0);
						Services.disciplinas().getListByCurriculo(ofertaDTO.getMatrizCurricularId(),callback);
					} else {
						Services.disciplinas().getList((BasePagingLoadConfig)loadConfig,callback);
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
			
			// Configura o listener para o componente externo
			if (ofertaCB != null) {
				ofertaCB.addSelectionChangedListener(new SelectionChangedListener<OfertaDTO>() {
					@Override
					public void selectionChanged(SelectionChangedEvent<OfertaDTO> se) {
						final OfertaDTO ofertaDTO = se.getSelectedItem();
						if (ofertaDTO != null) {
							getStore().removeAll();
							setValue(null);
							getStore().getLoader().load();
						}
					}
				});
			}
		} else {
			setStore(new ListStore<DisciplinaDTO>());
		}
	}
	
	private void configureView(boolean readOnly) {
		setDisplayField(DisciplinaDTO.PROPERTY_CODIGO);
		setFieldLabel("Disciplina");
		setEmptyText("Selecione uma disciplina");
		setTemplate(getTemplateCB());
		setReadOnly(readOnly);
		setHideTrigger(true);
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
