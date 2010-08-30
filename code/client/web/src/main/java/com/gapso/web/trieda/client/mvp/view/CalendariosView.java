package com.gapso.web.trieda.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.toolbar.SeparatorToolItem;
import com.gapso.web.trieda.client.mvp.model.CalendarioDTO;
import com.gapso.web.trieda.client.mvp.presenter.CalendariosPresenter;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.client.util.view.SimpleToolBar;
import com.gapso.web.trieda.client.util.view.simplecrud.ISimpleGridService;

public class CalendariosView extends MyComposite implements CalendariosPresenter.Display {

	private SimpleToolBar toolBar;
	private SimpleGrid<CalendarioDTO> gridPanel;
	private ContentPanel panel;
	private GTabItem tabItem;
	private Button diasDeAulaButton;
	
	public CalendariosView() {
		initUI();
	}
	
	private void initUI() {
		panel = new ContentPanel(new BorderLayout());
		panel.setHeading("Master Data » Calendários");
		createToolBar();
		createGrid();
		createTabItem();
		initComponent(tabItem);
	}
	
	private void createTabItem() {
		tabItem = new GTabItem("Calendarios", Resources.DEFAULTS.calendario16());
		tabItem.setContent(panel);
	}
	
	private void createToolBar() {
		toolBar = new SimpleToolBar();
		toolBar.add(new SeparatorToolItem());
		diasDeAulaButton = toolBar.createButton("Dias de Aula", Resources.DEFAULTS.periodoDeAula16());
		toolBar.add(diasDeAulaButton);
		panel.setTopComponent(toolBar);
	}
	
	private void createGrid() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
	    bld.setMargins(new Margins(5, 5, 5, 5));
	    
	    ISimpleGridService service = Services.calendarios();
	    	    
	    gridPanel = new SimpleGrid<CalendarioDTO>(getColumnList(), service);
	    panel.add(gridPanel, bld);
	}

	public List<ColumnConfig> getColumnList() {
		List<ColumnConfig> list = new ArrayList<ColumnConfig>();
		list.add(new ColumnConfig("codigo", "Codigo", 100));
		list.add(new ColumnConfig("descricao", "Descrição", 100));
		return list;
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
	public SimpleGrid<CalendarioDTO> getGrid() {
		return gridPanel;
	}

	@Override
	public Button getDiasDeAulaButton() {
		return diasDeAulaButton;
	}

}
