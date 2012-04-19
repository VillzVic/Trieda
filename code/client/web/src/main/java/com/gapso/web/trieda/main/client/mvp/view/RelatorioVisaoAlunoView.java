package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.extjs.gxt.ui.client.widget.form.Field;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.gapso.web.trieda.main.client.mvp.presenter.RelatorioVisaoAlunoPresenter;
import com.gapso.web.trieda.shared.util.relatorioVisao.GradeHorariaAlunoGrid;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoAlunoFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoView;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.AlunosComboBox;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;

public class RelatorioVisaoAlunoView extends RelatorioVisaoView implements RelatorioVisaoAlunoPresenter.Display{
	private CampusComboBox campusCB;
	private TurnoComboBox turnoCB;
	private AlunosComboBox alunoCB;
	private RelatorioVisaoAlunoFiltro filtro;

	public RelatorioVisaoAlunoView(){
		super(new RelatorioVisaoAlunoFiltro());
	}
	
	@Override
	protected String getHeadingPanel(){
		return "Master Data » Grade Horária Visão Aluno";
	}
	
	@Override
	protected GTabItem createGTabItem(){
		return new GTabItem("Grade Horária Visão Aluno", Resources.DEFAULTS.saidaProfessor16());
	}
	
	@Override
	protected GradeHorariaAlunoGrid createGradeHorariaVisao(){
		return new GradeHorariaAlunoGrid();
	}

	@Override
	protected void createFilter(){
		Map<LabelAlign, List<Field<?>>> mapLayout = new HashMap<LabelAlign, List<Field<?>>>();
		
		List<Field<?>> leftList = new ArrayList<Field<?>>();
		
		this.campusCB = new CampusComboBox();
		filtro.addCampusValueListener(this.campusCB);
		leftList.add(this.campusCB);
		
		this.turnoCB = new TurnoComboBox(this.campusCB);
		filtro.addTurnoValueListener(this.turnoCB);
		leftList.add(this.turnoCB);
		
		List<Field<?>> rightList = new ArrayList<Field<?>>();
		
		this.alunoCB = new AlunosComboBox(this.campusCB);
		filtro.addAlunoValueListener(this.alunoCB);
		rightList.add(this.alunoCB);

		mapLayout.put(LabelAlign.LEFT, leftList);
		mapLayout.put(LabelAlign.RIGHT, rightList);
		
		super.createFilter(mapLayout);
	}

	@Override
	public GradeHorariaAlunoGrid getGrid(){
		return (GradeHorariaAlunoGrid) this.grid;
	}

	@Override
	public RelatorioVisaoAlunoFiltro getFiltro(){
		return this.filtro;
	}
	
	@Override
	public void setFiltro(RelatorioVisaoFiltro filtro){
		this.filtro = (RelatorioVisaoAlunoFiltro) filtro;
	}
	
}
