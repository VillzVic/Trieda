package com.gapso.web.trieda.client.mvc.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Registry;
import com.extjs.gxt.ui.client.Style.HorizontalAlignment;
import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.event.EventType;
import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.GridEvent;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.extjs.gxt.ui.client.mvc.View;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.util.Padding;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.ComboBox;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.grid.CellEditor;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnData;
import com.extjs.gxt.ui.client.widget.grid.ColumnModel;
import com.extjs.gxt.ui.client.widget.grid.EditorGrid;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.grid.GridCellRenderer;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.BoxLayout.BoxLayoutPack;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayout;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayoutData;
import com.extjs.gxt.ui.client.widget.toolbar.ToolBar;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.mvc.model.CampusDeslocamentoModel;
import com.gapso.web.trieda.client.mvp.view.AppView;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class CampusDeslocamentoView extends View {

	private GTabItem tabItem;
	
	public CampusDeslocamentoView(Controller controller) {
		super(controller);
	}

	@Override
	public void initialize() {
	}
	
	private void onCampusDeslocamento() {
		tabItem = new GTabItem("Matriz de Tempos de Deslocamento entre Campi", Resources.DEFAULTS.campi16());
		tabItem.setContent(getPanel());
		GTab tab = (GTab)Registry.get(AppView.TAB);
		tab.add(tabItem);
	}
	
	private ContentPanel getPanel() {
		ContentPanel panel = new ContentPanel(new BorderLayout());
		panel.setHeaderVisible(false);
		
		BorderLayoutData bldC = new BorderLayoutData(LayoutRegion.CENTER);
	    bldC.setMargins(new Margins(5, 5, 5, 5));
		panel.add(getMatriz(), bldC);
		
		BorderLayoutData bldE = new BorderLayoutData(LayoutRegion.EAST);
		bldE.setMargins(new Margins(5, 5, 5, 5));
		bldE.setCollapsible(true);
		panel.add(getFiltro(), bldE);
		
		panel.setTopComponent(getToolBar());
		
		return panel;
	}
	
	private ToolBar getToolBar() {
		ToolBar tb = new ToolBar();
		Button bt = new Button("", AbstractImagePrototype.create(Resources.DEFAULTS.save16()));
		bt.setToolTip("Salvar");
		tb.add(bt);
		bt = new Button("", AbstractImagePrototype.create(Resources.DEFAULTS.cancel16()));
		bt.setToolTip("Cancelar");
		tb.add(bt);
		bt = new Button("", AbstractImagePrototype.create(Resources.DEFAULTS.campi16()));
		bt.setToolTip("Igualar o deslocamento");
		tb.add(bt);
		return tb;
	}
	
	private FormPanel getFiltro() {
		FormPanel form = new FormPanel();
		FormData formDataFilter = new FormData("-5");
		form.setWidth(350);
		
		ComboBox<ModelData> estadosCB = new ComboBox<ModelData>();
		estadosCB.setStore(new ListStore<ModelData>());
		estadosCB.setFieldLabel("Estado");
		form.add(estadosCB, formDataFilter);

		TextField<String> municipioTF = new TextField<String>();
		municipioTF.setFieldLabel("Município");
		form.add(municipioTF, formDataFilter);
		
		TextField<String> bairroTF = new TextField<String>();
		bairroTF.setFieldLabel("Bairro");
		form.add(bairroTF, formDataFilter);
		
		LayoutContainer lc = new LayoutContainer();
		HBoxLayout llc = new HBoxLayout();  
		llc.setPadding(new Padding(5));
		llc.setPack(BoxLayoutPack.CENTER);
		lc.setLayout(llc);
		HBoxLayoutData hbld = new HBoxLayoutData(new Margins(0, 5, 0, 0));
		lc.add(new Button("Filtrar", AbstractImagePrototype.create(Resources.SIMPLE_CRUD.filter16())), hbld);
		lc.add(new Button("Limpar",  AbstractImagePrototype.create(Resources.SIMPLE_CRUD.filterClean16())), hbld);  
		form.add(lc);
		return form;
	}
	
	private Grid<CampusDeslocamentoModel> getMatriz() {
		List<ColumnConfig> columnList = new ArrayList<ColumnConfig>();
		
		ColumnConfig column = null;
		column = new ColumnConfig("name", "", 60);
		column.setMenuDisabled(true);
		column.setSortable(false);
		column.setRenderer(new GridCellRenderer<CampusDeslocamentoModel>() {
			@Override
			public Object render(CampusDeslocamentoModel model,
					String property, ColumnData config, int rowIndex,
					int colIndex, ListStore<CampusDeslocamentoModel> store,
					Grid<CampusDeslocamentoModel> grid) {
				config.css = "x-grid3-header";
				if(rowIndex == (colIndex - 1)) return "";
				return model.get(property);
			}
		});
		columnList.add(column);
		
		for(int i = 1; i<=5; i++) {
			column = new ColumnConfig("campus"+i, "Campus "+i, 60);
			column.setMenuDisabled(true);
			column.setSortable(false);
			column.setAlignment(HorizontalAlignment.CENTER);
			NumberField field = new NumberField();
			field.setAllowDecimals(false);
			column.setEditor(new CellEditor(field));
			column.setRenderer(new GridCellRenderer<CampusDeslocamentoModel>() {
				@Override
				public Object render(CampusDeslocamentoModel model, String property, ColumnData config, int rowIndex, int colIndex, ListStore<CampusDeslocamentoModel> store, Grid<CampusDeslocamentoModel> grid) {
					if(rowIndex == (colIndex - 1)) {
						config.style = "background-color: #EBECEE;";
						return "";
					} else {
						config.style = "background-color: #FFFFFF;";
					}
					return null;
				}
			});
			columnList.add(column);
		}
		
		ColumnModel cm = new ColumnModel(columnList);
		ListStore<CampusDeslocamentoModel> store = new ListStore<CampusDeslocamentoModel>();  
		store.add(new CampusDeslocamentoModel("Campus 1", 10.0, 20.0, 30.0, 40.0, 50.0));  
		store.add(new CampusDeslocamentoModel("Campus 2", 10.0, 20.0, 30.0, 40.0, 50.0));  
		store.add(new CampusDeslocamentoModel("Campus 3", 10.0, 20.0, 30.0, 40.0, 50.0));  
		store.add(new CampusDeslocamentoModel("Campus 4", 10.0, 20.0, 30.0, 40.0, 50.0));  
		store.add(new CampusDeslocamentoModel("Campus 5", 10.0, 20.0, 30.0, 40.0, 50.0));  
		EditorGrid<CampusDeslocamentoModel> grid = new EditorGrid<CampusDeslocamentoModel>(store, cm);
		
		grid.addListener(Events.BeforeEdit, new Listener<GridEvent<CampusDeslocamentoModel>>() {
			public void handleEvent(GridEvent<CampusDeslocamentoModel> be) {
				int rowIndex = be.getRowIndex();
				int colIndex = be.getColIndex();
				if (rowIndex == (colIndex - 1)) {
					be.setCancelled(true);
				}
			}
		});
		
		return grid;
	}
	
	
	
	@Override
	protected void handleEvent(AppEvent event) {
		EventType type = event.getType();
		if (type == AppEvents.CampusDeslocamento) onCampusDeslocamento();
	}

}
