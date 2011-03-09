package com.gapso.web.trieda.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.toolbar.SeparatorToolItem;
import com.gapso.web.trieda.client.mvp.presenter.SalasPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.CampusComboBox;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.SimpleFilter;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.client.util.view.SimpleToolBar;
import com.gapso.web.trieda.client.util.view.UnidadeComboBox;
import com.gapso.web.trieda.shared.dtos.SalaDTO;

public class SalasView extends MyComposite implements SalasPresenter.Display {

	private SimpleToolBar toolBar;
	private SimpleGrid<SalaDTO> gridPanel;
	private SimpleFilter filter;
	private ContentPanel panel;
	private GTabItem tabItem;
	private Button disciplinasAssociadasBT;
	private Button gruposDeSalasBT;
	private Button disponibilidadeBT;
	private UnidadeComboBox unidadeCB;
	private CampusComboBox campusCB;
	
	public SalasView() {
		initUI();
	}
	
	private void initUI() {
		panel = new ContentPanel(new BorderLayout());
		panel.setHeading("Master Data » Salas");
		createToolBar();
		createGrid();
		createFilter();
		createTabItem();
		initComponent(tabItem);
	}
	
	private void createTabItem() {
		tabItem = new GTabItem("Salas", Resources.DEFAULTS.sala16());
		tabItem.setContent(panel);
	}
	
	private void createToolBar() {
		toolBar = new SimpleToolBar(this);
		toolBar.add(new SeparatorToolItem());
		disciplinasAssociadasBT = toolBar.createButton("Disciplinas Associadas", Resources.DEFAULTS.disciplina16());
		toolBar.add(disciplinasAssociadasBT);
		gruposDeSalasBT = toolBar.createButton("Grupos de Salas", Resources.DEFAULTS.sala16());
		toolBar.add(gruposDeSalasBT);
		disponibilidadeBT = toolBar.createButton("Disponibilidade da Sala", Resources.DEFAULTS.disponibilidade16());
		toolBar.add(disponibilidadeBT);
		panel.setTopComponent(toolBar);
	}
	
	private void createGrid() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
	    bld.setMargins(new Margins(5, 5, 5, 5));
	    
	    gridPanel = new SimpleGrid<SalaDTO>(getColumnList());
	    panel.add(gridPanel, bld);
	}

	public List<ColumnConfig> getColumnList() {
		List<ColumnConfig> list = new ArrayList<ColumnConfig>();
		list.add(new ColumnConfig(SalaDTO.PROPERTY_CODIGO, "Código", 100));
		list.add(new ColumnConfig(SalaDTO.PROPERTY_TIPO_STRING, "Tipo", 100));
		list.add(new ColumnConfig(SalaDTO.PROPERTY_UNIDADE_STRING, "Unidade", 100));
		list.add(new ColumnConfig(SalaDTO.PROPERTY_NUMERO, "Número", 100));
		list.add(new ColumnConfig(SalaDTO.PROPERTY_ANDAR, "Andar", 100));
		list.add(new ColumnConfig(SalaDTO.PROPERTY_CAPACIDADE, "Capacidade", 100));
		return list;
	}

	private void createFilter() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.EAST);
		bld.setMargins(new Margins(5, 5, 5, 0));
		bld.setCollapsible(true);
		
		filter = new SimpleFilter();
		
		campusCB = new CampusComboBox();
		unidadeCB = new UnidadeComboBox(campusCB);
		
		filter.addField(campusCB);
		filter.addField(unidadeCB);
		
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
	public Button getExportExcelButton() {
		return toolBar.getExportExcelButton();
	}

	@Override
	public GTabItem getGTabItem() {
		return tabItem;
	}
	
	@Override
	public SimpleGrid<SalaDTO> getGrid() {
		return gridPanel;
	}

	@Override
	public void setProxy(RpcProxy<PagingLoadResult<SalaDTO>> proxy) {
		gridPanel.setProxy(proxy);
	}

	@Override
	public Button getDisciplinasAssociadasButton() {
		return disciplinasAssociadasBT;
	}

	@Override
	public Button getGruposDeSalasButton() {
		return gruposDeSalasBT;
	}
	
	@Override
	public Button getDisponibilidadeButton() {
		return disponibilidadeBT;
	}
	
	@Override
	public UnidadeComboBox getUnidadeCB() {
		return unidadeCB;
	}

	@Override
	public CampusComboBox getCampusCB() {
		return campusCB;
	}

	@Override
	public Button getSubmitBuscaButton() {
		return filter.getSubmitButton();
	}
	
	@Override
	public Button getResetBuscaButton() {
		return filter.getResetButton();
	}
	
}
