package com.gapso.web.trieda.client.util.view.simplecrud;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.SelectionMode;
import com.extjs.gxt.ui.client.data.BasePagingLoader;
import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoader;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.grid.CheckColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnModel;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.toolbar.PagingToolBar;
import com.gapso.web.trieda.client.services.Services;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class SimpleCrudGrid<M extends ModelData> extends ContentPanel {

	private Grid<M> grid;
	private PagingLoader<PagingLoadResult<M>> loader;
	private AbstractCrudModel crudModel;
	
	public SimpleCrudGrid(AbstractCrudModel crudModel) {
		super(new FitLayout());
		this.crudModel = crudModel;
		init();
	}
	
	private void init() {
		final ICrudService service = (ICrudService)Services.get(crudModel.getServiceId());
		
		RpcProxy<PagingLoadResult<ModelData>> proxy = new RpcProxy<PagingLoadResult<ModelData>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<ModelData>> callback) {
				service.getList(callback);
			}
		};
		loader = new BasePagingLoader<PagingLoadResult<M>>(proxy);  
		loader.setRemoteSort(true);  
		ListStore<M> store = new ListStore<M>(loader);  
		grid = new Grid<M>(store, new ColumnModel(columnConfigList()));
		configuration();
		pagingPanel();
		add(grid);
		loader.load();
	}
	
	private void configuration() {
		grid.setStripeRows(true);
		grid.setBorders(true);
		setHeaderVisible(false);
		setListeners();
	}

	private void setListeners() {
		grid.getSelectionModel().setSelectionMode(SelectionMode.MULTI);
	}
	
	private void pagingPanel() {
		PagingToolBar paggingToolBar = new PagingToolBar(50);
		paggingToolBar.bind(loader);
		setBottomComponent(paggingToolBar);
	}
	
	private List<ColumnConfig> columnConfigList() {
		List<ColumnConfig> columnList = new ArrayList<ColumnConfig>();
		for(String id : crudModel.getIds()) {
			ColumnConfig column = null;
			if(crudModel.isBoolean(id)) {
				column = new CheckColumnConfig(id, crudModel.getHeader(id), crudModel.getWidth(id));
			} else {
				column = new ColumnConfig(id, crudModel.getHeader(id), crudModel.getWidth(id));
			}
			column.setToolTip(crudModel.getHeader(id));
			columnList.add(column);
		}
		return columnList;
	}
	
}
