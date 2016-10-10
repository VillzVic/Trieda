package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.store.StoreEvent;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class OtimizacaoProfessorComboBox extends ComboBox<ProfessorDTO>
{
	private CenarioDTO cenarioDTO;


	public OtimizacaoProfessorComboBox(CenarioDTO cenario){
		this.cenarioDTO = cenario;

		
			RpcProxy<ListLoadResult<ProfessorDTO>> proxy = new RpcProxy<ListLoadResult<ProfessorDTO>>(){
				@Override
				public void load(Object loadConfig, AsyncCallback<ListLoadResult<ProfessorDTO>> callback){
						Services.professores().getProfessoresOtimizados(cenarioDTO, callback);
				}
			};

			setStore(new ListStore<ProfessorDTO>(new BaseListLoader<BaseListLoadResult<ProfessorDTO>>(proxy)));

		this.configComboBox();
	}
	
	private void configComboBox(){
		setDisplayField(ProfessorDTO.PROPERTY_NOME);
		setFieldLabel("Professor");
		setEmptyText("Selecione o professor");
		setSimpleTemplate("{" + ProfessorDTO.PROPERTY_NOME + "} ({" + ProfessorDTO.PROPERTY_CPF + "})");
		setEditable(false);
		setTriggerAction(TriggerAction.ALL);
		setUseQueryCache(false);
	}
	
    @Override
    public void onLoad(StoreEvent<ProfessorDTO> se) {
        super.onLoad(se);
        if(getStore().getModels().isEmpty())
        {
			MessageBox.alert("Aviso!","Não existem Professores cadastrados e/ou Otimizados.", null);
        }
    }



}
