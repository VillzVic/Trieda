package com.gapso.web.trieda.client.util.view;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.extjs.gxt.ui.client.Style.HorizontalAlignment;
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
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.google.gwt.user.client.Element;

public class SemanaLetivaDoCenarioGrid<M extends BaseModel> extends ContentPanel {

	private Grid<M> grid;
	private RpcProxy<PagingLoadResult<M>> proxy;
	private PagingLoader<PagingLoadResult<ModelData>> loader;
	private List<M> horariosDisponiveisDisponivel;
	
	private Map<String, List<ToggleImageButton>> comboboxWeek;
	
	private ToggleImageButton segCB;
	private ToggleImageButton terCB;
	private ToggleImageButton quaCB;
	private ToggleImageButton quiCB;
	private ToggleImageButton sexCB;
	private ToggleImageButton sabCB;
	private ToggleImageButton domCB;
	
	private boolean selectDefault = false;
	
	private String horarioAulaIdPropertyName;
	
	public SemanaLetivaDoCenarioGrid(List<M> horariosDisponiveisDisponivel, String horarioAulaIdPropertyName) {
		super(new FitLayout());
		this.horariosDisponiveisDisponivel = horariosDisponiveisDisponivel;
		this.horarioAulaIdPropertyName = horarioAulaIdPropertyName;
		setHeaderVisible(false);
		setBodyBorder(false);
		createCheckboxs();
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
		
		createComboboxWeek();
		
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
				if(!(Boolean)modelCenario.get(property)) { return ""; }
				
				M model = getModel(modelCenario);
				Boolean flag = (model == null) ? false : (Boolean) model.get(property);
				modelCenario.set(property, (model == null)? false : model.get(property));
				
				if(isSelectDefault()) {
					modelCenario.set(property, true);
					flag = true;
				}
				
				ToggleImageButton tb = new ToggleImageButton(flag, Resources.DEFAULTS.save16(), Resources.DEFAULTS.cancel16());
				tb.addSelectionListener(new SelectionListener<ButtonEvent>() {
					@Override
					public void componentSelected(ButtonEvent ce) {
						ToggleImageButton tib = (ToggleImageButton) ce.getComponent();
						modelCenario.set(property, tib.isPressed());
					}
				});
				comboboxWeek.get(property).add(tb);
				return tb;
			}  
		};
		
		List<ColumnConfig> list = new ArrayList<ColumnConfig>(9);
		
		list.add(createColumnConfig("turnoString", "Turno", 100, mergeRenderer, null));
		list.add(createColumnConfig("horarioString", "Horario", 90, horarioRenderer, null));
		list.add(createColumnConfig("segunda", "Seg", 55, buttonRenderer, segCB));
		list.add(createColumnConfig("terca",   "Ter", 55, buttonRenderer, terCB));
		list.add(createColumnConfig("quarta",  "Qua", 55, buttonRenderer, quaCB));
		list.add(createColumnConfig("quinta",  "Qui", 55, buttonRenderer, quiCB));
		list.add(createColumnConfig("sexta",   "Sex", 55, buttonRenderer, sexCB));
		list.add(createColumnConfig("sabado",  "Sab", 55, buttonRenderer, sabCB));
		list.add(createColumnConfig("domingo", "Dom", 55, buttonRenderer, domCB));

		return list;
	}
	
	
	private void createComboboxWeek() {
		comboboxWeek = new HashMap<String, List<ToggleImageButton>>();
		comboboxWeek.put("segunda", new ArrayList<ToggleImageButton>());
		comboboxWeek.put("terca", new ArrayList<ToggleImageButton>());
		comboboxWeek.put("quarta", new ArrayList<ToggleImageButton>());
		comboboxWeek.put("quinta", new ArrayList<ToggleImageButton>());
		comboboxWeek.put("sexta", new ArrayList<ToggleImageButton>());
		comboboxWeek.put("sabado", new ArrayList<ToggleImageButton>());
		comboboxWeek.put("domingo", new ArrayList<ToggleImageButton>());
	}
	
	private M getModel(M model) {
		if(model != null) {
			Long horarioDeAulaId = model.get(horarioAulaIdPropertyName);
			for(M o : horariosDisponiveisDisponivel) {
				if(o.get(horarioAulaIdPropertyName).equals(horarioDeAulaId)) {
					return o;
				}
			}
		}
		return null;		
	}
	
	private ColumnConfig createColumnConfig(String id, String name, Integer width, GridCellRenderer<M> gcr, ToggleImageButton checkbox) {
		ColumnConfig c = new ColumnConfig(id, name, width);
		c.setResizable(false);
		c.setMenuDisabled(true);
		c.setSortable(false);
		c.setRenderer(gcr);
		if(checkbox != null) {
			c.setAlignment(HorizontalAlignment.LEFT);
			c.setWidget(checkbox, name);
		}
		return c;
	}
	
	private void createCheckboxs() {
		segCB = createCheckboxs("Seg");
		terCB = createCheckboxs("Ter");
		quaCB = createCheckboxs("Qua");
		quiCB = createCheckboxs("Qui");
		sexCB = createCheckboxs("Sex");
		sabCB = createCheckboxs("Sab");
		domCB = createCheckboxs("Dom");
	}

	private ToggleImageButton createCheckboxs(String text) {
		ToggleImageButton cb = new ToggleImageButton(true, Resources.DEFAULTS.checkBoxChecked(), Resources.DEFAULTS.checkBoxNotChecked());
		cb.setText("<span style='padding-left: 15px;'>"+text+"</span>");
		cb.addSelectionListener(getSelectionListener());
		return cb;
	}
	
	private SelectionListener<ButtonEvent> getSelectionListener() {
		return new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				ToggleImageButton cb = (ToggleImageButton) ce.getSource();
				if(cb.equals(segCB)) selectWeek("segunda", cb.isPressed());
				else if(cb.equals(terCB)) selectWeek("terca", cb.isPressed());
				else if(cb.equals(quaCB)) selectWeek("quarta", cb.isPressed());
				else if(cb.equals(quiCB)) selectWeek("quinta", cb.isPressed());
				else if(cb.equals(sexCB)) selectWeek("sexta", cb.isPressed());
				else if(cb.equals(sabCB)) selectWeek("sabado", cb.isPressed());
				else if(cb.equals(domCB)) selectWeek("domingo", cb.isPressed());
			}
		};
	}
	
	public void selectWeek(String week, boolean flag) {
		for(ToggleImageButton tb : comboboxWeek.get(week)) {
			tb.toggle(flag);
			tb.fireEvent(Events.Select);
		}
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

	public boolean isSelectDefault() {
		return selectDefault;
	}
	public void setSelectDefault(boolean selectDefault) {
		this.selectDefault = selectDefault;
	}
	
}
