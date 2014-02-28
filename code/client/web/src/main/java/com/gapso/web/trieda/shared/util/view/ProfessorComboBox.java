package com.gapso.web.trieda.shared.util.view;

import java.util.ArrayList;
import java.util.List;

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

public class ProfessorComboBox extends ComboBox<ProfessorDTO>{
	private CampusComboBox campusComboBox;
	private Boolean readOnly;
	private CenarioDTO cenarioDTO;

	public ProfessorComboBox(CenarioDTO cenarioDTO){
		this(null, false);
	}
	
	public ProfessorComboBox(CenarioDTO cenario,boolean readOnly){
		this(cenario, null, readOnly);
	}

	public ProfessorComboBox(CenarioDTO cenarioDTO, CampusComboBox campusCB){
		this(cenarioDTO, campusCB, false);
	}
	
	public ProfessorComboBox(ProfessorDTO professorDTO){
		this.setReadOnly(true);
		ListStore<ProfessorDTO> store = new ListStore<ProfessorDTO>();
		store.add(professorDTO);
		setStore(store);
		this.configComboBox();
		List<ProfessorDTO> list = new ArrayList<ProfessorDTO>();
		list.add(professorDTO);
		this.setSelection(list);
	}

	public ProfessorComboBox(CenarioDTO cenario, CampusComboBox campusCB, boolean readOnly){
		this.campusComboBox = campusCB;
		this.readOnly = readOnly;
		this.cenarioDTO = cenario;
		this.setReadOnly(readOnly);

		if(!this.readOnly){
			RpcProxy<ListLoadResult<ProfessorDTO>> proxy = new RpcProxy<ListLoadResult<ProfessorDTO>>(){
				@Override
				public void load(Object loadConfig, AsyncCallback<ListLoadResult<ProfessorDTO>> callback){
					if(campusComboBox != null && campusComboBox.getValue() != null){
						Services.professores().getProfessoresEmCampus(campusComboBox.getValue(), callback);
					}
					else{
						Services.professores().getList(cenarioDTO, callback);
					}
				}
			};

			setStore(new ListStore<ProfessorDTO>(new BaseListLoader<BaseListLoadResult<ProfessorDTO>>(proxy)));
		}
		else{
			setStore(new ListStore<ProfessorDTO>());
		}

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
        System.out.println(getStore().getModels().size());
        if(getStore().getModels().isEmpty())
        {
			MessageBox.alert("Aviso!","Não existem Professores cadastrados", null);
        }
    }
}
