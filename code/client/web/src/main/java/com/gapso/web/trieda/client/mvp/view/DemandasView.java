package com.gapso.web.trieda.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.gapso.web.trieda.client.mvp.model.DemandaDTO;
import com.gapso.web.trieda.client.mvp.presenter.DemandasPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.SimpleFilter;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.client.util.view.SimpleToolBar;

public class DemandasView extends MyComposite implements DemandasPresenter.Display {

	private SimpleToolBar toolBar;
	private SimpleGrid<DemandaDTO> gridPanel;
	private SimpleFilter filter;
	private TextField<String> nomeBuscaTextField;
	private NumberField tempoBuscaTextField;
	private Button saveBT;
	private Button resetBT;
	private ContentPanel panel;
	private GTabItem tabItem;
	
	public DemandasView() {
		initUI();
	}
	
	private void initUI() {
		panel = new ContentPanel(new BorderLayout());
		panel.setHeading("Master Data » Demandas");
		createToolBar();
		createGrid();
		createFilter();
		createTabItem();
		initComponent(tabItem);
	}

	private void createTabItem() {
		tabItem = new GTabItem("Demandas", Resources.DEFAULTS.demanda16());
		tabItem.setContent(panel);
	}
	
	private void createToolBar() {
		toolBar = new SimpleToolBar(false, false, false, true, true);
		
		saveBT = toolBar.createButton("Salvar", Resources.DEFAULTS.save16());
		resetBT = toolBar.createButton("Cancelar", Resources.DEFAULTS.cancel16());
		
		toolBar.insert(resetBT, 0);
		toolBar.insert(saveBT, 0);
		
		panel.setTopComponent(toolBar);
	}
	
	private void createGrid() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
	    bld.setMargins(new Margins(5, 0, 5, 5));
	    
	    gridPanel = new SimpleGrid<DemandaDTO>(getColumnList());
	    panel.add(gridPanel, bld);
	}

	private List<ColumnConfig> getColumnList() {
		List<ColumnConfig> list = new ArrayList<ColumnConfig>();
		list.add(new ColumnConfig("campusSting", "Campus", 100));
		list.add(new ColumnConfig("cursoString", "Curso", 100));
		list.add(new ColumnConfig("curriculoString", "Código de Matriz Curricular", 100));
		list.add(new ColumnConfig("turnoString", "Turno", 100));
		list.add(new ColumnConfig("disciplinaString", "Disciplina", 100));
		list.add(new ColumnConfig("demanda", "Demanda de Alunos", 100));
		return list;
	}

	private void createFilter() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.EAST);
		bld.setMargins(new Margins(5, 5, 5, 5));
		bld.setCollapsible(true);
		
		filter = new SimpleFilter();
		nomeBuscaTextField = new TextField<String>();
		nomeBuscaTextField.setFieldLabel("Nome");
		tempoBuscaTextField = new NumberField();
		tempoBuscaTextField.setFieldLabel("Duração da Aula (min)");
		filter.addField(nomeBuscaTextField);
		filter.addField(tempoBuscaTextField);
		
		panel.add(filter, bld);
	}
	
	@Override
	public Button getSaveButton() {
		return saveBT;
	}

	@Override
	public Button getResetButton() {
		return resetBT;
	}

	@Override
	public Button getImportExcelButton() {
		return toolBar.getImportExcelButton();
	}

	@Override
	public Button getExportExcelButton() {
		return toolBar.getExportExcelButton();
	}
	
	@Override
	public SimpleGrid<DemandaDTO> getGrid() {
		return gridPanel;
	}
	
	@Override
	public void setProxy(RpcProxy<PagingLoadResult<DemandaDTO>> proxy) {
		gridPanel.setProxy(proxy);
	}

}
