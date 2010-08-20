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
import com.extjs.gxt.ui.client.event.EventType;
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
import com.gapso.web.trieda.client.crudmodel.CampusProfessorCrudModel;
import com.gapso.web.trieda.client.mvc.model.CampusModel;
import com.gapso.web.trieda.client.mvc.model.CampusProfessorModel;
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

public class CampusProfessorView extends View {

	private GTabItem tabItem;
	
	public CampusProfessorView(Controller controller) {
		super(controller);
	}

	@Override
	public void initialize() {
	}
	
	private void onCampusProfessoresList() {
		tabItem = new GTabItem("Campi/Professores", Resources.DEFAULTS.professores16());
		tabItem.setContent(new SimpleCrud<CampusProfessorModel>(new CampusProfessorCrudModel()));
		GTab tab = (GTab)Registry.get(AppView.TAB);
		tab.add(tabItem);
	}
	
	private void onCampusProfessoresView() {
		SimpleModal modal = new SimpleModal("Definir Campi/Professores", Resources.DEFAULTS.professores16());
		modal.setContent(getPanel());
		modal.show();
	}
	
	private ContentPanel getPanel() {
		ContentPanel panelLists = new ContentPanel(new RowLayout(Orientation.HORIZONTAL));
		panelLists.setHeaderVisible(false);
				
		ContentPanel campusNaoAssociadasListPanel = new ContentPanel(new FitLayout());
		campusNaoAssociadasListPanel.setHeading("Professor(es) não associados ao Campus");
		campusNaoAssociadasListPanel.add(getCampusNaoAssociadasList());
		
		ContentPanel campusAssociadasListPanel = new ContentPanel(new FitLayout());
		campusAssociadasListPanel.setHeading("Professor(es) associados ao Campus");
		campusAssociadasListPanel.add(getCampusAssociadasList());
		
		panelLists.add(campusNaoAssociadasListPanel, new RowData(.5, 1, new Margins(0, 0, 0, 0)));
		panelLists.add(getButtonsRightLeftPanel(), new RowData(42, 1, new Margins(0, 5, 0, 5)));
		panelLists.add(campusAssociadasListPanel, new RowData(.5, 1, new Margins(0, 0, 0, 0)));
		
		FormPanel panelField = new FormPanel();
		panelField.setHeaderVisible(false);
		panelField.setPadding(2);
		FormData formDataFilter = new FormData("-5");
		
		TextField<String> campusField = getCampusField();
		campusField.setFieldLabel("Campus");
		campusField.setLabelStyle("text-align: right;");
		panelField.add(campusField, formDataFilter);
		
		panelField.add(new Button());		
		panelField.setCollapsible(true);
		
		ContentPanel panel = new ContentPanel(new BorderLayout());
		panel.setHeaderVisible(false);
		
		panel.add(panelField, new BorderLayoutData(LayoutRegion.NORTH, 30));
		panel.add(panelLists, new BorderLayoutData(LayoutRegion.CENTER));
		
		return panel;
	}
	
	private ListView<CampusModel> getCampusNaoAssociadasList() {
		final ICrudService service = (ICrudService)Services.get(Services.CAMPI);
		RpcProxy<PagingLoadResult<ModelData>> proxy = new RpcProxy<PagingLoadResult<ModelData>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<ModelData>> callback) {
				service.getList(callback);
			}
		};
		ListLoader<ListLoadResult<CampusModel>> loader = new BaseListLoader<ListLoadResult<CampusModel>>(proxy);
		loader.load();
		ListStore<CampusModel> store = new ListStore<CampusModel>(loader);
		
		ListView<CampusModel> campi = new ListView<CampusModel>(store);
		campi.setDisplayProperty("nome");
		return campi;
	}
	
	private ListView<CampusModel> getCampusAssociadasList() {
		final ICrudService service = (ICrudService)Services.get(Services.CAMPI);
		RpcProxy<PagingLoadResult<ModelData>> proxy = new RpcProxy<PagingLoadResult<ModelData>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<ModelData>> callback) {
				service.getList(callback);
			}
		};
		ListLoader<ListLoadResult<CampusModel>> loader = new BaseListLoader<ListLoadResult<CampusModel>>(proxy);
		loader.load();
		ListStore<CampusModel> store = new ListStore<CampusModel>(loader);
		
		ListView<CampusModel> campi = new ListView<CampusModel>(store);
		campi.setDisplayProperty("nome");
		return campi;
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
		
	private TextField<String> getCampusField() {
		TextField<String> campusField = new TextField<String>();
		return campusField;
	}
	
	@Override
	protected void handleEvent(AppEvent event) {
		EventType type = event.getType();
		if (type == AppEvents.CampusProfessorList) onCampusProfessoresList();
		if (type == AppEvents.CampusProfessorView) onCampusProfessoresView();
	}

}
