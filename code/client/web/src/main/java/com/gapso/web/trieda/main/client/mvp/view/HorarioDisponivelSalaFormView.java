package com.gapso.web.trieda.main.client.mvp.view;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.gapso.web.trieda.main.client.mvp.presenter.HorarioDisponivelSalaFormPresenter;
import com.gapso.web.trieda.main.client.util.resources.Resources;
import com.gapso.web.trieda.main.client.util.view.SemanaLetivaDoCenarioGrid;
import com.gapso.web.trieda.main.client.util.view.SimpleModal;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;

public class HorarioDisponivelSalaFormView extends MyComposite implements HorarioDisponivelSalaFormPresenter.Display {

	private SimpleModal simpleModal;
	private ContentPanel contentPanel;
	private SemanaLetivaDoCenarioGrid<HorarioDisponivelCenarioDTO> gridPanel;
	private List<HorarioDisponivelCenarioDTO> horariosDisponiveis;
	private SalaDTO salaDTO;
	
	public HorarioDisponivelSalaFormView(SalaDTO salaDTO, List<HorarioDisponivelCenarioDTO> horariosDisponiveis) {
		this.salaDTO = salaDTO;
		this.horariosDisponiveis = horariosDisponiveis;
		initUI();
		createGrid();
		// TODO
//		initComponent(simpleModal);
	}
	
	private void initUI() {
		simpleModal = new SimpleModal("Disponibilidade da Sala (" + salaDTO.getCodigo() + ")", Resources.DEFAULTS.sala16());
		simpleModal.setHeight(500);
		simpleModal.setWidth(600);
	}

	private void createGrid() {
		contentPanel = new ContentPanel(new FitLayout());
		contentPanel.setHeaderVisible(false);
		contentPanel.setBodyBorder(false);
	    gridPanel = new SemanaLetivaDoCenarioGrid<HorarioDisponivelCenarioDTO>(horariosDisponiveis,HorarioDisponivelCenarioDTO.PROPERTY_ID);
	    contentPanel.add(gridPanel);
		simpleModal.setContent(contentPanel);
	}

	@Override
	public Button getSalvarButton() {
		return simpleModal.getSalvarBt();
	}

	@Override
	public SalaDTO getSalaDTO() {
		return salaDTO;
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
