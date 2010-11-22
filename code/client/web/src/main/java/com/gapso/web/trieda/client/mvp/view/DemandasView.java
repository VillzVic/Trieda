package com.gapso.web.trieda.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.gapso.web.trieda.client.mvp.model.DemandaDTO;
import com.gapso.web.trieda.client.mvp.presenter.DemandasPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.CampusComboBox;
import com.gapso.web.trieda.client.util.view.CurriculoComboBox;
import com.gapso.web.trieda.client.util.view.CursoComboBox;
import com.gapso.web.trieda.client.util.view.DisciplinaComboBox;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.SimpleFilter;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.client.util.view.SimpleToolBar;
import com.gapso.web.trieda.client.util.view.TurnoComboBox;

public class DemandasView extends MyComposite implements DemandasPresenter.Display {

	private SimpleToolBar toolBar;
	private SimpleGrid<DemandaDTO> gridPanel;
	private SimpleFilter filter;
	private CampusComboBox campusBuscaCB;
	private CursoComboBox cursoBuscaCB;
	private CurriculoComboBox curriculoBuscaCB;
	private TurnoComboBox turnoBuscaCB;
	private DisciplinaComboBox disciplinaBuscaCB;
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
		toolBar = new SimpleToolBar();
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
		list.add(new ColumnConfig("campusString", "Campus", 100));
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
		
		campusBuscaCB = new CampusComboBox();
		cursoBuscaCB = new CursoComboBox();
		curriculoBuscaCB = new CurriculoComboBox();
		turnoBuscaCB = new TurnoComboBox();
		disciplinaBuscaCB = new DisciplinaComboBox();
		
		filter.addField(campusBuscaCB);
		filter.addField(cursoBuscaCB);
		filter.addField(curriculoBuscaCB);
		filter.addField(turnoBuscaCB);
		filter.addField(disciplinaBuscaCB);
		
		panel.add(filter, bld);
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

	@Override
	public Button getNewButton() {
		return toolBar.getNewButton();
	}

	@Override
	public Button getEditButton() {
		return toolBar.getEditButton();
	}

	@Override
	public Button getRemoveButton() {
		return toolBar.getRemoveButton();
	}

	@Override
	public CampusComboBox getCampusBuscaComboBox() {
		return campusBuscaCB;
	}

	@Override
	public CursoComboBox getCursoBuscaComboBox() {
		return cursoBuscaCB;
	}

	@Override
	public CurriculoComboBox getCurriculoBuscaComboBox() {
		return curriculoBuscaCB;
	}

	@Override
	public TurnoComboBox getTurnoBuscaComboBox() {
		return turnoBuscaCB;
	}

	@Override
	public DisciplinaComboBox getDisciplinaBuscaComboBox() {
		return disciplinaBuscaCB;
	}

	@Override
	public Button getSubmitBuscaButton() {
		return filter.getSubmitButton();
	}

	@Override
	public Button getResetBuscaButton() {
		return filter.getResetButton();
	}

}
