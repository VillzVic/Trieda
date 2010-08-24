package com.gapso.web.trieda.client.util.view.simplecrud;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import com.extjs.gxt.ui.client.Style.SelectionMode;
import com.extjs.gxt.ui.client.data.BasePagingLoader;
import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoader;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.store.GroupingStore;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.form.Time;
import com.extjs.gxt.ui.client.widget.grid.CheckColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnData;
import com.extjs.gxt.ui.client.widget.grid.ColumnModel;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.grid.GridCellRenderer;
import com.extjs.gxt.ui.client.widget.grid.GridGroupRenderer;
import com.extjs.gxt.ui.client.widget.grid.GridSelectionModel;
import com.extjs.gxt.ui.client.widget.grid.GroupColumnData;
import com.extjs.gxt.ui.client.widget.grid.GroupSummaryView;
import com.extjs.gxt.ui.client.widget.grid.SummaryColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.SummaryRenderer;
import com.extjs.gxt.ui.client.widget.grid.SummaryType;
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
		ListStore<M> store = null;  
		if(crudModel.groupBy() == null) {
			store = new ListStore<M>(loader);  
		} else {
			store = new GroupingStore<M>(loader);
			((GroupingStore<M>)store).groupBy(crudModel.groupBy()); 
		}
		final ColumnModel cm = new ColumnModel(columnConfigList());
		grid = new Grid<M>(store, cm);
		
		if(crudModel.groupBy() != null) {
			GroupSummaryView view = new GroupSummaryView();
			view.setShowGroupedColumn(false);
			view.setForceFit(true);
			view.setGroupRenderer(new GridGroupRenderer() {
				public String render(GroupColumnData data) {
					String f = cm.getColumnById(data.field).getHeader();
					String l = data.models.size() == 1 ? "Item" : "Itens";
					return f + ": " + data.group + " (" + data.models.size() + " " + l + ")";
				}
			});
			view.setForceFit(true);  
			view.setShowGroupedColumn(false);  
			grid.setView(view);
		}
		
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
			if(!crudModel.isIdTable(id)) continue;
			ColumnConfig column = null;
			if(crudModel.isBoolean(id)) {
				column = new CheckColumnConfig(id, crudModel.getHeader(id), crudModel.getWidth(id));
			} else {
				if(crudModel.groupBy() != null) {
					column = new SummaryColumnConfig<Integer>(id, crudModel.getHeader(id), crudModel.getWidth(id));
					if(crudModel.isNumber(id)) {
						((SummaryColumnConfig<Double>)column).setSummaryType(SummaryType.SUM);
						((SummaryColumnConfig<Double>)column).setSummaryRenderer(new SummaryRenderer() {
							public String render(Number value, Map<String, Number> data) {
								return (new Integer(value.intValue())).toString();
							}
						});
					}
				} else {
					column = new ColumnConfig(id, crudModel.getHeader(id), crudModel.getWidth(id));
				}
			}
			if(crudModel.isTime(id)) {
				column.setRenderer(new GridCellRenderer<ModelData>() {  
					public Object render(ModelData model, String property, ColumnData config, int rowIndex, int colIndex, ListStore<ModelData> store, Grid<ModelData> grid) {
						Time time = (Time) model.get(property);
						return time.getHour() + ":" + time.getMinutes();
					}
				});
			}
			column.setToolTip(crudModel.getHeader(id));
			columnList.add(column);
		}
		return columnList;
	}
	
	public GridSelectionModel<M> getSelectionModel() {
		return grid.getSelectionModel();
	}
	
}