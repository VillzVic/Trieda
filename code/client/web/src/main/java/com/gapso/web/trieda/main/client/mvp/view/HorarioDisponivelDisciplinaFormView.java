package com.gapso.web.trieda.main.client.mvp.view;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.gapso.web.trieda.main.client.mvp.presenter.HorarioDisponivelDisciplinaFormPresenter;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.SemanaLetivaDoCenarioGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;

public class HorarioDisponivelDisciplinaFormView extends MyComposite implements HorarioDisponivelDisciplinaFormPresenter.Display {

	private SimpleModal simpleModal;
	private ContentPanel contentPanel;
	private SemanaLetivaDoCenarioGrid<HorarioDisponivelCenarioDTO> gridPanel;
	private List<HorarioDisponivelCenarioDTO> horariosDisponiveis;
	private DisciplinaDTO disciplinaDTO;
	
	public HorarioDisponivelDisciplinaFormView(DisciplinaDTO disciplinaDTO, List<HorarioDisponivelCenarioDTO> horariosDisponiveis) {
		this.disciplinaDTO = disciplinaDTO;
		this.horariosDisponiveis = horariosDisponiveis;
		initUI();
		createGrid();
	}

	private void initUI()
	{
		simpleModal = new SimpleModal("Disponibilidade da Disciplina (" + disciplinaDTO.getCodigo() + ")", Resources.DEFAULTS.disciplina16());
		simpleModal.setHeight(500);
		simpleModal.setWidth(750);
	}

	private void createGrid()
	{
		contentPanel = new ContentPanel(new FitLayout());
		contentPanel.setHeaderVisible(false);
		contentPanel.setBodyBorder(false);
	    gridPanel = new SemanaLetivaDoCenarioGrid<HorarioDisponivelCenarioDTO>(horariosDisponiveis,HorarioDisponivelCenarioDTO.PROPERTY_ID, disciplinaDTO.getUsaSabado(), disciplinaDTO.getUsaDomingo(),this);
	    
	    if(!disciplinaDTO.getUsaSabado()){
	    	gridPanel.getSabCB().toggle(false);
	    	gridPanel.getSabCB().fireEvent(Events.Select);
	    	gridPanel.getSabCB().disable();
	    }
	    if(!disciplinaDTO.getUsaDomingo()){
	    	gridPanel.getDomCB().toggle(false);
	    	gridPanel.getDomCB().fireEvent(Events.Select);
	    	gridPanel.getDomCB().disable();
	    }
	    
	    
	    contentPanel.add(gridPanel);
		simpleModal.setContent(contentPanel);
	}

	@Override
	public Button getSalvarButton() {
		return simpleModal.getSalvarBt();
	}

	@Override
	public DisciplinaDTO getDisciplinaDTO() {
		return disciplinaDTO;
	}

	@Override
	public SimpleModal getSimpleModal() {
		return simpleModal;
	}

	@Override
	public void setProxy(RpcProxy<PagingLoadResult<HorarioDisponivelCenarioDTO>> proxy) {
		gridPanel.setProxy(proxy);
	}

	@Override
	public ListStore<HorarioDisponivelCenarioDTO> getStore() {
		return gridPanel.getStore();
	}

	

}
