package com.gapso.web.trieda.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.Composite;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.gapso.web.trieda.client.mvp.model.TurnoModel;
import com.gapso.web.trieda.client.mvp.presenter.TurnosPresenter;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.client.util.view.SimpleToolBar;
import com.gapso.web.trieda.client.util.view.simplecrud.ICrudService;

public class TurnosView extends Composite implements TurnosPresenter.Display {

	private SimpleToolBar toolBar;
	private ContentPanel gridPanel;
	private ContentPanel panel;
	private GTabItem tabItem;
	
	public TurnosView() {
		initUI();
	}
	
	private void initUI() {
		panel = new ContentPanel(new BorderLayout());
		createToolBar();
		createGrid();
		createTabItem();
		initComponent(panel);
	}
	
//	private void createWest() {
//		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.WEST);
//	    bld.setMargins(new Margins(5, 0, 5, 5));
//	    bld.setCollapsible(true);
//	    bld.setFloatable(true);
//	    
//	    treePanel = new GTreePanel();
//	    panel.add(treePanel, bld);
//	}
	
	private void createTabItem() {
		tabItem = new GTabItem("Turnos", Resources.DEFAULTS.turnos16());
		tabItem.setContent(panel);
	}
	
	private void createToolBar() {
		toolBar = new SimpleToolBar();
		panel.setTopComponent(toolBar);
	}
	
	private void createGrid() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
	    bld.setMargins(new Margins(5, 5, 5, 5));
	    
	    ICrudService service = (ICrudService)Services.get(Services.TURNOS);
	    
	    gridPanel = new SimpleGrid<TurnoModel>(getColumnList(), service);
	    panel.add(gridPanel, bld);
	}

	public List<ColumnConfig> getColumnList() {
		List<ColumnConfig> list = new ArrayList<ColumnConfig>();
		list.add(new ColumnConfig("nome", "Nome", 100));
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
	public Button getDeleteButton() {
		return toolBar.getDeleteButton();
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
	public Button getGrid() {
		// TODO Auto-generated method stub
		return null;
	}
	
}
