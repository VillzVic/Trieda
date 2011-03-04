package com.gapso.web.trieda.client.mvp.view;

import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.presenter.UnidadesDeslocamentoPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.CampusComboBox;
import com.gapso.web.trieda.client.util.view.DeslocamentoGrid;
import com.gapso.web.trieda.client.util.view.DeslocamentoToolBar;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.shared.dtos.DeslocamentoUnidadeDTO;

public class UnidadesDeslocamentoView extends MyComposite implements UnidadesDeslocamentoPresenter.Display {

	private CampusComboBox campusBuscaComboBox;
	private DeslocamentoGrid<DeslocamentoUnidadeDTO> deslocamentoGrid;
	private DeslocamentoToolBar deslocamentoToolBar;
	private CampusDTO campusDTO;
	private List<DeslocamentoUnidadeDTO> deslocamentoUnidadeDTOList;
	
	private ContentPanel panel;
	private GTabItem tabItem;

	public UnidadesDeslocamentoView(CampusDTO campusDTO, List<DeslocamentoUnidadeDTO> deslocamentoUnidadeDTOList) {
		this.campusDTO = campusDTO;
		this.deslocamentoUnidadeDTOList = deslocamentoUnidadeDTOList;
		initUI();
	}
	
	private void initUI() {
		panel = new ContentPanel(new BorderLayout());
		panel.setHeading("Master Data » Deslocamentos entre Unidades");
		createFilter();
		createToolBar();
		createGrid();
		createTabItem();
		initComponent(tabItem);
	}
	
	private void createTabItem() {
		tabItem = new GTabItem("Deslocamentos entre Unidades", Resources.DEFAULTS.deslocamentoUnidade16());
		tabItem.setContent(panel);
	}
	
	private void createToolBar() {
		deslocamentoToolBar = new DeslocamentoToolBar();
		panel.setTopComponent(deslocamentoToolBar);
	}
	
	private void createGrid() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
	    bld.setMargins(new Margins(5, 5, 5, 5));
	    
	    deslocamentoGrid = new DeslocamentoGrid<DeslocamentoUnidadeDTO>(deslocamentoUnidadeDTOList);
	    panel.add(deslocamentoGrid, bld);
	}

	private void createFilter() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.NORTH);
		bld.setMargins(new Margins(5, 5, 0, 5));
		bld.setCollapsible(true);
		bld.setSize(65);
		
		FormPanel filter = new FormPanel();
		filter.setHeading("Campus");
		FormData formData = new FormData("-5");
		
		campusBuscaComboBox = new CampusComboBox();
		campusBuscaComboBox.setFieldLabel("Campus");
		if(campusDTO != null) {
			campusBuscaComboBox.setValue(campusDTO);
		}
		
		filter.add(campusBuscaComboBox, formData);
		
		panel.add(filter, bld);
	}

	@Override
	public Button getSaveButton() {
		return deslocamentoToolBar.getSaveButton();
	}

	@Override
	public Button getCancelButton() {
		return deslocamentoToolBar.getCancelButton();
	}

	@Override
	public Button getSimetricaButton() {
		return deslocamentoToolBar.getSimetriaButton();
	}
	
	@Override
	public Button getImportExcelButton() {
		return deslocamentoToolBar.getImportExcelButton();
	}
	
	@Override
	public Button getExportExcelButton() {
		return deslocamentoToolBar.getExportExcelButton();
	}

	@Override
	public CampusComboBox getCampusComboBox() {
		return campusBuscaComboBox;
	}

	@Override
	public DeslocamentoGrid<DeslocamentoUnidadeDTO> getGrid() {
		return deslocamentoGrid;
	}

	@Override
	public CampusDTO getCampusDTO() {
		return campusBuscaComboBox.getValue();
	}

}
