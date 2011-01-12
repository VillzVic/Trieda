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

public class SemanaLetivaDoCenarioGrid<M extends BaseModel> extends ContentPanel {

	private Grid<M> grid;
	private RpcProxy<PagingLoadResult<M>> proxy;
	private PagingLoader<PagingLoadResult<ModelData>> loader;
	private List<M> horariosDisponiveisDisponivel;
	
	public SemanaLetivaDoCenarioGrid(List<M> horariosDisponiveisDisponivel) {
		super(new FitLayout());
		this.horariosDisponiveisDisponivel = horariosDisponiveisDisponivel;
		setHeaderVisible(false);
		setBodyBorder(false);
	}

	@Override
	protected void beforeRender() {
		super.beforeRender();
		
		loader = new BasePagingLoader<PagingLoadResult<ModelData>>(proxy);
		loader.setRemoteSort(true);
		
		ListStore<M> store = new ListStore<M>(loader);
		
		grid = new Grid<M>(store, new ColumnModel(getColumnList()));
		
		grid.setBorders(true);
		grid.setTrackMouseOver(false);
		grid.addStyleName("SemanaLetivaGrid");
		
		grid.getView().setEmptyText("Não existe horários na Semana Letiva");
		
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
			public Object render(final M modelCenario, final String property, ColumnData config, int rowIndex, int colIndex, ListStore<M> store, Grid<M> grid) {
				if(rowIndex == 0) {
					config.style += "border-top: 1px solid #EDEDED;";
				}
				if((rowIndex + 1) == store.getModels().size()) {
					config.style += "border-bottom: 1px solid #EDEDED;";
				}
				config.style += "border-right: 1px solid #EDEDED;";
				final M model = getModel(modelCenario);
				Boolean flag = false;
				if(model != null) {
					flag = (Boolean) model.get(property);
				}
				ToggleImageButton tb = new ToggleImageButton(flag, Resources.DEFAULTS.save16(), Resources.DEFAULTS.cancel16());
				tb.setEnabled((Boolean)modelCenario.get(property));
				modelCenario.set(property, (model == null)? false : model.get(property));
				tb.addSelectionListener(new SelectionListener<ButtonEvent>() {
					@Override
					public void componentSelected(ButtonEvent ce) {
						ToggleImageButton tib = (ToggleImageButton) ce.getComponent();
						modelCenario.set(property, tib.isPressed());
					}
				});
				if(!tb.isEnabled()) return "";
				return tb;
			}  
		};
		
		List<ColumnConfig> list = new ArrayList<ColumnConfig>(9);
		
		list.add(createColumnConfig("turnoString", "Turno", 120, mergeRenderer));
		list.add(createColumnConfig("horarioString", "Horario", 110, horarioRenderer));
		list.add(createColumnConfig("segunda", "Seg", 50, buttonRenderer));
		list.add(createColumnConfig("terca", "Ter", 50, buttonRenderer));
		list.add(createColumnConfig("quarta", "Qua", 50, buttonRenderer));
		list.add(createColumnConfig("quinta", "Qui", 50, buttonRenderer));
		list.add(createColumnConfig("sexta", "Sex", 50, buttonRenderer));
		list.add(createColumnConfig("sabado", "Sab", 50, buttonRenderer));
		list.add(createColumnConfig("domingo", "Dom", 50, buttonRenderer));

		return list;
	}
	
	private M getModel(M model) {
		if(model != null) {
			Long horarioDeAulaId = model.get("horarioDeAulaId");
			for(M o : horariosDisponiveisDisponivel) {
				if(o.get("horarioDeAulaId").equals(horarioDeAulaId)) {
					return o;
				}
			}
		}
		return null;		
	}
	
	private ColumnConfig createColumnConfig(String id, String name, Integer width, GridCellRenderer<M> gcr) {
		ColumnConfig c = new ColumnConfig(id, name, width);
		c.setResizable(false);
		c.setMenuDisabled(true);
		c.setSortable(false);
		c.setRenderer(gcr);
		return c;
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
		return grid.getStore();
	}
	
	public void updateList() {
		loader.load();
	}
	
}
