package com.gapso.web.trieda.client.mvc.view;

import com.extjs.gxt.ui.client.Registry;
import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.Style.Orientation;
import com.extjs.gxt.ui.client.data.BaseListLoader;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.ListLoader;
import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.event.ButtonEvent;
import com.extjs.gxt.ui.client.event.SelectionListener;
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.extjs.gxt.ui.client.mvc.View;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.ListView;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.RowData;
import com.extjs.gxt.ui.client.widget.layout.RowLayout;
import com.extjs.gxt.ui.client.widget.toolbar.ToolBar;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.mvc.model.AreaTitulacaoModel;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.simplecrud.ICrudService;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class CursoAreaTitulacaoView extends View {

	private GTabItem tabItem;
	private ContentPanel panel;
	private ListView<AreaTitulacaoModel> notContainsList;
	private ListView<AreaTitulacaoModel> containsList;
	private TextField<String> cursoField;
	
	public CursoAreaTitulacaoView(Controller controller) {
		super(controller);
	}

	@Override
	public void initialize() {
	}
	
	private void initList() {
		tabItem = new GTabItem("Vincular Áreas de Titulação", Resources.DEFAULTS.areasDeTitulacao16());
		tabItem.setContent(getPanel());
		
		GTab tab = (GTab)Registry.get(AppView.TAB);
		tab.add(tabItem);
	}
	
	private ContentPanel getPanel() {
		ContentPanel panelLists = new ContentPanel(new RowLayout(Orientation.HORIZONTAL));
		panelLists.setHeaderVisible(false);
		
		ContentPanel notContainsListPanel = new ContentPanel(new FitLayout());
		notContainsListPanel.setHeading("Áreas de Titulação NÃO vinculadas ao curso");
		notContainsListPanel.add(getNotContainsList());
		
		ContentPanel containsListPanel = new ContentPanel(new FitLayout());
		containsListPanel.setHeading("Áreas de Titulação vinculadas ao curso");
		containsListPanel.add(getContainsList());
		
		panelLists.add(notContainsListPanel, new RowData(.5, 1, new Margins(0, 0, 0, 0)));
		panelLists.add(getButtonsPanel(), new RowData(42, 1, new Margins(0, 5, 0, 5)));
		panelLists.add(containsListPanel, new RowData(.5, 1, new Margins(0, 0, 0, 0)));
		
		FormPanel panelField = new FormPanel();
		panelField.setHeaderVisible(false);
		panelField.setPadding(2);
		FormData formDataFilter = new FormData("-5");
		TextField<String> field = getCursoField();
		field.setFieldLabel("Curso");
		field.setLabelStyle("text-align: right;");
		panelField.add(field, formDataFilter);
		panelField.add(new Button());		
		panelField.setCollapsible(true);
		
		
		panel = new ContentPanel(new BorderLayout());
		panel.setHeaderVisible(false);
		
		panel.setTopComponent(getToolBar());
		panel.add(panelField, new BorderLayoutData(LayoutRegion.NORTH, 30));
		panel.add(panelLists, new BorderLayoutData(LayoutRegion.CENTER));
		
		return panel;
	}
	
	private ListView<AreaTitulacaoModel> getNotContainsList() {
		if(notContainsList == null) {
			final ICrudService service = (ICrudService)Services.get(Services.AREASTITULACAO);
			RpcProxy<PagingLoadResult<ModelData>> proxy = new RpcProxy<PagingLoadResult<ModelData>>() {
				@Override
				public void load(Object loadConfig, AsyncCallback<PagingLoadResult<ModelData>> callback) {
					service.getList(callback);
				}
			};
			ListLoader<ListLoadResult<AreaTitulacaoModel>> loader = new BaseListLoader<ListLoadResult<AreaTitulacaoModel>>(proxy);
			loader.load();
			ListStore<AreaTitulacaoModel> store = new ListStore<AreaTitulacaoModel>(loader);
			
			notContainsList = new ListView<AreaTitulacaoModel>(store);
			notContainsList.setDisplayProperty("codigo");
		}
		return notContainsList;
	}
	
	private ListView<AreaTitulacaoModel> getContainsList() {
		if(containsList == null) {
			final ICrudService service = (ICrudService)Services.get(Services.AREASTITULACAO);
			RpcProxy<PagingLoadResult<ModelData>> proxy = new RpcProxy<PagingLoadResult<ModelData>>() {
				@Override
				public void load(Object loadConfig, AsyncCallback<PagingLoadResult<ModelData>> callback) {
					service.getList(callback);
				}
			};
			ListLoader<ListLoadResult<AreaTitulacaoModel>> loader = new BaseListLoader<ListLoadResult<AreaTitulacaoModel>>(proxy);
			loader.load();
			ListStore<AreaTitulacaoModel> store = new ListStore<AreaTitulacaoModel>(loader);
			
			containsList = new ListView<AreaTitulacaoModel>(store);
			containsList.setDisplayProperty("codigo");
		}
		return containsList;
	}
	
	private LayoutContainer getButtonsPanel() {
		ContentPanel panel = new ContentPanel();
		panel.setHeaderVisible(false);
		panel.setBodyBorder(false);
		panel.setBodyStyle("display: table-cell; vertical-align: middle");
		panel.setLayout(new RowLayout(Orientation.VERTICAL));
		
		Button toRightBt = new Button();
		toRightBt.setSize(30, 50);
		toRightBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.toRight24()));
		
		Button toLeftBt = new Button();
		toLeftBt.setSize(30, 50);
		toLeftBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.toLeft24()));
		
		RowData rowData = new RowData(-1, -1, new Margins(4, 0, 4, 0));
		
		panel.add(toRightBt, rowData);
		panel.add(toLeftBt, rowData);
		
		return panel;
	}
	
	private ToolBar getToolBar() {
		// EXPORTAR PARA EXCEL
		Button exportarExcelBt = new Button("", AbstractImagePrototype.create(Resources.SIMPLE_CRUD.excelExport16()), new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				MessageBox.alert("Desenvolvimento", "Esta sessão está em desenvolvimento", null);
			}
		});
		exportarExcelBt.setToolTip("Exportar para Excel");
		
		// IMPORTAR PARA EXCEL
		Button importarExcelBt = new Button("", AbstractImagePrototype.create(Resources.SIMPLE_CRUD.excelImport16()), new SelectionListener<ButtonEvent>() {
			@Override
			public void componentSelected(ButtonEvent ce) {
				MessageBox.alert("Desenvolvimento", "Esta sessão está em desenvolvimento", null);
			}
		});
		importarExcelBt.setToolTip("Importar do Excel");
		
		ToolBar toolBar = new ToolBar();
		toolBar.add(exportarExcelBt);
		toolBar.add(importarExcelBt);

		return toolBar;
	}
	
	private TextField<String> getCursoField() {
		if(cursoField == null) {
			cursoField = new TextField<String>();
		}
		return cursoField;
	}
	
	@Override
	protected void handleEvent(AppEvent event) {
		if (event.getType() == AppEvents.CursoAreaTitulacaoList) {
			initList();
		}
	}

}
