package com.gapso.web.trieda.shared.mvp.view;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.mvp.presenter.HorarioDisponivelProfessorFormPresenter;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.SemanaLetivaDoCenarioGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;

public class HorarioDisponivelProfessorFormView extends MyComposite implements HorarioDisponivelProfessorFormPresenter.Display {

	private SimpleModal simpleModal;
	private ContentPanel contentPanel;
	private SemanaLetivaDoCenarioGrid<HorarioDisponivelCenarioDTO> gridPanel;
	private List<HorarioDisponivelCenarioDTO> horariosDisponiveisProfessor;
	private ProfessorDTO professorDTO;
	
	public HorarioDisponivelProfessorFormView(ProfessorDTO professorDTO, List<HorarioDisponivelCenarioDTO> horariosDisponiveisProfessor) {
		this.professorDTO = professorDTO;
		this.horariosDisponiveisProfessor = horariosDisponiveisProfessor;
		initUI();
		createGrid();
	}
	
	private void initUI()
	{
		simpleModal = new SimpleModal("Disponibilidade de Professor (" + professorDTO.getCpf() + ")", Resources.DEFAULTS.professor16());
		simpleModal.setHeight(500);
		simpleModal.setWidth(750);
	}

	private void createGrid()
	{
		contentPanel = new ContentPanel(new FitLayout());
		contentPanel.setHeaderVisible(false);
		contentPanel.setBodyBorder(false);
	    gridPanel = new SemanaLetivaDoCenarioGrid<HorarioDisponivelCenarioDTO>(
	    	horariosDisponiveisProfessor,HorarioDisponivelCenarioDTO.PROPERTY_ID, this);
	    contentPanel.add(gridPanel);
		simpleModal.setContent(contentPanel);
	}

	@Override
	public Button getSalvarButton() {
		return simpleModal.getSalvarBt();
	}

	@Override
	public ProfessorDTO getProfessorDTO() {
		return professorDTO;
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
