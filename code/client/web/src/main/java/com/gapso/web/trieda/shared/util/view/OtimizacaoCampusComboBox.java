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
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class OtimizacaoCampusComboBox extends ComboBox<CampusDTO>
{
	private OtimizacaoProfessorComboBox professorComboBox;
	
	public OtimizacaoCampusComboBox(OtimizacaoProfessorComboBox professorCB) {
		this.professorComboBox = professorCB;
		addListeners();
		
		RpcProxy<ListLoadResult<CampusDTO>> proxy = new RpcProxy<ListLoadResult<CampusDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<CampusDTO>> callback) {
				Services.campi().getCampusPorProfessor(professorComboBox.getValue(), callback);
			}
		};
		
		setStore(new ListStore<CampusDTO>(new BaseListLoader<BaseListLoadResult<CampusDTO>>(proxy)));
		
		setDisplayField( CampusDTO.PROPERTY_CODIGO );
		setFieldLabel( "Campus" );
		setEmptyText( "Selecione o campus" );
		setSimpleTemplate( "{" + CampusDTO.PROPERTY_NOME +
			"} ({" + CampusDTO.PROPERTY_CODIGO + "})" );
		setEditable( false );
		setTriggerAction( TriggerAction.ALL );
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
    public void onLoad(StoreEvent<CampusDTO> se) {
        super.onLoad(se);
        if(getStore().getModels().isEmpty())
        {
			MessageBox.alert("Aviso!","Não existem Campus cadastrados e/ou Otimizadas.", null);
        }
    }


}
