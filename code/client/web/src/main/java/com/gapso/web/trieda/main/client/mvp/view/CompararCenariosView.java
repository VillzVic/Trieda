package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.data.BaseTreeModel;
import com.extjs.gxt.ui.client.store.TreeStore;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnModel;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.extjs.gxt.ui.client.widget.treegrid.TreeGrid;
import com.extjs.gxt.ui.client.widget.treegrid.TreeGridCellRenderer;
import com.gapso.web.trieda.main.client.mvp.presenter.CompararCenariosPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;

public class CompararCenariosView extends MyComposite
	implements CompararCenariosPresenter.Display
{
	private TreeStore< BaseTreeModel > store = new TreeStore< BaseTreeModel >();;
	private TreeGrid< BaseTreeModel > tree;
	private ContentPanel panel;
	private GTabItem tabItem;
	private List<CenarioDTO> cenarios;
	private SimpleToolBar toolBar;
	
	public CompararCenariosView(
		List<CenarioDTO> cenarios )
	{
		this.cenarios = cenarios;
		initUI();
		createGrid();
		createTabItem();
		initComponent( tabItem );
	}
	
	private void initUI()
	{
		panel = new ContentPanel( new BorderLayout() );
		panel.setHeadingHtml( "Comparar Cenários" );
		toolBar = new SimpleToolBar( false, false, false, false, true, this );
		panel.setTopComponent(toolBar);
	}
	
	@Override
	protected void beforeRender()
	{
		super.beforeRender();
	}
	
	private void createTabItem()
	{
		tabItem = new GTabItem(
			"Comparar Cenários", Resources.DEFAULTS.cenario16() );
	
		tabItem.setContent( panel );
	}
	
	private void createGrid()
	{
		tree = new TreeGrid< BaseTreeModel >(
				getStore(), new ColumnModel( getColumnList() ) );
	    
	    ContentPanel contentPanel = new ContentPanel( new FitLayout() );
	    contentPanel.setHeaderVisible(false);
	    contentPanel.add( tree );
	
	    BorderLayoutData bld = new BorderLayoutData( LayoutRegion.CENTER );
	    bld.setMargins( new Margins( 5 ) );
	    panel.add( contentPanel, bld );
	}
	
	public List< ColumnConfig > getColumnList()
	{
		List< ColumnConfig > list = new ArrayList< ColumnConfig >();
		ColumnConfig campusColumnConfig = new ColumnConfig("label", "",400);
		campusColumnConfig.setRenderer(new TreeGridCellRenderer< BaseTreeModel >());
		list.add(campusColumnConfig);
		for (CenarioDTO cenario : cenarios)
		{
			list.add(new ColumnConfig(cenario.getNome(), cenario.getNome(),170));
		}

		return list;
	}
	
	@Override
	public void setStore( TreeStore< BaseTreeModel > store )
	{
		this.store = store;
	}
	
	@Override
	public TreeStore< BaseTreeModel > getStore()
	{
		return store;
	}
	
	public List<CenarioDTO> getCenarios()
	{
		return cenarios;
	}
	
	@Override
	public TreeGrid< BaseTreeModel > getTree()
	{
		return tree;
	}
	
	@Override
	public MenuItem getExportXlsExcelButton()
	{
		return (MenuItem) this.toolBar.getExportExcelButton().getMenu().getItem(0);
	}
	
	@Override
	public MenuItem getExportXlsxExcelButton()
	{
		return (MenuItem) this.toolBar.getExportExcelButton().getMenu().getItem(1);
	}
}
