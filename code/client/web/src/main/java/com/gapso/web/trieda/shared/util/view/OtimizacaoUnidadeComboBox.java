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
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class OtimizacaoUnidadeComboBox	extends ComboBox<UnidadeDTO>
{
	private OtimizacaoTurmaComboBox turmaComboBox;
	private OtimizacaoCampusComboBox campusComboBox;
	
	public OtimizacaoUnidadeComboBox(OtimizacaoTurmaComboBox turmaCB, OtimizacaoCampusComboBox campusCB) {
		this.turmaComboBox = turmaCB;
		this.campusComboBox = campusCB;
		
		RpcProxy<ListLoadResult<UnidadeDTO>> proxy = new RpcProxy<ListLoadResult<UnidadeDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<UnidadeDTO>> callback) {
				Services.unidades().getUnidadesPorCampusOtimizado(turmaComboBox.getValue(), campusComboBox.getValue(), callback);
			}
		};
		
		setStore(new ListStore<UnidadeDTO>(new BaseListLoader<BaseListLoadResult<UnidadeDTO>>(proxy)));
		
		setDisplayField(UnidadeDTO.PROPERTY_NOME);
		setFieldLabel("Unidade");
		setEmptyText("Selecione a unidade");
		setSimpleTemplate("{"+UnidadeDTO.PROPERTY_NOME+"}");
		setEditable(false);
		setTriggerAction(TriggerAction.ALL);
			
	}
	
    @Override
    public void onLoad(StoreEvent<UnidadeDTO> se) {
        super.onLoad(se);
        if(getStore().getModels().isEmpty())
        {
			MessageBox.alert("Aviso!","Não existem Unidades cadastradas e/ou Otimizadas. ", null);
        }
    }

}
