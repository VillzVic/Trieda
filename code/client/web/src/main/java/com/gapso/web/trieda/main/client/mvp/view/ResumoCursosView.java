package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.store.TreeStore;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnData;
import com.extjs.gxt.ui.client.widget.grid.ColumnModel;
import com.extjs.gxt.ui.client.widget.grid.Grid;
import com.extjs.gxt.ui.client.widget.grid.GridCellRenderer;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.treegrid.TreeGrid;
import com.extjs.gxt.ui.client.widget.treegrid.TreeGridCellRenderer;
import com.gapso.web.trieda.main.client.mvp.presenter.ResumoCursosPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ResumoCursoDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class ResumoCursosView extends MyComposite implements ResumoCursosPresenter.Display {

	private TreeStore<ResumoCursoDTO> store;
	private TreeGrid<ResumoCursoDTO> tree;

	private ContentPanel panel;
	private GTabItem tabItem;
	
	private CenarioDTO cenario;
	
	public ResumoCursosView(CenarioDTO cenario) {
		this.cenario = cenario;
		configureProxy();
		initUI();
		createGrid();
		createTabItem();
		initComponent(tabItem);
	}
	
	private void initUI() {
		panel = new ContentPanel(new BorderLayout());
		panel.setHeading("Master Data » Resultados por Cursos");
	}

	@Override
	protected void beforeRender() {
		super.beforeRender();
	}

	private void createTabItem() {
		tabItem = new GTabItem("Resumo por Cursos", Resources.DEFAULTS.resumoCampi16());
		tabItem.setContent(panel);
	}
	
	private void createGrid() {
		tree = new TreeGrid<ResumoCursoDTO>(getStore(), new ColumnModel(getColumnList()));
	    
	    ContentPanel contentPanel = new ContentPanel(new FitLayout());
	    contentPanel.setHeaderVisible(false);
	    contentPanel.add(tree);
	    
	    BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
	    bld.setMargins(new Margins(5));
	    panel.add(contentPanel, bld);
	}

	public List<ColumnConfig> getColumnList() {
		GridCellRenderer<ResumoCursoDTO> percenteRenderer = new GridCellRenderer<ResumoCursoDTO>() {
			@Override
			public String render(ResumoCursoDTO model, String property,
					ColumnData config, int rowIndex, int colIndex,
					ListStore<ResumoCursoDTO> store, Grid<ResumoCursoDTO> grid) {
				if(model.get(property) == null) return "";
				return (((Double)model.get(property)) * 100) + "%";
			}
		};
		
		List<ColumnConfig> list = new ArrayList<ColumnConfig>();
		ColumnConfig campusColumnConfig = new ColumnConfig(ResumoCursoDTO.PROPERTY_CAMPUS_STRING, getI18nConstants().campus(), 80);
		campusColumnConfig.setRenderer(new TreeGridCellRenderer<ResumoCursoDTO>());
		list.add(campusColumnConfig);
		list.add(new ColumnConfig(ResumoCursoDTO.PROPERTY_TURNO_STRING, getI18nConstants().turno(), 80));
		list.add(new ColumnConfig(ResumoCursoDTO.PROPERTY_CURSO_STRING, getI18nConstants().curso(), 80));
		list.add(new ColumnConfig(ResumoCursoDTO.PROPERTY_MATRIZCURRICULAR_STRING, getI18nConstants().matrizCurricular(), 80));
		list.add(new ColumnConfig(ResumoCursoDTO.PROPERTY_PERIODO_INT, getI18nConstants().periodo(), 55));
		list.add(new ColumnConfig(ResumoCursoDTO.PROPERTY_DISCIPLINA_STRING, getI18nConstants().disciplina(), 80));
		list.add(new ColumnConfig(ResumoCursoDTO.PROPERTY_TURMA_STRING, getI18nConstants().turma(), 80));
		list.add(new ColumnConfig(ResumoCursoDTO.PROPERTY_TIPO_CREDITO_TEORICO_BOOLEAN, getI18nConstants().TipoCredito(), 80));
		list.add(new ColumnConfig(ResumoCursoDTO.PROPERTY_CREDITOS_INT, getI18nConstants().creditos(), 60));
		list.add(new ColumnConfig(ResumoCursoDTO.PROPERTY_QUANTIDADE_ALUNOS_INT, getI18nConstants().quantidadeAlunos(), 70));
		ColumnConfig rateioColumnConfig = new ColumnConfig(ResumoCursoDTO.PROPERTY_RATEIO_DOUBLE, getI18nConstants().rateio(), 80);
		rateioColumnConfig.setRenderer(percenteRenderer);
		list.add(rateioColumnConfig);
		list.add(new ColumnConfig(ResumoCursoDTO.PROPERTY_CUSTO_DOCENTE_DOUBLE, getI18nConstants().custoDocente(), 100));
		list.add(new ColumnConfig(ResumoCursoDTO.PROPERTY_RECEITA_DOUBLE, getI18nConstants().receita(), 100));
		list.add(new ColumnConfig(ResumoCursoDTO.PROPERTY_MARGEM_DOUBLE, getI18nConstants().margem(), 100));
		ColumnConfig margemPercenteColumnConfig = new ColumnConfig(ResumoCursoDTO.PROPERTY_MARGEM_PERCENTE_DOUBLE, getI18nConstants().margemPercente(), 100);
		margemPercenteColumnConfig.setRenderer(percenteRenderer);
		list.add(margemPercenteColumnConfig);
		return list;
	}
	
	@Override
	public void setStore(TreeStore<ResumoCursoDTO> store) {
		this.store = store;
	}

	public TreeStore<ResumoCursoDTO> getStore() {
		return store;
	}
	
	@Override
	public TreeGrid<ResumoCursoDTO> getTree() {
		return tree;
	}
	
	private void configureProxy() {
		store = new TreeStore<ResumoCursoDTO>();
		
		Services.cursos().getResumos(cenario, null, new AsyncCallback<List<ResumoCursoDTO>>() {
			@Override
			public void onFailure(Throwable caught) {
				caught.printStackTrace();
			}
			@Override
			public void onSuccess(List<ResumoCursoDTO> list) {
				store.add(list, true);
			}
		});
	}
	
}
