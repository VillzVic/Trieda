package com.gapso.web.trieda.client.util.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.SelectionMode;
import com.extjs.gxt.ui.client.data.BaseModel;
import com.extjs.gxt.ui.client.data.BasePagingLoader;
import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoader;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.ComponentPlugin;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnModel;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.grid.GridSelectionModel;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.toolbar.PagingToolBar;
import com.google.gwt.user.client.Element;

public class SimpleGrid<M extends BaseModel> extends ContentPanel {

	private Grid<M> grid;
	RpcProxy<PagingLoadResult<M>> proxy;
	private PagingLoader<PagingLoadResult<ModelData>> loader;
	private List<ColumnConfig> columnList;
	private List<ComponentPlugin> plugins = new ArrayList<ComponentPlugin>();
	
	public SimpleGrid(List<ColumnConfig> columnList) {
		super(new FitLayout());
		this.columnList = columnList;
		setHeaderVisible(false);
	}

	@Override
	protected void beforeRender() {
		super.beforeRender();
		
		loader = new BasePagingLoader<PagingLoadResult<ModelData>>(proxy);
		loader.setRemoteSort(true);
		
		ListStore<M> store = new ListStore<M>(loader);  
		
		grid = new Grid<M>(store, new ColumnModel(columnList));
		grid.setStripeRows(true);
		grid.setBorders(true);
		grid.getSelectionModel().setSelectionMode(SelectionMode.MULTI);
		for(ComponentPlugin plugin : plugins) {
			grid.addPlugin(plugin);
		}
		grid.setBorders(false);
		pagingPanel();
		add(grid);
	}

	@Override
	protected void onRender(Element parent, int pos) {
		super.onRender(parent, pos);
		loader.load();
	}

	public Grid<M> getGrid() {
		return grid;
	}
	
	public void updateList() {
		loader.load();
	}
	
	public void addPlugin(ComponentPlugin plugin) {
		this.plugins.add(plugin);
	}
	
	private void pagingPanel() {
		PagingToolBar paggingToolBar = new PagingToolBar(25);
		paggingToolBar.bind(loader);
		setBottomComponent(paggingToolBar);
	}
	
	public GridSelectionModel<M> getSelectionModel() {
		return grid.getSelectionModel();
	}
	
	public void setProxy(RpcProxy<PagingLoadResult<M>> proxy) {
		this.proxy = proxy;
	}
	
}
