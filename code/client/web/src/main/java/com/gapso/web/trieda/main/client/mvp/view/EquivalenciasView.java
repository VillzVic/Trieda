package com.gapso.web.trieda.main.client.mvp.view;

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
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.gapso.web.trieda.main.client.mvp.presenter.EquivalenciasPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.EquivalenciaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.DisciplinaAutoCompleteBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleFilter;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;

public class EquivalenciasView extends MyComposite implements EquivalenciasPresenter.Display {

	private SimpleToolBar toolBar;
	private SimpleGrid<EquivalenciaDTO> gridPanel;
	private SimpleFilter filter;
	private CampusComboBox campusCB;
	private DisciplinaAutoCompleteBox disciplinaCB;
	private CursoComboBox cursoCB;
	private ContentPanel panel;
	private GTabItem tabItem;
	private CenarioDTO cenarioDTO;
	
	public EquivalenciasView(CenarioDTO cenarioDTO) {
		this.cenarioDTO = cenarioDTO;
		initUI();
	}
	
	private void initUI() {
		panel = new ContentPanel(new BorderLayout());
		panel.setHeadingHtml(cenarioDTO.getNome() + " » Equivalências");
		createToolBar();
		createGrid();
		createFilter();
		createTabItem();
		initComponent(tabItem);
	}

	private void createTabItem() {
		tabItem = new GTabItem("Equivalências", Resources.DEFAULTS.equivalencia16());
		tabItem.setContent(panel);
	}
	
	private void createToolBar() {
		this.toolBar = new SimpleToolBar(true, true, true, true, true, true, this, "Equivalências" );
		panel.setTopComponent(toolBar);
	}
	
	private void createGrid() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
	    bld.setMargins(new Margins(5, 5, 5, 5));
	    
	    gridPanel = new SimpleGrid<EquivalenciaDTO>(getColumnList(), this, this.toolBar);
	    panel.add(gridPanel, bld);
	}

	private List<ColumnConfig> getColumnList() {
		List<ColumnConfig> list = new ArrayList<ColumnConfig>();
		list.add(new ColumnConfig(EquivalenciaDTO.PROPERTY_CURSOU_CODIGO, "Cód. Disciplina (Cursou)", 250));
		list.add(new ColumnConfig(EquivalenciaDTO.PROPERTY_CURSOU_STRING, "Cursou", 250));
		list.add(new ColumnConfig(EquivalenciaDTO.PROPERTY_ELIMINA_CODIGO, "Cód. Disciplina (Elimina)", 250));
		list.add(new ColumnConfig(EquivalenciaDTO.PROPERTY_ELIMINA_STRING, "Elimina", 250));
		list.add(new ColumnConfig(EquivalenciaDTO.PROPERTY_CURSO_STRING, "Curso", 250) );
		return list;
	}

	private void createFilter() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.EAST);
		bld.setMargins(new Margins(5, 5, 5, 0));
		bld.setCollapsible(true);
		
		filter = new SimpleFilter();
		campusCB = new CampusComboBox(cenarioDTO);
		disciplinaCB = new DisciplinaAutoCompleteBox(cenarioDTO);
		cursoCB = new CursoComboBox(cenarioDTO);
		filter.addField(campusCB);
		filter.addField(disciplinaCB);
		filter.addField(cursoCB);
		
		panel.add(filter, bld);
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
	public Button getRemoveAllButton()
	{
		return this.toolBar.getRemoveAllButton();
	}

	@Override
	public Button getImportExcelButton() {
		return toolBar.getImportExcelButton();
	}

	@Override
	public MenuItem getExportXlsExcelButton() {
		return (MenuItem) toolBar.getExportExcelButton().getMenu().getItem(0);
	}
	
	@Override
	public MenuItem getExportXlsxExcelButton() {
		return (MenuItem) toolBar.getExportExcelButton().getMenu().getItem(1);
	}
	
	@Override
	public SimpleGrid<EquivalenciaDTO> getGrid() {
		return gridPanel;
	}
	
	@Override
	public void setProxy(RpcProxy<PagingLoadResult<EquivalenciaDTO>> proxy) {
		gridPanel.setProxy(proxy);
	}

	@Override
	public CampusComboBox getCampusComboBox() {
		return campusCB;
	}

	@Override
	public DisciplinaAutoCompleteBox getDisciplinaComboBox() {
		return disciplinaCB;
	}
	
	@Override
	public CursoComboBox getCursoComboBox() {
		return cursoCB;
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
