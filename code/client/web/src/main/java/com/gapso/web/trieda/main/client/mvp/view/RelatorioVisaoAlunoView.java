package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.extjs.gxt.ui.client.widget.form.Field;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.main.client.mvp.presenter.RelatorioVisaoAlunoPresenter;
import com.gapso.web.trieda.shared.util.relatorioVisao.GradeHorariaAlunoGrid;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoAlunoFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoView;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.GTabItem;

public class RelatorioVisaoAlunoView extends RelatorioVisaoView implements RelatorioVisaoAlunoPresenter.Display{
	private TextField<String> alunoTF;
	private TextField<String> matriculaTF;
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
		
		this.alunoTF = new TextField<String>();
		this.alunoTF.setEmptyText("Digite o nome do aluno");
		this.alunoTF.setName("aluno");
		this.alunoTF.setFieldLabel("Aluno");
		filtro.addAlunoNomeValueListener(alunoTF);
		leftList.add(alunoTF);
		
		List<Field<?>> centerList = new ArrayList<Field<?>>();
		
		List<Field<?>> rightList = new ArrayList<Field<?>>();
		
		this.matriculaTF = new TextField<String>();
		this.matriculaTF.setEmptyText("Digite o número da matricula");
		this.matriculaTF.setName("matricula");
		this.matriculaTF.setFieldLabel("Matricula");
		filtro.addAlunoMatriculaValueListener(matriculaTF);
		rightList.add(matriculaTF);

		mapLayout.put(LabelAlign.LEFT, leftList);
		mapLayout.put(LabelAlign.RIGHT, rightList);
		mapLayout.put(LabelAlign.TOP, centerList);
		
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
