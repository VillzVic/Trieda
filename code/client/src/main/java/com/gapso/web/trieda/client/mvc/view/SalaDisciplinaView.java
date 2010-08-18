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
import com.gapso.web.trieda.client.mvc.model.CursoModel;
import com.gapso.web.trieda.client.mvc.model.DisciplinaModel;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.simplecrud.ICrudService;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class SalaDisciplinaView extends View {

	private GTabItem tabItem;

	public SalaDisciplinaView(Controller controller) {
		super(controller);
	}

	@Override
	public void initialize() {
	}
	
	private void initList() {
		tabItem = new GTabItem("Associação de Disciplinas à Salas", Resources.DEFAULTS.disciplina16());
		tabItem.setContent(getPanel());
		GTab tab = (GTab)Registry.get(AppView.TAB);
		tab.add(tabItem);
	}
	

	private ContentPanel getPanel() {
		ContentPanel panelLists = new ContentPanel(new RowLayout(Orientation.HORIZONTAL));
		panelLists.setHeaderVisible(false);
		
		ContentPanel cursosListPanel = new ContentPanel(new FitLayout());
		cursosListPanel.setHeading("Cursos");
		cursosListPanel.add(getCursosList());
		
		ContentPanel disciplinasNaoPertencesListPanel = new ContentPanel(new FitLayout());
		disciplinasNaoPertencesListPanel.setHeading("Disciplina(s) não associadas à Sala");
		disciplinasNaoPertencesListPanel.add(getDisciplinasNaoPertencesList());
		
		ContentPanel disciplinasPertencesListPanel = new ContentPanel(new FitLayout());
		disciplinasPertencesListPanel.setHeading("Disciplina(s) associadas à Sala");
		disciplinasPertencesListPanel.add(getDisciplinasPertencesList());
		
		panelLists.add(cursosListPanel, new RowData(.2, 1, new Margins(0, 0, 0, 0)));
		panelLists.add(getButtonUpdatePanel(), new RowData(42, 1, new Margins(0, 5, 0, 5)));
		panelLists.add(disciplinasNaoPertencesListPanel, new RowData(.4, 1, new Margins(0, 0, 0, 0)));
		panelLists.add(getButtonsRightLeftPanel(), new RowData(42, 1, new Margins(0, 5, 0, 5)));
		panelLists.add(disciplinasPertencesListPanel, new RowData(.4, 1, new Margins(0, 0, 0, 0)));
		
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
		
		TextField<String> andarField = getUnidadeField();
		andarField.setFieldLabel("Andar");
		andarField.setLabelStyle("text-align: right;");
		panelField.add(andarField, formDataFilter);
		
		TextField<String> salaField = getUnidadeField();
		salaField.setFieldLabel("Sala");
		salaField.setLabelStyle("text-align: right;");
		panelField.add(salaField, formDataFilter);
		
		panelField.add(new Button());		
		panelField.setCollapsible(true);
		
		ContentPanel panel = new ContentPanel(new BorderLayout());
		panel.setHeaderVisible(false);
		
		panel.add(panelField, new BorderLayoutData(LayoutRegion.NORTH, 110));
		panel.add(panelLists, new BorderLayoutData(LayoutRegion.CENTER));
		
		return panel;
	}
	
	private ListView<CursoModel> getCursosList() {
		final ICrudService service = (ICrudService)Services.get(Services.CURSOS);
		RpcProxy<PagingLoadResult<ModelData>> proxy = new RpcProxy<PagingLoadResult<ModelData>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<ModelData>> callback) {
				service.getList(callback);
			}
		};
		ListLoader<ListLoadResult<CursoModel>> loader = new BaseListLoader<ListLoadResult<CursoModel>>(proxy);
		loader.load();
		ListStore<CursoModel> store = new ListStore<CursoModel>(loader);
		
		ListView<CursoModel> cursos = new ListView<CursoModel>(store);
		cursos.setDisplayProperty("nome");
		return cursos;
	}
	
	private ListView<DisciplinaModel> getDisciplinasNaoPertencesList() {
		final ICrudService service = (ICrudService)Services.get(Services.DISCIPLINAS);
		RpcProxy<PagingLoadResult<ModelData>> proxy = new RpcProxy<PagingLoadResult<ModelData>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<ModelData>> callback) {
				service.getList(callback);
			}
		};
		ListLoader<ListLoadResult<DisciplinaModel>> loader = new BaseListLoader<ListLoadResult<DisciplinaModel>>(proxy);
		loader.load();
		ListStore<DisciplinaModel> store = new ListStore<DisciplinaModel>(loader);
		
		ListView<DisciplinaModel> disciplinas = new ListView<DisciplinaModel>(store);
		disciplinas.setDisplayProperty("nome");
		return disciplinas;
	}
	
	private ListView<DisciplinaModel> getDisciplinasPertencesList() {
		final ICrudService service = (ICrudService)Services.get(Services.DISCIPLINAS);
		RpcProxy<PagingLoadResult<ModelData>> proxy = new RpcProxy<PagingLoadResult<ModelData>>() {
			@Override
			public void load(Object loadConfig, AsyncCallback<PagingLoadResult<ModelData>> callback) {
				service.getList(callback);
			}
		};
		ListLoader<ListLoadResult<DisciplinaModel>> loader = new BaseListLoader<ListLoadResult<DisciplinaModel>>(proxy);
		loader.load();
		ListStore<DisciplinaModel> store = new ListStore<DisciplinaModel>(loader);
		
		ListView<DisciplinaModel> disciplinas = new ListView<DisciplinaModel>(store);
		disciplinas.setDisplayProperty("nome");
		return disciplinas;
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
		if (event.getType() == AppEvents.SalaDisciplinaList) {
			initList();
		}
	}

}
