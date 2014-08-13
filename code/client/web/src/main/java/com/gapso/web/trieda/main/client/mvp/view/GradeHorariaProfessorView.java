package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.gapso.web.trieda.main.client.mvp.presenter.GradeHorariaProfessorFormPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.relatorioVisao.GradeHorariaProfessorGrid;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoProfessorFiltro;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.SimpleModal;

public class GradeHorariaProfessorView extends MyComposite implements
	GradeHorariaProfessorFormPresenter.Display {


	private SimpleModal simpleModal;
	private ContentPanel contentPanel;
	private ProfessorDTO professorDTO;
	private ProfessorVirtualDTO professorVirtualDTO;
	private CenarioDTO cenarioDTO;
	private GradeHorariaProfessorGrid gridPanel;
	
	public GradeHorariaProfessorView(CenarioDTO cenarioDTO, ProfessorDTO professorDTO) {
		this.professorDTO = professorDTO;
		this.cenarioDTO = cenarioDTO;
		initUI();
		createGrid();
	}
	
	public GradeHorariaProfessorView(CenarioDTO cenarioDTO, ProfessorVirtualDTO professorVirtualDTO) {
		this.professorVirtualDTO = professorVirtualDTO;
		this.cenarioDTO = cenarioDTO;
		initUI();
		createGrid();
	}
	
	private void initUI() {
		simpleModal = new SimpleModal(null, "Fechar", "Grade Horaria do Professor ("
				+ (professorDTO != null ? professorDTO.getCpf() : professorVirtualDTO.getNome()) + ")", Resources.DEFAULTS.professor16());
		simpleModal.setHeight(500);
		simpleModal.setWidth(820);
	}
	
	protected GradeHorariaProfessorGrid createGradeHorariaVisao(){
		GradeHorariaProfessorGrid grade = new GradeHorariaProfessorGrid(cenarioDTO, false)
		{
			@Override
			protected void afterRender() {
				this.requestAtendimentos();
				super.afterRender();
			}
		};
		return grade;
	}
	
	private void createGrid() {
		contentPanel = new ContentPanel(new FitLayout());
		contentPanel.setHeaderVisible(false);
		contentPanel.setBodyBorder(false);
		gridPanel = createGradeHorariaVisao();
		RelatorioVisaoProfessorFiltro filtro = new RelatorioVisaoProfessorFiltro();
		if (professorDTO != null)
		{
			filtro.setProfessorCpf(professorDTO.getCpf());
		}
		else if(professorVirtualDTO != null)
		{
			filtro.setProfessorVirtualDTO(professorVirtualDTO);
		}
		gridPanel.setFiltro(filtro);
		contentPanel.add(gridPanel);
		simpleModal.setContent(contentPanel);
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
	public GradeHorariaProfessorGrid getPanel()
	{
		return gridPanel;
	}
}
