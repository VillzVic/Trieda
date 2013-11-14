package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.Field;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.RelatorioAlunoFiltro;
import com.gapso.web.trieda.shared.util.view.RelatorioView;
import com.gapso.web.trieda.main.client.mvp.presenter.RelatorioAlunosPresenter;

public class RelatorioAlunosView extends RelatorioView
	implements RelatorioAlunosPresenter.Display
{

	RelatorioAlunoFiltro filtro;
	CheckBox formandoCB;
	NumberField periodoCB;
	CursoComboBox cursoCB;
	int numHistogramas = 1;
	
	
	public RelatorioAlunosView(CenarioDTO cenarioDTO) {
		super(cenarioDTO);
		createHistogramasButton(numHistogramas);
	}

	@Override
	protected String getHeadingPanel() {
		return "Master Data » Relatório de Alunos";
	}

	@Override
	protected GTabItem createGTabItem() {
		return new GTabItem("Relatório de Alunos", Resources.DEFAULTS.professor16());
	}

	@Override
	protected void setFilter() {
		this.filtro = new RelatorioAlunoFiltro();
		
		cursoCB = new CursoComboBox(cenarioDTO);
		
		periodoCB = new NumberField();
		periodoCB.setFieldLabel("Periodo");
		periodoCB.setAllowDecimals(false);
		periodoCB.setMinValue(0);
		periodoCB.setMaxValue(100);
		
		formandoCB = new CheckBox();
		formandoCB.setFieldLabel("Somente Alunos Formandos");
		formandoCB.setLabelStyle("width: 200px");
	}
	
	@Override
	protected List<Field<?>> getFiltersList() {
		List<Field<?>> listFiltro = new ArrayList<Field<?>>();
		listFiltro.add(cursoCB);
		listFiltro.add(periodoCB);
		listFiltro.add(formandoCB);
		
		return listFiltro;
	}
	
	@Override
	public RelatorioAlunoFiltro getAlunoFiltro()
	{
		return filtro;
	}
	
	@Override
	public NumberField getPeriodoComboBox()
	{
		return periodoCB;
	}
	
	@Override
	public CursoComboBox getCursoComboBox()
	{
		return cursoCB;
	}
	
	@Override
	public CheckBox getFormandoCheckBox()
	{
		return formandoCB;
	}
}
