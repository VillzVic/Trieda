package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.extjs.gxt.ui.client.widget.toolbar.SeparatorToolItem;
import com.gapso.web.trieda.main.client.mvp.presenter.UnidadesPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleFilter;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;

public class UnidadesView extends MyComposite implements UnidadesPresenter.Display {

	private SimpleToolBar toolBar;
	private SimpleGrid<UnidadeDTO> gridPanel;
	private SimpleFilter filter;
	private TextField<String> nomeBuscaTextField;
	private TextField<String> codigoBuscaTextField;
	private CampusComboBox campusBuscaComboBox;
	private ContentPanel panel;
	private GTabItem tabItem;
	private Button deslocamentoUnidadesBT;
	private Button salasBT;
	private Button disponibilidadeBT;
	private CenarioDTO cenarioDTO;
	
	public UnidadesView( CenarioDTO cenarioDTO ) {
		this.cenarioDTO = cenarioDTO;
		initUI();
	}
	
	private void initUI() {
		panel = new ContentPanel(new BorderLayout());
		panel.setHeading(getI18nConstants().unidadesHeadingPanel());
		createToolBar();
		createGrid();
		createFilter();
		createTabItem();
		initComponent(tabItem);
	}
	
	private void createTabItem() {
		tabItem = new GTabItem(getI18nConstants().unidades(), Resources.DEFAULTS.unidade16());
		tabItem.setContent(panel);
	}
	
	private void createToolBar() {
		toolBar = new SimpleToolBar(this);
		toolBar.add(new SeparatorToolItem());
		deslocamentoUnidadesBT = toolBar.createButton(getI18nConstants().deslocamentoUnidades(), Resources.DEFAULTS.deslocamentoUnidade16());
		toolBar.add(deslocamentoUnidadesBT);
		salasBT = toolBar.createButton(getI18nConstants().salas(), Resources.DEFAULTS.sala16());
		toolBar.add(salasBT);
		disponibilidadeBT = toolBar.createButton(getI18nConstants().disponibilidadesSemanaLetiva(), Resources.DEFAULTS.disponibilidade16());
		toolBar.add(disponibilidadeBT);
		panel.setTopComponent(toolBar);
	}
	
	private void createGrid() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
	    bld.setMargins(new Margins(5, 5, 5, 5));
	    
	    gridPanel = new SimpleGrid<UnidadeDTO>(getColumnList(), this);
	    panel.add(gridPanel, bld);
	}

	public List<ColumnConfig> getColumnList() {
		List<ColumnConfig> list = new ArrayList<ColumnConfig>();
		list.add(new ColumnConfig(UnidadeDTO.PROPERTY_CODIGO, getI18nConstants().codigoUnidade(), 100));
		list.add(new ColumnConfig(UnidadeDTO.PROPERTY_NOME, getI18nConstants().nome(), 100));
		list.add(new ColumnConfig(UnidadeDTO.PROPERTY_CAMPUS_STRING, getI18nConstants().codigoCampus(), 100));
		list.add(new ColumnConfig(UnidadeDTO.PROPERTY_CAPACIDADE_SALAS, getI18nConstants().capacidadeMediaSalas(), 200));
		return list;
	}

	private void createFilter() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.EAST);
		bld.setMargins(new Margins(5, 5, 5, 0));
		bld.setCollapsible(true);
		
		filter = new SimpleFilter();
		campusBuscaComboBox = new CampusComboBox(cenarioDTO);
		campusBuscaComboBox.setFieldLabel(getI18nConstants().campus());
		nomeBuscaTextField = new TextField<String>();
		nomeBuscaTextField.setFieldLabel(getI18nConstants().nome());
		codigoBuscaTextField = new TextField<String>();
		codigoBuscaTextField.setFieldLabel(getI18nConstants().codigo());
		
		filter.addField(campusBuscaComboBox);
		filter.addField(nomeBuscaTextField);
		filter.addField(codigoBuscaTextField);
		
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
	public Button getImportExcelButton() {
		return toolBar.getImportExcelButton();
	}

	@Override
	public MenuItem getExportXlsExcelButton() {
		return (MenuItem) toolBar.getExportExcelButton().getMenu().getItem(0);
	}
	
	@Override
	public MenuItem getExportXlsxExcelButton() {
		return (MenuItem) toolBar.getExportExcelButton().getMenu().getItem(1);
	}
	
	@Override
	public SimpleGrid<UnidadeDTO> getGrid() {
		return gridPanel;
	}
	
	@Override
	public void setProxy(RpcProxy<PagingLoadResult<UnidadeDTO>> proxy) {
		gridPanel.setProxy(proxy);
	}

	@Override
	public TextField<String> getNomeBuscaTextField() {
		return nomeBuscaTextField;
	}

	@Override
	public TextField<String> getCodigoBuscaTextField() {
		return codigoBuscaTextField;
	}
	
	@Override
	public CampusComboBox getCampusBuscaComboBox() {
		return campusBuscaComboBox;
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
	public Button getDeslocamentoUnidadesButton() {
		return deslocamentoUnidadesBT;
	}

	@Override
	public Button getSalasButton() {
		return salasBT;
	}

	@Override
	public Button getDisponibilidadeButton() {
		return disponibilidadeBT;
	}
}
