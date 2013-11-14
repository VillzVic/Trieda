package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.widget.form.Field;
import com.gapso.web.trieda.main.client.mvp.presenter.RelatorioProfessoresPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.AreaTitulacaoComboBox;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.RelatorioProfessorFiltro;
import com.gapso.web.trieda.shared.util.view.RelatorioView;
import com.gapso.web.trieda.shared.util.view.TipoContratoComboBox;
import com.gapso.web.trieda.shared.util.view.TitulacaoComboBox;
import com.gapso.web.trieda.shared.util.view.TurnoComboBox;

public class RelatorioProfessoresView extends RelatorioView
	implements RelatorioProfessoresPresenter.Display
{
	
	RelatorioProfessorFiltro filtro;
	TurnoComboBox turnoCB;
	TitulacaoComboBox titulacaoCB;
	TipoContratoComboBox tipoContratoCB;
	CursoComboBox cursoCB;
	AreaTitulacaoComboBox areaTitulacaoCB;
	int numHistogramas = 5;
	
	
	public RelatorioProfessoresView(CenarioDTO cenarioDTO) {
		super(cenarioDTO);
		createHistogramasButton(numHistogramas);
	}
	
	@Override
	protected String getHeadingPanel() {
		return "Master Data » Relatório de Professores";
	}
	
	@Override
	protected GTabItem createGTabItem() {
		return new GTabItem("Relatório de Professores", Resources.DEFAULTS.professor16());
	}
	
	@Override
	protected void setFilter() {
		this.filtro = new RelatorioProfessorFiltro();
		
		turnoCB = new TurnoComboBox(cenarioDTO);
		titulacaoCB = new TitulacaoComboBox();
		tipoContratoCB = new TipoContratoComboBox();
		cursoCB = new CursoComboBox(cenarioDTO);
		areaTitulacaoCB = new AreaTitulacaoComboBox();
	}
	
	@Override
	protected List<Field<?>> getFiltersList() {
		List<Field<?>> listFiltro = new ArrayList<Field<?>>();
		listFiltro.add(turnoCB);
		listFiltro.add(titulacaoCB);
		listFiltro.add(tipoContratoCB);
		listFiltro.add(cursoCB);
		listFiltro.add(areaTitulacaoCB);
		
		return listFiltro;
	}
	
	@Override
	public RelatorioProfessorFiltro getProfessorFiltro()
	{
		return filtro;
	}
	
	@Override
	public TurnoComboBox getTurnoComboBox()
	{
		return turnoCB;
	}
	
	@Override
	public CursoComboBox getCursoComboBox()
	{
		return cursoCB;
	}
	
	@Override
	public TitulacaoComboBox getTitulacaoComboBox()
	{
		return titulacaoCB;
	}
	
	@Override
	public TipoContratoComboBox getTipoContratoComboBox()
	{
		return tipoContratoCB;
	}
	
	@Override
	public AreaTitulacaoComboBox getAreaTitulacaoComboBox()
	{
		return areaTitulacaoCB;
	}
}
