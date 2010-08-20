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
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.extjs.gxt.ui.client.mvc.View;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.ListView;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.RowData;
import com.extjs.gxt.ui.client.widget.layout.RowLayout;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.crudmodel.GrupoSalaCrudModel;
import com.gapso.web.trieda.client.mvc.model.GrupoSalaModel;
import com.gapso.web.trieda.client.mvc.model.SalaModel;
import com.gapso.web.trieda.client.mvp.view.AppView;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.gapso.web.trieda.client.util.view.simplecrud.ICrudService;
import com.gapso.web.trieda.client.util.view.simplecrud.SimpleCrud;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class GrupoSalaView extends View {

	private GTabItem tabItem;
	
	public GrupoSalaView(Controller controller) {
		super(controller);
	}

	@Override
	public void initialize() {
	}
	
	private void initList() {
		tabItem = new GTabItem("Grupos de Salas", Resources.DEFAULTS.sala16());
		tabItem.setContent(new SimpleCrud<GrupoSalaModel>(new GrupoSalaCrudModel()));
		GTab tab = (GTab)Registry.get(AppView.TAB);
		tab.add(tabItem);
	}
	
	private void initSalasAssociadas() {
		SimpleModal modal = new SimpleModal("Salas Associadas", Resources.DEFAULTS.sala16());
		modal.setContent(getPanel());
		modal.show();
	}
	
	private ContentPanel getPanel() {
		ContentPanel panelLists = new ContentPanel(new RowLayout(Orientation.HORIZONTAL));
		panelLists.setHeaderVisible(false);
		
		ContentPanel andaresListPanel = new ContentPanel(new FitLayout());
		andaresListPanel.setHeading("Andares");
		andaresListPanel.add(getAndaresList());
		
		ContentPanel salasNaoPertencesListPanel = new ContentPanel(new FitLayout());
		salasNaoPertencesListPanel.setHeading("Sala(s) não pertencentes ao Grupo");
		salasNaoPertencesListPanel.add(getSalasNaoPertencesList());
		
		ContentPanel salasPertencesListPanel = new ContentPanel(new FitLayout());
		salasPertencesListPanel.setHeading("Sala(s) pertencentes ao Grupo");
		salasPertencesListPanel.add(getSalasPertencesList());
		
		panelLists.add(andaresListPanel, new RowData(.2, 1, new Margins(0, 0, 0, 0)));
		panelLists.add(getButtonUpdatePanel(), new RowData(42, 1, new Margins(0, 5, 0, 5)));
		panelLists.add(salasNaoPertencesListPanel, new RowData(.4, 1, new Margins(0, 0, 0, 0)));
		panelLists.add(getButtonsRightLeftPanel(), new RowData(42, 1, new Margins(0, 5, 0, 5)));
		panelLists.add(salasPertencesListPanel, new RowData(.4, 1, new Margins(0, 0, 0, 0)));
		
		FormPanel panelField = new FormPanel();
		panelField.setHeaderVisible(false);
		panelField.setPadding(2);
		FormData formDataFilter = new FormData("-5");
		
		TextField<String> campusField = getCampusField();
		campusField.setFieldLabel("Curso");
		campusField.setLabelStyle("text-align: right;");
		panelField.add(campusField, formDataFilter);
		
		TextField<String> unidadeField = getUnidadeField();
		unidadeField.setFieldLabel("Unidade");
		unidadeField.setLabelStyle("text-align: right;");
		panelField.add(unidadeField, formDataFilter);
		
		panelField.add(new Button());		
		panelField.setCollapsible(true);
		
		ContentPanel panel = new ContentPanel(new BorderLayout());
		panel.setHeaderVisible(false);
		
		panel.add(panelField, new BorderLayoutData(LayoutRegion.NORTH, 55));
		panel.add(panelLists, new BorderLayoutData(LayoutRegion.CENTER));
		
		return panel;
	}
	
	private ListView<SalaModel> getAndaresList() {
		final ICrudService service = (ICrudService)Services.get(Services.SALAS);
		RpcProxy<PagingLoadResult<ModelData>> proxy = new RpcProxy<PagingLoadResult<ModelData>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<ModelData>> callback) {
				service.getList(callback);
			}
		};
		ListLoader<ListLoadResult<SalaModel>> loader = new BaseListLoader<ListLoadResult<SalaModel>>(proxy);
		loader.load();
		ListStore<SalaModel> store = new ListStore<SalaModel>(loader);
		
		ListView<SalaModel> salas = new ListView<SalaModel>(store);
		salas.setDisplayProperty("andar");
		return salas;
	}
	
	private ListView<SalaModel> getSalasNaoPertencesList() {
		final ICrudService service = (ICrudService)Services.get(Services.SALAS);
		RpcProxy<PagingLoadResult<ModelData>> proxy = new RpcProxy<PagingLoadResult<ModelData>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<ModelData>> callback) {
				service.getList(callback);
			}
		};
		ListLoader<ListLoadResult<SalaModel>> loader = new BaseListLoader<ListLoadResult<SalaModel>>(proxy);
		loader.load();
		ListStore<SalaModel> store = new ListStore<SalaModel>(loader);
		
		ListView<SalaModel> salas = new ListView<SalaModel>(store);
		salas.setDisplayProperty("codigo");
		return salas;
	}
	
	private ListView<SalaModel> getSalasPertencesList() {
		final ICrudService service = (ICrudService)Services.get(Services.SALAS);
		RpcProxy<PagingLoadResult<ModelData>> proxy = new RpcProxy<PagingLoadResult<ModelData>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<ModelData>> callback) {
				service.getList(callback);
			}
		};
		ListLoader<ListLoadResult<SalaModel>> loader = new BaseListLoader<ListLoadResult<SalaModel>>(proxy);
		loader.load();
		ListStore<SalaModel> store = new ListStore<SalaModel>(loader);
		
		ListView<SalaModel> salas = new ListView<SalaModel>(store);
		salas.setDisplayProperty("codigo");
		return salas;
	}
	
	private LayoutContainer getButtonsRightLeftPanel() {
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
	
	private LayoutContainer getButtonUpdatePanel() {
		ContentPanel panel = new ContentPanel();
		panel.setHeaderVisible(false);
		panel.setBodyBorder(false);
		panel.setBodyStyle("display: table-cell; vertical-align: middle");
		panel.setLayout(new RowLayout(Orientation.VERTICAL));
		
		Button toRightBt = new Button();
		toRightBt.setSize(30, 50);
		toRightBt.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.toRight24()));
				
		RowData rowData = new RowData(-1, -1, new Margins(4, 0, 4, 0));
		
		panel.add(toRightBt, rowData);
		
		return panel;
	}
	
	private TextField<String> getCampusField() {
		TextField<String> cursoField = new TextField<String>();
		return cursoField;
	}
	private TextField<String> getUnidadeField() {
		TextField<String> unidadeField = new TextField<String>();
		return unidadeField;
	}
	
	@Override
	protected void handleEvent(AppEvent event) {
		if (event.getType() == AppEvents.GrupoSalaList) initList();
		if (event.getType() == AppEvents.GrupoSalaAssociadasView) initSalasAssociadas();
	}

}
