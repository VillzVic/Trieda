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
import com.gapso.web.trieda.main.client.mvp.presenter.ResumoDisciplinaPresenter;
import com.gapso.web.trieda.shared.dtos.ResumoDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.services.Services;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.google.gwt.user.client.rpc.AsyncCallback;

public class ResumoDisciplinaView extends MyComposite implements ResumoDisciplinaPresenter.Display {

	private TreeStore<ResumoDisciplinaDTO> store;
	private TreeGrid<ResumoDisciplinaDTO> tree;

	private ContentPanel panel;
	private GTabItem tabItem;
	
	private CenarioDTO cenario;
	
	public ResumoDisciplinaView(CenarioDTO cenario) {
		this.cenario = cenario;
		configureProxy();
		initUI();
		createGrid();
		createTabItem();
		initComponent(tabItem);
	}
	
	private void initUI() {
		panel = new ContentPanel(new BorderLayout());
		panel.setHeading("Master Data » Resultados por Disciplina");
	}

	@Override
	protected void beforeRender() {
		super.beforeRender();
	}

	private void createTabItem() {
		tabItem = new GTabItem("Resumo por Disciplina", Resources.DEFAULTS.resumoCampi16());
		tabItem.setContent(panel);
	}
	
	private void createGrid() {
		tree = new TreeGrid<ResumoDisciplinaDTO>(getStore(), new ColumnModel(getColumnList()));
	    
	    ContentPanel contentPanel = new ContentPanel(new FitLayout());
	    contentPanel.setHeaderVisible(false);
	    contentPanel.add(tree);
	    
	    BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
	    bld.setMargins(new Margins(5));
	    panel.add(contentPanel, bld);
	}

	public List<ColumnConfig> getColumnList() {
		GridCellRenderer<ResumoDisciplinaDTO> percenteRenderer = new GridCellRenderer<ResumoDisciplinaDTO>() {
			@Override
			public String render(ResumoDisciplinaDTO model, String property,
					ColumnData config, int rowIndex, int colIndex,
					ListStore<ResumoDisciplinaDTO> store, Grid<ResumoDisciplinaDTO> grid) {
				if(model.get(property) == null) return "";
				return (((Double)model.get(property)) * 100) + "%";
			}
		};
		GridCellRenderer<ResumoDisciplinaDTO> tipoDeCreditoRenderer = new GridCellRenderer<ResumoDisciplinaDTO>() {
			@Override
			public String render(ResumoDisciplinaDTO model, String property,
					ColumnData config, int rowIndex, int colIndex,
					ListStore<ResumoDisciplinaDTO> store, Grid<ResumoDisciplinaDTO> grid) {
				if(model.get(property) == null) return "";
				return ((Boolean)model.get(property)) ? getI18nConstants().teorico() : getI18nConstants().pratico();
			}
		};
		
		List<ColumnConfig> list = new ArrayList<ColumnConfig>();
		ColumnConfig campusColumnConfig = new ColumnConfig(ResumoDisciplinaDTO.PROPERTY_DISCIPLINA_STRING, getI18nConstants().disciplina(), 80);
		campusColumnConfig.setRenderer(new TreeGridCellRenderer<ResumoDisciplinaDTO>());
		list.add(campusColumnConfig);
		
		list.add(new ColumnConfig(ResumoDisciplinaDTO.PROPERTY_TURMA_STRING, getI18nConstants().turma(), 80));
		
		ColumnConfig tipoDeCreditoColumnConfig = new ColumnConfig(ResumoDisciplinaDTO.PROPERTY_TIPO_CREDITO_TEORICO_BOOLEAN, getI18nConstants().TipoCredito(), 80);
		tipoDeCreditoColumnConfig.setRenderer(tipoDeCreditoRenderer);
		list.add(tipoDeCreditoColumnConfig);
		
		list.add(new ColumnConfig(ResumoDisciplinaDTO.PROPERTY_CREDITOS_INT, getI18nConstants().creditos(), 60));
		list.add(new ColumnConfig(ResumoDisciplinaDTO.PROPERTY_QUANTIDADE_ALUNOS_INT, getI18nConstants().quantidadeAlunos(), 70));
		list.add(new ColumnConfig(ResumoDisciplinaDTO.PROPERTY_CUSTO_DOCENTE_DOUBLE, getI18nConstants().custoDocente(), 100));
		list.add(new ColumnConfig(ResumoDisciplinaDTO.PROPERTY_RECEITA_DOUBLE, getI18nConstants().receita(), 100));
		list.add(new ColumnConfig(ResumoDisciplinaDTO.PROPERTY_MARGEM_DOUBLE, getI18nConstants().margem(), 100));
		ColumnConfig margemPercenteColumnConfig = new ColumnConfig(ResumoDisciplinaDTO.PROPERTY_MARGEM_PERCENTE_DOUBLE, getI18nConstants().margemPercente(), 100);
		margemPercenteColumnConfig.setRenderer(percenteRenderer);
		list.add(margemPercenteColumnConfig);
		return list;
	}
	
	@Override
	public void setStore(TreeStore<ResumoDisciplinaDTO> store) {
		this.store = store;
	}

	public TreeStore<ResumoDisciplinaDTO> getStore() {
		return store;
	}
	
	@Override
	public TreeGrid<ResumoDisciplinaDTO> getTree() {
		return tree;
	}
	
	private void configureProxy() {
		store = new TreeStore<ResumoDisciplinaDTO>();
		
		Services.disciplinas().getResumos(cenario, null, new AsyncCallback<List<ResumoDisciplinaDTO>>() {
			@Override
			public void onFailure(Throwable caught) {
				caught.printStackTrace();
			}
			@Override
			public void onSuccess(List<ResumoDisciplinaDTO> list) {
				store.add(list, true);
			}
		});
	}
	
}
