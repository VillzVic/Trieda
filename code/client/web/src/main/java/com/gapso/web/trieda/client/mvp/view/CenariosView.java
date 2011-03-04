package com.gapso.web.trieda.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.grid.CheckColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.toolbar.SeparatorToolItem;
import com.gapso.web.trieda.client.mvp.presenter.CenariosPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.SimpleFilter;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.client.util.view.SimpleToolBar;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;

public class CenariosView extends MyComposite implements CenariosPresenter.Display {

	private SimpleToolBar toolBar;
	private SimpleGrid<CenarioDTO> gridPanel;
	private Button abrirCenarioBT;
	private Button clonarCenarioBT;
	private SimpleFilter filter;
	private TextField<Integer> anoBuscaTextField;
	private TextField<Integer> semestreBuscaTextField;
	private ContentPanel panel;
	private GTabItem tabItem;
	
	public CenariosView() {
		initUI();
	}
	
	private void initUI() {
		panel = new ContentPanel(new BorderLayout());
		panel.setHeading("Master Data » Cenários");
		createToolBar();
		createGrid();
		createFilter();
		createTabItem();
		initComponent(tabItem);
	}
	
	private void createTabItem() {
		tabItem = new GTabItem("Cenário", Resources.DEFAULTS.cenario16());
		tabItem.setContent(panel);
	}
	
	private void createToolBar() {
		toolBar = new SimpleToolBar(this);
		toolBar.add(new SeparatorToolItem());
		clonarCenarioBT = toolBar.createButton("Clonar cenário", Resources.DEFAULTS.cenarioClonar16());
		toolBar.add(clonarCenarioBT);
		abrirCenarioBT = toolBar.createButton("Adicionar na árvore de cenários", Resources.DEFAULTS.cenarioAbrir16());
		toolBar.add(abrirCenarioBT);
		panel.setTopComponent(toolBar);
	}
	
	private void createGrid() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
	    bld.setMargins(new Margins(5, 5, 5, 5));
	    
	    gridPanel = new SimpleGrid<CenarioDTO>(getColumnList());
	    panel.add(gridPanel, bld);
	}

	public List<ColumnConfig> getColumnList() {
		List<ColumnConfig> list = new ArrayList<ColumnConfig>();
		list.add(new CheckColumnConfig(CenarioDTO.PROPERTY_OFICIAL, "Oficial", 40));
		list.add(new ColumnConfig(CenarioDTO.PROPERTY_NOME, "Nome", 100));
		list.add(new ColumnConfig(CenarioDTO.PROPERTY_ANO, "Ano", 100));
		list.add(new ColumnConfig(CenarioDTO.PROPERTY_SEMESTRE, "Semestre", 100));
		list.add(new ColumnConfig(CenarioDTO.PROPERTY_SEMANA_LETIVA_STRING, "Semana Letiva", 100));
		return list;
	}

	private void createFilter() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.EAST);
		bld.setMargins(new Margins(5, 5, 5, 0));
		bld.setCollapsible(true);
		
		filter = new SimpleFilter();
		anoBuscaTextField = new TextField<Integer>();
		anoBuscaTextField.setFieldLabel("Ano");
		semestreBuscaTextField = new TextField<Integer>();
		semestreBuscaTextField.setFieldLabel("Semestre");
		filter.addField(anoBuscaTextField);
		filter.addField(semestreBuscaTextField); 
		
		panel.add(filter, bld);
	}
	
	@Override
	public Button getNewButton() {
		return toolBar.getNewButton();
	}

	@Override
	public Button getEditButton() {
		return toolBar.getEditButton();
	}

	@Override
	public Button getRemoveButton() {
		return toolBar.getRemoveButton();
	}

	@Override
	public SimpleGrid<CenarioDTO> getGrid() {
		return gridPanel;
	}

	@Override
	public void setProxy(RpcProxy<PagingLoadResult<CenarioDTO>> proxy) {
		gridPanel.setProxy(proxy);
	}
	
	@Override
	public TextField<Integer> getAnoBuscaTextField() {
		return anoBuscaTextField;
	}

	@Override
	public TextField<Integer> getSemestreBuscaTextField() {
		return semestreBuscaTextField;
	}

	@Override
	public Button getSubmitBuscaButton() {
		return filter.getSubmitButton();
	}

	@Override
	public Button getResetBuscaButton() {
		return filter.getResetButton();
	}

	@Override
	public Button getAbrirCenarioButton() {
		return abrirCenarioBT;
	}

	@Override
	public Button getClonarCenarioButton() {
		return clonarCenarioBT;
	}
	
}
