package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.extjs.gxt.ui.client.widget.form.ComboBox.TriggerAction;
import com.extjs.gxt.ui.client.widget.form.Field;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.form.SimpleComboBox;
import com.gapso.web.trieda.main.client.mvp.presenter.RelatorioVisaoCursoPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.util.relatorioVisao.GradeHorariaCursoGrid;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoCursoFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoView;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.CurriculoComboBox;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;

public class RelatorioVisaoCursoView extends RelatorioVisaoView	implements RelatorioVisaoCursoPresenter.Display{
	private CampusComboBox campusCB;
	private CursoComboBox cursoCB;
	private TurnoComboBox turnoCB;
	private CurriculoComboBox curriculoCB;
	private SimpleComboBox<Integer> periodoCB;
	private RelatorioVisaoCursoFiltro filtro;

	public RelatorioVisaoCursoView(CenarioDTO cenarioDTO){
		super(cenarioDTO, new RelatorioVisaoCursoFiltro());
	}

	@Override
	protected String getHeadingPanel(){
		return "Master Data » Grade Horária Visão Curso";
	}
	
	@Override
	protected GTabItem createGTabItem(){
		return new GTabItem("Grade Horária Visão Curso", Resources.DEFAULTS.saidaCurso16());
	}
	
	@Override
	protected GradeHorariaCursoGrid createGradeHorariaVisao(){
		return new GradeHorariaCursoGrid();
	}

	@Override
	protected void createFilter(){
		Map<LabelAlign, List<Field<?>>> mapLayout = new HashMap<LabelAlign, List<Field<?>>>();
		
		List<Field<?>> leftList = new ArrayList<Field<?>>();
		
		this.cursoCB = new CursoComboBox(cenarioDTO);
		filtro.addCursoValueListener(this.cursoCB);
		filtro.addCursoValueListener(this.cursoCB);
		leftList.add(this.cursoCB);
		
		this.curriculoCB = new CurriculoComboBox(cenarioDTO, this.cursoCB);
		this.curriculoCB.setUseQueryCache(false);
		filtro.addCurriculoValueListener(this.curriculoCB);
		leftList.add(this.curriculoCB);
		
		
		List<Field<?>> rightList = new ArrayList<Field<?>>();
		
		List<Field<?>> centerList = new ArrayList<Field<?>>();
		
		this.campusCB = new CampusComboBox(cenarioDTO, this.curriculoCB, false);
		filtro.addCampusValueListener(this.campusCB);
		centerList.add(this.campusCB);
		
		this.periodoCB = new SimpleComboBox<Integer>();
		this.periodoCB.setFieldLabel("Período");
		this.periodoCB.setEditable(false);
		this.periodoCB.setTriggerAction(TriggerAction.ALL);
		filtro.addPeriodoValueListener(this.periodoCB);
		centerList.add(this.periodoCB);
		
		this.turnoCB = new TurnoComboBox(this.campusCB, cenarioDTO);
		filtro.addTurnoValueListener(this.turnoCB);
		rightList.add(this.turnoCB);

		mapLayout.put(LabelAlign.LEFT, leftList);
		mapLayout.put(LabelAlign.RIGHT, rightList);
		mapLayout.put(LabelAlign.TOP, centerList);
		
		super.createFilter(mapLayout);
	}
	
	@Override
	public GradeHorariaCursoGrid getGrid(){
		return (GradeHorariaCursoGrid) this.grid;
	}

	@Override
	public RelatorioVisaoCursoFiltro getFiltro(){
		return this.filtro;
	}
	
	@Override
	public void setFiltro(RelatorioVisaoFiltro filtro){
		this.filtro = (RelatorioVisaoCursoFiltro) filtro;
	}

	@Override
	public CampusComboBox getCampusComboBox() {
		return this.campusCB;
	}

	@Override
	public CursoComboBox getCursoComboBox() {
		return this.cursoCB;
	}

	@Override
	public CurriculoComboBox getCurriculoComboBox() {
		return this.curriculoCB;
	}

	@Override
	public TurnoComboBox getTurnoComboBox() {
		return this.turnoCB;
	}

	@Override
	public SimpleComboBox<Integer> getPeriodoComboBox() {
		return this.periodoCB;
	}

}
