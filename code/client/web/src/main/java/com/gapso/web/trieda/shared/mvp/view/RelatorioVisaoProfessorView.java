package com.gapso.web.trieda.shared.mvp.view;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.extjs.gxt.ui.client.widget.form.Field;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.mvp.presenter.RelatorioVisaoProfessorPresenter;
import com.gapso.web.trieda.shared.util.relatorioVisao.GradeHorariaProfessorGrid;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoProfessorFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoView;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.ProfessorCpfComboBox;
import com.gapso.web.trieda.shared.util.view.ProfessorNomeComboBox;
import com.gapso.web.trieda.shared.util.view.ProfessorVirtualComboBox;


public class RelatorioVisaoProfessorView extends RelatorioVisaoView	implements RelatorioVisaoProfessorPresenter.Display{
	private ProfessorVirtualComboBox professorVirtualCB;
	private ProfessorNomeComboBox professorTF;
	private ProfessorCpfComboBox cpfTF;
	private UsuarioDTO usuario;
	private boolean isVisaoProfessor;
	private RelatorioVisaoProfessorFiltro filtro;

	public RelatorioVisaoProfessorView(CenarioDTO cenarioDTO, UsuarioDTO usuario, boolean isVisaoProfessor){
		this.usuario = usuario;
		this.isVisaoProfessor = isVisaoProfessor;
		this.cenarioDTO = cenarioDTO;
		
		this.setFiltro(new RelatorioVisaoProfessorFiltro());
		this.initUI();
	}

	public boolean isVisaoProfessor(){
		return this.isVisaoProfessor;
	}

	public void setVisaoProfessor(boolean isVisaoProfessor){
		this.isVisaoProfessor = isVisaoProfessor;
	}
	
	@Override
	protected String getHeadingPanel(){
		return cenarioDTO.getNome() + " » Grade Horária Visão Professor";
	}
	
	@Override
	protected GTabItem createGTabItem(){
		return new GTabItem("Grade Horária Visão Professor", Resources.DEFAULTS.saidaProfessor16());
	}
	
	@Override
	protected GradeHorariaProfessorGrid createGradeHorariaVisao(){
		return new GradeHorariaProfessorGrid(cenarioDTO, isVisaoProfessor);
	}

	protected void createFilter(){
		Map<LabelAlign, List<Field<?>>> mapLayout = new HashMap<LabelAlign, List<Field<?>>>();
		
		List<Field<?>> leftList = new ArrayList<Field<?>>();
		
		this.professorTF = new ProfessorNomeComboBox(cenarioDTO);
		this.professorTF.setEmptyText("Digite o nome do professor");
		this.professorTF.setName("professor");
		this.professorTF.setFieldLabel("Professor");
		filtro.addProfessorNomeValueListener(professorTF);
		leftList.add(professorTF);
		
		List<Field<?>> centerList = new ArrayList<Field<?>>();
		
		this.cpfTF = new ProfessorCpfComboBox(cenarioDTO);
		this.cpfTF.setEmptyText("Digite o cpf do professor");
		this.cpfTF.setName("cpf");
		this.cpfTF.setFieldLabel("CPF");
		filtro.addProfessorCpfValueListener(cpfTF);
		centerList.add(cpfTF);
		
		List<Field<?>> rightList = new ArrayList<Field<?>>();
				
		this.professorVirtualCB = new ProfessorVirtualComboBox(cenarioDTO);
		filtro.addProfessorVirtualValueListener(this.professorVirtualCB);
		rightList.add(this.professorVirtualCB);
		
		mapLayout.put(LabelAlign.LEFT, leftList);
		mapLayout.put(LabelAlign.RIGHT, rightList);
		mapLayout.put(LabelAlign.TOP, centerList);
		
		super.createFilter(mapLayout);
	}

	@Override
	public GradeHorariaProfessorGrid getGrid(){
		return (GradeHorariaProfessorGrid) this.grid;
	}
	
	@Override
	public ProfessorNomeComboBox getProfessorTextField(){
		return this.professorTF;
	}
	
	@Override
	public ProfessorCpfComboBox getCpfTextField(){
		return this.cpfTF;
	}

	@Override
	public RelatorioVisaoProfessorFiltro getFiltro(){
		return this.filtro;
	}
	
	@Override
	public void setFiltro(RelatorioVisaoFiltro filtro){
		this.filtro = (RelatorioVisaoProfessorFiltro) filtro;
	}

	@Override
	public ProfessorVirtualComboBox getProfessorVirtualComboBox(){
		return this.professorVirtualCB;
	}

	@Override
	public void disableView() {
		getProfessorTextField().disable();
		getCpfTextField().disable();
		getProfessorVirtualComboBox().disable();
		getExportXlsExcelButton().disable();
		getExportXlsxExcelButton().disable();
		getSubmitBuscaButton().disable();
		getExportExcelButton().disable();
		
	}

	@Override
	public void checkRegistered() {
		if(!cenarioDTO.hasProfessores()){
			disableView();
		}
	}

}
