package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.store.StoreEvent;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class OtimizacaoDisciplinasComboBox	extends ComboBox<DisciplinaDTO>
	{
		private OtimizacaoProfessorComboBox professorComboBox;
		
		public OtimizacaoDisciplinasComboBox(OtimizacaoProfessorComboBox professorCB) {
			this.professorComboBox = professorCB;
			addListeners();
			
			RpcProxy<ListLoadResult<DisciplinaDTO>> proxy = new RpcProxy<ListLoadResult<DisciplinaDTO>>() {
				@Override
				public void load(Object loadConfig, AsyncCallback<ListLoadResult<DisciplinaDTO>> callback) {
					Services.disciplinas().getDisciplinaPorProfessor(professorComboBox.getValue(), callback);
				}
			};
			
			setStore(new ListStore<DisciplinaDTO>(new BaseListLoader<BaseListLoadResult<DisciplinaDTO>>(proxy)));
			
			setDisplayField(DisciplinaDTO.PROPERTY_NOME);
			setFieldLabel("Disciplina");
			setEmptyText("Selecione a disciplina");
			setSimpleTemplate("{" + DisciplinaDTO.PROPERTY_NOME + "} ({" + DisciplinaDTO.PROPERTY_CODIGO + "})");
			setEditable(false);
			setTriggerAction(TriggerAction.ALL);
			setUseQueryCache(false);
			
		}

		private void addListeners() {
			professorComboBox.addSelectionChangedListener(new SelectionChangedListener<ProfessorDTO>(){
				@Override
				public void selectionChanged(SelectionChangedEvent<ProfessorDTO> se) {
					final ProfessorDTO professorDTO = se.getSelectedItem();
					getStore().removeAll();
					setValue(null);
					setEnabled(professorDTO != null);
					if(professorDTO != null) {
						getStore().getLoader().load();
					}
				}
			});
		}
		
	    @Override
	    public void onLoad(StoreEvent<DisciplinaDTO> se) {
	        super.onLoad(se);
	        if(getStore().getModels().isEmpty())
	        {
				MessageBox.alert("Aviso!","Não existem Disciplinas cadastradas e/ou Otimizadas.", null);
	        }
	    }

}
