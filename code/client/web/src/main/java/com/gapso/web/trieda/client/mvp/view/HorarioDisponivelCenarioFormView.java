package com.gapso.web.trieda.client.mvp.view;

import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.gapso.web.trieda.client.mvp.model.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.client.mvp.model.SemanaLetivaDTO;
import com.gapso.web.trieda.client.mvp.presenter.HorarioDisponivelCenarioFormPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.SemanaLetivaGrid;
import com.gapso.web.trieda.client.util.view.SimpleModal;

public class HorarioDisponivelCenarioFormView extends MyComposite implements HorarioDisponivelCenarioFormPresenter.Display {

	private SimpleModal simpleModal;
	private ContentPanel contentPanel;
	private SemanaLetivaDTO semanaLetivaDTO;
	private SemanaLetivaGrid<HorarioDisponivelCenarioDTO> gridPanel;
	
	public HorarioDisponivelCenarioFormView(SemanaLetivaDTO semanaLetivaDTO) {
		this.semanaLetivaDTO = semanaLetivaDTO;
		initUI();
		createGrid();
		// TODO
//		initComponent(simpleModal);
	}
	
	private void initUI() {
		simpleModal = new SimpleModal("Dias e Horários de Aula", Resources.DEFAULTS.semanaLetiva16());
		simpleModal.setHeight(500);
		simpleModal.setWidth(600);
	}

	private void createGrid() {
		contentPanel = new ContentPanel(new FitLayout());
		contentPanel.setHeaderVisible(false);
		contentPanel.setBodyBorder(false);
	    gridPanel = new SemanaLetivaGrid<HorarioDisponivelCenarioDTO>();
	    contentPanel.add(gridPanel);
		simpleModal.setContent(contentPanel);
	}

	@Override
	public Button getSalvarButton() {
		return simpleModal.getSalvarBt();
	}

	@Override
	public SemanaLetivaDTO getSemanaLetivaDTO() {
		return semanaLetivaDTO;
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
