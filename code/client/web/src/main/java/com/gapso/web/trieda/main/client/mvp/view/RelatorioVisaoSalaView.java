package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.extjs.gxt.ui.client.widget.form.Field;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.gapso.web.trieda.main.client.mvp.presenter.RelatorioVisaoSalaPresenter;
import com.gapso.web.trieda.shared.util.relatorioVisao.GradeHorariaSalaGrid;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoSalaFiltro;
import com.gapso.web.trieda.shared.util.relatorioVisao.RelatorioVisaoView;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.GTabItem;

public class RelatorioVisaoSalaView	extends RelatorioVisaoView	implements RelatorioVisaoSalaPresenter.Display{
	private TextField<String> salaTF;
	private RelatorioVisaoSalaFiltro filtro;

	public RelatorioVisaoSalaView(){
		super(new RelatorioVisaoSalaFiltro());
	}

	@Override
	protected String getHeadingPanel(){
		return "Master Data » Grade Horária Visão Sala";
	}
	
	@Override
	protected GTabItem createGTabItem(){
		return new GTabItem("Grade Horária Visão Sala", Resources.DEFAULTS.saidaSala16());
	}
	
	@Override
	protected GradeHorariaSalaGrid createGradeHorariaVisao(){
		return new GradeHorariaSalaGrid();
	}
	
	@Override
	protected void createFilter(){
		Map<LabelAlign, List<Field<?>>> mapLayout = new HashMap<LabelAlign, List<Field<?>>>();
		
		List<Field<?>> leftList = new ArrayList<Field<?>>();
		
		this.salaTF = new TextField<String>();
		this.salaTF.setEmptyText("Digite codigo da sala");
		this.salaTF.setName("salaTF");
		this.salaTF.setFieldLabel("Codigo da Sala");
		filtro.addSalaValueListener(salaTF);
		leftList.add(this.salaTF);
		
		List<Field<?>> centerList = new ArrayList<Field<?>>();
		
		List<Field<?>> rightList = new ArrayList<Field<?>>();
		
		mapLayout.put(LabelAlign.LEFT, leftList);
		mapLayout.put(LabelAlign.RIGHT, rightList);
		mapLayout.put(LabelAlign.TOP, centerList);
		
		super.createFilter(mapLayout);
	}

	@Override
	public GradeHorariaSalaGrid getGrid(){
		return (GradeHorariaSalaGrid) this.grid;
	}
	
	@Override
	public RelatorioVisaoSalaFiltro getFiltro(){
		return this.filtro;
	}
	
	@Override
	public void setFiltro(RelatorioVisaoFiltro filtro){
		this.filtro = (RelatorioVisaoSalaFiltro) filtro;
	}
	
	@Override
	public TextField<String> getSalaTextField(){
		return this.salaTF;
	}

}
