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
import com.gapso.web.trieda.shared.dtos.OfertaDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class OfertaComboBox extends ComboBox<OfertaDTO>
{
	private CenarioDTO cenarioDTO;
	
	public OfertaComboBox(CenarioDTO cenarioDTO) {
		this.cenarioDTO = cenarioDTO;
		configureContentOfComboBox();
		configureView();
	}
	
	private void configureView() {
		setDisplayField(OfertaDTO.PROPERTY_TEXT);
		setFieldLabel("Oferta");
		setEmptyText("Selecione uma oferta");
		setSimpleTemplate("{" + OfertaDTO.PROPERTY_CAMPUS_STRING + "} / " +
				          "{" + OfertaDTO.PROPERTY_CURSO_STRING + "} / " +
				          "{" + OfertaDTO.PROPERTY_MATRIZ_CURRICULAR_STRING + "} / " +
				          "{" + OfertaDTO.PROPERTY_TURNO_STRING + "}");
		setEditable(false);
		setAutoWidth(true);
		setTriggerAction(TriggerAction.ALL);
	}
	
	private void configureContentOfComboBox() {
		RpcProxy<ListLoadResult<OfertaDTO>> proxy = new RpcProxy<ListLoadResult<OfertaDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<OfertaDTO>> callback) {
					Services.ofertas().getListAll( cenarioDTO, callback );
			}
		};

		setStore(new ListStore<OfertaDTO>(new BaseListLoader<BaseListLoadResult<OfertaDTO>>(proxy)));
	}
	
    @Override
    public void onLoad(StoreEvent<OfertaDTO> se) {
        super.onLoad(se);
        if(getStore().getModels().isEmpty())
        {
			MessageBox.alert("Aviso!","Não existem Ofertas cadastradas", null);
        }
    }
}