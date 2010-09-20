package com.gapso.web.trieda.client.util.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.data.BaseModel;
import com.extjs.gxt.ui.client.data.BasePagingLoader;
import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoader;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.GridEvent;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnData;
import com.extjs.gxt.ui.client.widget.grid.ColumnModel;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.grid.GridCellRenderer;
import com.extjs.gxt.ui.client.widget.grid.GridSelectionModel;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.google.gwt.user.client.Element;

public class SemanaLetivaGrid<M extends BaseModel> extends ContentPanel {

	private Grid<M> grid;
	private RpcProxy<PagingLoadResult<M>> proxy;
	private PagingLoader<PagingLoadResult<ModelData>> loader;
	private ListStore<M> store;
	
	public SemanaLetivaGrid() {
		super(new FitLayout());
		setHeaderVisible(false);
	}

	@Override
	protected void beforeRender() {
		super.beforeRender();
		
		loader = new BasePagingLoader<PagingLoadResult<ModelData>>(proxy);
		loader.setRemoteSort(true);
		
		store = new ListStore<M>(loader);
		
		grid = new Grid<M>(store, new ColumnModel(getColumnList()));
		grid.setBorders(true);
		grid.setTrackMouseOver(false);
		grid.addStyleName("SemanaLetivaGrid");
		
		grid.getView().setEmptyText("Não existe horários de aula cadastrados no sistema");
		
//		grid.setView(new CustomGridView(new int[]{7, 1}, new int[][]{{0,2}}));
		
		grid.addListener(Events.BeforeSelect, new Listener<GridEvent<M>>() {
			@Override
			public void handleEvent(GridEvent<M> be) {
				be.setCancelled(true);
			}
		});
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
	
	public List<ColumnConfig> getColumnList() {
		
		GridCellRenderer<M> mergeRenderer = new GridCellRenderer<M>() {
			private String lastTurno = "";
			@Override
			public Object render(M model, String property, ColumnData config, int rowIndex, int colIndex, ListStore<M> store, Grid<M> grid) {
				config.style += "border-right: 1px solid #EDEDED;";
				config.style += "border-top: 1px solid #FFF;";
				if((rowIndex + 1) == store.getModels().size()) {
					config.style += "border-bottom: 1px solid #EDEDED;";
				}
				if(model.get(property).equals(lastTurno)) {
					return "";
				}
				lastTurno = model.get(property);
				config.style += "border-top: 1px solid #EDEDED;";
				return lastTurno;
			}  
		};
		
		GridCellRenderer<M> horarioRenderer = new GridCellRenderer<M>() {
			@Override
			public Object render(M model, String property, ColumnData config, int rowIndex, int colIndex, ListStore<M> store, Grid<M> grid) {
				if(rowIndex == 0) {
					config.style += "border-top: 1px solid #EDEDED;";
				}
				if((rowIndex + 1) == store.getModels().size()) {
					config.style += "border-bottom: 1px solid #EDEDED;";
				}
				config.style += "border-right: 1px solid #EDEDED;";
				return model.get(property);
			}  
		};
		
		GridCellRenderer<M> buttonRenderer = new GridCellRenderer<M>() {
			@Override
			public Object render(final M model, final String property, ColumnData config, int rowIndex, int colIndex, ListStore<M> store, Grid<M> grid) {
				if(rowIndex == 0) {
					config.style += "border-top: 1px solid #EDEDED;";
				}
				if((rowIndex + 1) == store.getModels().size()) {
					config.style += "border-bottom: 1px solid #EDEDED;";
				}
				config.style += "border-right: 1px solid #EDEDED;";
				ToggleImageButton tb = new ToggleImageButton((Boolean) model.get(property), Resources.DEFAULTS.save16(), Resources.DEFAULTS.cancel16());
				tb.addSelectionListener(new SelectionListener<ButtonEvent>() {
					@Override
					public void componentSelected(ButtonEvent ce) {
						ToggleImageButton tib = (ToggleImageButton) ce.getComponent();
						model.set(property, tib.isPressed());
					}
				});
				return tb;
			}  
		};
		
		List<ColumnConfig> list = new ArrayList<ColumnConfig>();
		ColumnConfig column = new ColumnConfig("turnoString", "Turno", 120);
		column.setResizable(false);
		column.setMenuDisabled(true);
		column.setSortable(false);
		column.setRenderer(mergeRenderer);
		list.add(column);
		
		column = new ColumnConfig("horarioString", "Horario", 110);
		column.setResizable(false);
		column.setMenuDisabled(true);
		column.setSortable(false);
		column.setRenderer(horarioRenderer);
		list.add(column);
		
		column = new ColumnConfig("segunda", "Seg", 50);
		column.setResizable(false);
		column.setMenuDisabled(true);
		column.setSortable(false);
		column.setRenderer(buttonRenderer);
		list.add(column);
		
		column = new ColumnConfig("terca", "Ter", 50);
		column.setResizable(false);
		column.setMenuDisabled(true);
		column.setSortable(false);
		column.setRenderer(buttonRenderer);
		list.add(column);
		
		column = new ColumnConfig("quarta", "Qua", 50);
		column.setResizable(false);
		column.setMenuDisabled(true);
		column.setSortable(false);
		column.setRenderer(buttonRenderer);
		list.add(column);
		
		column = new ColumnConfig("quinta", "Qui", 50);
		column.setResizable(false);
		column.setMenuDisabled(true);
		column.setSortable(false);
		column.setRenderer(buttonRenderer);
		list.add(column);
		
		column = new ColumnConfig("sexta", "Sex", 50);
		column.setResizable(false);
		column.setMenuDisabled(true);
		column.setSortable(false);
		column.setRenderer(buttonRenderer);
		list.add(column);
		
		column = new ColumnConfig("sabado", "Sab", 50);
		column.setResizable(false);
		column.setMenuDisabled(true);
		column.setSortable(false);
		column.setRenderer(buttonRenderer);
		list.add(column);
		
		column = new ColumnConfig("domingo", "Dom", 50);
		column.setResizable(false);
		column.setMenuDisabled(true);
		column.setSortable(false);
		column.setRenderer(buttonRenderer);
		list.add(column);

		return list;
	}

	
	public GridSelectionModel<M> getSelectionModel() {
		return grid.getSelectionModel();
	}
	
	public void setProxy(RpcProxy<PagingLoadResult<M>> proxy) {
		this.proxy = proxy;
	}
	
	public RpcProxy<PagingLoadResult<M>> getProxy() {
		return this.proxy;
	}

	public ListStore<M> getStore() {
		return store;
	}
	
}
