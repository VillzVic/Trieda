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
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class OtimizacaoDisciplinasComboBox	extends ComboBox<DisciplinaDTO>
	{
		private CenarioDTO cenarioDTO;
		
		public OtimizacaoDisciplinasComboBox(CenarioDTO cenario) {
			this.cenarioDTO = cenario;
			
			RpcProxy<ListLoadResult<DisciplinaDTO>> proxy = new RpcProxy<ListLoadResult<DisciplinaDTO>>() {
				@Override
				public void load(Object loadConfig, AsyncCallback<ListLoadResult<DisciplinaDTO>> callback) {
					Services.disciplinas().getDisciplinaPorCenarioOtimizado(cenarioDTO, callback);
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
		
	    @Override
	    public void onLoad(StoreEvent<DisciplinaDTO> se) {
	        super.onLoad(se);
	        if(getStore().getModels().isEmpty())
	        {
				MessageBox.alert("Aviso!","Não existem Disciplinas cadastradas e/ou Otimizadas.", null);
	        }
	    }

}
