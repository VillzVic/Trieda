package com.gapso.web.trieda.main.client.mvp.view;

import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.gapso.web.trieda.main.client.mvp.presenter.CampiDeslocamentoPresenter;
import com.gapso.web.trieda.shared.dtos.DeslocamentoCampusDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.DeslocamentoGrid;
import com.gapso.web.trieda.shared.util.view.DeslocamentoToolBar;
import com.gapso.web.trieda.shared.util.view.GTabItem;

public class CampiDeslocamentoView extends MyComposite implements CampiDeslocamentoPresenter.Display {

	private DeslocamentoGrid<DeslocamentoCampusDTO> deslocamentoGrid;
	private DeslocamentoToolBar deslocamentoToolBar;
	private List<DeslocamentoCampusDTO> deslocamentoCampusDTOList;
	
	private ContentPanel panel;
	private GTabItem tabItem;

	public CampiDeslocamentoView(List<DeslocamentoCampusDTO> deslocamentoCampusDTOList) {
		this.deslocamentoCampusDTOList = deslocamentoCampusDTOList;
		initUI();
	}
	
	private void initUI() {
		panel = new ContentPanel(new BorderLayout());
		panel.setHeadingHtml("Master Data » Deslocamentos entre Campi");
		createToolBar();
		createGrid();
		createTabItem();
		initComponent(tabItem);
	}
	
	private void createTabItem() {
		tabItem = new GTabItem("Deslocamentos entre Campi", Resources.DEFAULTS.deslocamentoCampi16());
		tabItem.setContent(panel);
	}
	
	private void createToolBar() {
		deslocamentoToolBar = new DeslocamentoToolBar();
		panel.setTopComponent(deslocamentoToolBar);
	}
	
	private void createGrid() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
	    bld.setMargins(new Margins(5, 5, 5, 5));
	    
	    deslocamentoGrid = new DeslocamentoGrid<DeslocamentoCampusDTO>(deslocamentoCampusDTOList);
	    deslocamentoGrid.setContainsCusto(true);
	    panel.add(deslocamentoGrid, bld);
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
	public DeslocamentoGrid<DeslocamentoCampusDTO> getGrid() {
		return deslocamentoGrid;
	}

}
