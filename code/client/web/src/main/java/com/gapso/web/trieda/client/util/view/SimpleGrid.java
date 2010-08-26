package com.gapso.web.trieda.client.util.view;

import java.util.List;

import com.extjs.gxt.ui.client.Style.SelectionMode;
import com.extjs.gxt.ui.client.data.BasePagingLoader;
import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoader;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnModel;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.grid.GridSelectionModel;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.toolbar.PagingToolBar;
import com.gapso.web.trieda.client.util.view.simplecrud.ICrudService;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class SimpleGrid<M extends ModelData> extends ContentPanel {

	private Grid<M> grid;
	private PagingLoader<PagingLoadResult<M>> loader;
	private List<ColumnConfig> columnList;
	private ICrudService service;
	
	public SimpleGrid(List<ColumnConfig> columnList, ICrudService service) {
		super(new FitLayout());
		this.columnList = columnList;
		this.service = service;
		init();
	}
	
	private void init() {
		RpcProxy<PagingLoadResult<ModelData>> proxy = new RpcProxy<PagingLoadResult<ModelData>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<ModelData>> callback) {
				service.getList(callback);
			}
		};
		loader = new BasePagingLoader<PagingLoadResult<M>>(proxy);  
		loader.setRemoteSort(true);
		
		ListStore<M> store = new ListStore<M>(loader);  
		
		grid = new Grid<M>(store, new ColumnModel(columnList));
		
		configuration();
		pagingPanel();
		add(grid);
		loader.load();
	}
	
	private void configuration() {
		grid.setStripeRows(true);
		grid.setBorders(true);
		setHeaderVisible(false);
		grid.getSelectionModel().setSelectionMode(SelectionMode.MULTI);
	}
	
	private void pagingPanel() {
		PagingToolBar paggingToolBar = new PagingToolBar(50);
		paggingToolBar.bind(loader);
		setBottomComponent(paggingToolBar);
	}
	
	public GridSelectionModel<M> getSelectionModel() {
		return grid.getSelectionModel();
	}
	
}
