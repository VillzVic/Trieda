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
import com.gapso.web.trieda.shared.dtos.TurmaDTO;
import com.gapso.web.trieda.shared.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class OtimizacaoTurmaComboBox extends ComboBox<AtendimentoOperacionalDTO>
{
	private OtimizacaoDisciplinasComboBox disciplinaComboBox;
	
	public OtimizacaoTurmaComboBox(OtimizacaoDisciplinasComboBox disciplinaCB) {
		this.disciplinaComboBox = disciplinaCB;
		addListeners();
		
		RpcProxy<ListLoadResult<AtendimentoOperacionalDTO>> proxy = new RpcProxy<ListLoadResult<AtendimentoOperacionalDTO>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<ListLoadResult<AtendimentoOperacionalDTO>> callback) {
				Services.atendimentos().getTurmasPorDisciplina(disciplinaComboBox.getValue(), callback);
			}
		};
		
		setStore(new ListStore<AtendimentoOperacionalDTO>(new BaseListLoader<BaseListLoadResult<AtendimentoOperacionalDTO>>(proxy)));
		
		setDisplayField(AtendimentoOperacionalDTO.PROPERTY_TURMA);
		setFieldLabel("Turma");
		setEmptyText("Selecione a Turma");
		setSimpleTemplate("{" + AtendimentoOperacionalDTO.PROPERTY_TURMA + "}");
		setEditable(false);
		setTriggerAction(TriggerAction.ALL);
		setUseQueryCache(false);
		
	}

	private void addListeners() {
		disciplinaComboBox.addSelectionChangedListener(new SelectionChangedListener<DisciplinaDTO>(){
			@Override
			public void selectionChanged(SelectionChangedEvent<DisciplinaDTO> se) {
				final DisciplinaDTO disciplinaDTO = se.getSelectedItem();
				getStore().removeAll();
				setValue(null);
				setEnabled(disciplinaDTO != null);
				getStore().getLoader().load();
				
			}
		});
	}
	
    @Override
    public void onLoad(StoreEvent<AtendimentoOperacionalDTO> se) {
        super.onLoad(se);
        if(getStore().getModels().isEmpty())
        {
			MessageBox.alert("Aviso!","Não existem Turmas cadastradas e/ou Otimizadas.", null);
        }
    }


}
