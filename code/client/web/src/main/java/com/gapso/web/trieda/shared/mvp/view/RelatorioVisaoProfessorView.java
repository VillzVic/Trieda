package com.gapso.web.trieda.shared.mvp.view;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.extjs.gxt.ui.client.widget.form.Field;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.mvp.presenter.RelatorioVisaoProfessorPresenter;
import com.gapso.web.trieda.shared.util.relatorioVisao.GradeHorariaProfessorGrid;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoProfessorFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoView;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.ProfessorComboBox;
import com.gapso.web.trieda.shared.util.view.ProfessorVirtualComboBox;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;

public class RelatorioVisaoProfessorView extends RelatorioVisaoView	implements RelatorioVisaoProfessorPresenter.Display{
	private CampusComboBox campusCB;
	private TurnoComboBox turnoCB;
	private ProfessorComboBox professorCB;
	private ProfessorVirtualComboBox professorVirtualCB;
	private UsuarioDTO usuario;
	private boolean isVisaoProfessor;
	private RelatorioVisaoProfessorFiltro filtro;

	public RelatorioVisaoProfessorView(UsuarioDTO usuario, boolean isVisaoProfessor){
		this.usuario = usuario;
		this.isVisaoProfessor = isVisaoProfessor;
		
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
		return "Master Data » Grade Horária Visão Professor";
	}
	
	@Override
	protected GTabItem createGTabItem(){
		return new GTabItem("Grade Horária Visão Professor", Resources.DEFAULTS.saidaProfessor16());
	}
	
	@Override
	protected GradeHorariaProfessorGrid createGradeHorariaVisao(){
		return new GradeHorariaProfessorGrid(isVisaoProfessor);
	}

	protected void createFilter(){
		Map<LabelAlign, List<Field<?>>> mapLayout = new HashMap<LabelAlign, List<Field<?>>>();
		
		List<Field<?>> leftList = new ArrayList<Field<?>>();
		
		if(this.usuario.isAdministrador()){
			this.campusCB = new CampusComboBox();
			filtro.addCampusValueListener(this.campusCB);
			leftList.add(this.campusCB);
		}
		
		this.turnoCB = new TurnoComboBox(this.campusCB, usuario.isProfessor());
		filtro.addTurnoValueListener(this.turnoCB);
		leftList.add(this.turnoCB);
		
		List<Field<?>> rightList = new ArrayList<Field<?>>();
		
		if(this.usuario.isAdministrador()){
			this.professorCB = new ProfessorComboBox(this.campusCB);
			
			this.professorVirtualCB = new ProfessorVirtualComboBox(this.campusCB);
			filtro.addProfessorVirtualValueListener(this.professorVirtualCB);
			rightList.add(this.professorVirtualCB);
		}
		else{
			this.professorCB = new ProfessorComboBox(usuario.getProfessorId());
			this.professorCB.setReadOnly(true);
		}
		filtro.addProfessorValueListener(this.professorCB);
		rightList.add(this.professorCB);
		
		mapLayout.put(LabelAlign.LEFT, leftList);
		mapLayout.put(LabelAlign.RIGHT, rightList);
		
		super.createFilter(mapLayout);
	}

	@Override
	public GradeHorariaProfessorGrid getGrid(){
		return (GradeHorariaProfessorGrid) this.grid;
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
	public CampusComboBox getCampusComboBox(){
		return this.campusCB;
	}

	@Override
	public TurnoComboBox getTurnoComboBox(){
		return this.turnoCB;
	}

	@Override
	public ProfessorComboBox getProfessorComboBox(){
		return this.professorCB;
	}

	@Override
	public ProfessorVirtualComboBox getProfessorVirtualComboBox(){
		return this.professorVirtualCB;
	}

}
