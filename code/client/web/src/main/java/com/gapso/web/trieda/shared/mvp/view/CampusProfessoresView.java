package com.gapso.web.trieda.shared.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.gapso.web.trieda.shared.dtos.ProfessorCampusDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.mvp.presenter.CampusProfessoresPresenter;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.ProfessorComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleFilter;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;

public class CampusProfessoresView extends MyComposite implements CampusProfessoresPresenter.Display {

	private SimpleToolBar toolBar;
	private SimpleGrid<ProfessorCampusDTO> gridPanel;
	private SimpleFilter filter;
	private CampusComboBox campusBuscaComboBox;
	private ProfessorComboBox professorBuscaComboBox;
	private ContentPanel panel;
	private GTabItem tabItem;
	
	private UsuarioDTO usuario;
	
	public CampusProfessoresView(UsuarioDTO usuario) {
		this.usuario = usuario;
		initUI();
	}
	
	private void initUI() {
		panel = new ContentPanel(new BorderLayout());
		String title = (usuario.isAdministrador())? "Professor » Campi de Trabalho" : "Master Data » Campi de Trabalho";
		panel.setHeading(title);
		createGrid();
		if(usuario.isAdministrador()) {
			createToolBar();
			createFilter();
		}
		createTabItem();
		initComponent(tabItem);
	}

	private void createTabItem() {
		tabItem = new GTabItem("Campi de Trabalho", Resources.DEFAULTS.campiTrabalho16());
		tabItem.setContent(panel);
	}
	
	private void createToolBar() {
		toolBar = new SimpleToolBar(this);
		panel.setTopComponent(toolBar);
	}
	
	private void createGrid() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
	    bld.setMargins(new Margins(5, 5, 5, 5));
	    
	    gridPanel = new SimpleGrid<ProfessorCampusDTO>(getColumnList(), this);
	    panel.add(gridPanel, bld);
	}

	private List<ColumnConfig> getColumnList() {
		List<ColumnConfig> list = new ArrayList<ColumnConfig>();
		list.add(new ColumnConfig(ProfessorCampusDTO.PROPERTY_CAMPUS_STRING, "Campus", 150));
		list.add(new ColumnConfig(ProfessorCampusDTO.PROPERTY_PROFESSOR_CPF, "CPF Professor", 110));
		list.add(new ColumnConfig(ProfessorCampusDTO.PROPERTY_PROFESSOR_STRING, "Professor", 250));
		return list;
	}

	private void createFilter() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.EAST);
		bld.setMargins(new Margins(5, 5, 5, 0));
		bld.setCollapsible(true);
		
		filter = new SimpleFilter();
		campusBuscaComboBox = new CampusComboBox();
		professorBuscaComboBox = new ProfessorComboBox();
		filter.addField(campusBuscaComboBox);
		filter.addField(professorBuscaComboBox);
		
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
	public Button getImportExcelButton() {
		return toolBar.getImportExcelButton();
	}

	@Override
	public Button getExportExcelButton() {
		return toolBar.getExportExcelButton();
	}
	
	@Override
	public SimpleGrid<ProfessorCampusDTO> getGrid() {
		return gridPanel;
	}

	@Override
	public CampusComboBox getCampusBuscaComboBox() {
		return campusBuscaComboBox;
	}

	@Override
	public ProfessorComboBox getProfessorBuscaComboBox() {
		return professorBuscaComboBox;
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