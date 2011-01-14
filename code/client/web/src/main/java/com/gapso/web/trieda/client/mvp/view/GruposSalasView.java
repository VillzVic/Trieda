package com.gapso.web.trieda.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.core.XTemplate;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.RowExpander;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.toolbar.SeparatorToolItem;
import com.gapso.web.trieda.client.mvp.model.GrupoSalaDTO;
import com.gapso.web.trieda.client.mvp.presenter.GruposSalasPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.client.util.view.SimpleToolBar;

public class GruposSalasView extends MyComposite implements GruposSalasPresenter.Display {

	private SimpleToolBar toolBar;
	private SimpleGrid<GrupoSalaDTO> gridPanel;
	private ContentPanel panel;
	private GTabItem tabItem;
	private Button associarSalasBT;
	private RowExpander expander;
	
	public GruposSalasView() {
		initUI();
	}
	
	private void initUI() {
		panel = new ContentPanel(new BorderLayout());
		panel.setHeading("Master Data » Grupos de Salas");
		createToolBar();
		createGrid();
		createTabItem();
		initComponent(tabItem);
	}
	
	private void createTabItem() {
		tabItem = new GTabItem("Grupos de Salas", Resources.DEFAULTS.grupoSala16());
		tabItem.setContent(panel);
	}
	
	private void createToolBar() {
		toolBar = new SimpleToolBar();
		toolBar.add(new SeparatorToolItem());
		associarSalasBT = toolBar.createButton("Associar Salas", Resources.DEFAULTS.sala16());
		toolBar.add(associarSalasBT);
		panel.setTopComponent(toolBar);
	}
	
	private void createGrid() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
	    bld.setMargins(new Margins(5, 5, 5, 5));
	    gridPanel = new SimpleGrid<GrupoSalaDTO>(getColumnList());
	    panel.add(gridPanel, bld);
	    gridPanel.addPlugin(expander);
	}

	public List<ColumnConfig> getColumnList() {
		List<ColumnConfig> list = new ArrayList<ColumnConfig>();
	    XTemplate tpl = XTemplate.create("<p><b>Salas:</b></p><p>{salasString}</p>");  
	    expander = new RowExpander();  
	    expander.setTemplate(tpl);
	    list.add(expander);
		list.add(new ColumnConfig("codigo", "Código", 100));
		list.add(new ColumnConfig("nome", "Nome", 100));
		list.add(new ColumnConfig("unidadeString", "Unidade", 100));
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
	public SimpleGrid<GrupoSalaDTO> getGrid() {
		return gridPanel;
	}

	@Override
	public void setProxy(RpcProxy<PagingLoadResult<GrupoSalaDTO>> proxy) {
		gridPanel.setProxy(proxy);
	}

	@Override
	public Button getAssociarSalasButton() {
		return associarSalasBT;
	}
	
}
