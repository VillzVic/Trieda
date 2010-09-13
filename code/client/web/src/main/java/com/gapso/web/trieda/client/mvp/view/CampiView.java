package com.gapso.web.trieda.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.presenter.CampiPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.EstadoComboBox;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.SimpleFilter;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.client.util.view.SimpleToolBar;

public class CampiView extends MyComposite implements CampiPresenter.Display {

	private SimpleToolBar toolBar;
	private SimpleGrid<CampusDTO> gridPanel;
	private SimpleFilter filter;
	private TextField<String> nomeBuscaTextField;
	private TextField<String> codigoBuscaTextField;
	private EstadoComboBox estadoBuscaComboBox;
	private TextField<String> municipioBuscaTextField;
	private TextField<String> bairroBuscaTextField;
	private ContentPanel panel;
	private GTabItem tabItem;
	
	public CampiView() {
		initUI();
	}
	
	private void initUI() {
		panel = new ContentPanel(new BorderLayout());
		panel.setHeading("Master Data » Campi");
		createToolBar();
		createGrid();
		createFilter();
		createTabItem();
		initComponent(tabItem);
	}
	
	private void createTabItem() {
		tabItem = new GTabItem("Campi", Resources.DEFAULTS.campi16());
		tabItem.setContent(panel);
	}
	
	private void createToolBar() {
		toolBar = new SimpleToolBar();
		panel.setTopComponent(toolBar);
	}
	
	private void createGrid() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
	    bld.setMargins(new Margins(5, 5, 5, 5));
	    
	    gridPanel = new SimpleGrid<CampusDTO>(getColumnList());
	    panel.add(gridPanel, bld);
	}

	public List<ColumnConfig> getColumnList() {
		List<ColumnConfig> list = new ArrayList<ColumnConfig>();
		list.add(new ColumnConfig("nome", "Nome", 100));
		list.add(new ColumnConfig("codigo", "Código", 100));
		list.add(new ColumnConfig("estado", "Estado", 100));
		list.add(new ColumnConfig("municipio", "Município", 100));
		list.add(new ColumnConfig("bairro", "Bairro", 100));
		return list;
	}

	private void createFilter() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.EAST);
		bld.setMargins(new Margins(0, 0, 0, 5));
		bld.setCollapsible(true);
		
		filter = new SimpleFilter();
		nomeBuscaTextField = new TextField<String>();
		nomeBuscaTextField.setFieldLabel("Nome");
		codigoBuscaTextField = new TextField<String>();
		codigoBuscaTextField.setFieldLabel("Código");
		estadoBuscaComboBox = new EstadoComboBox();
		estadoBuscaComboBox.setFieldLabel("Estado");
		municipioBuscaTextField = new TextField<String>();
		municipioBuscaTextField.setFieldLabel("Município");
		bairroBuscaTextField = new TextField<String>();
		bairroBuscaTextField.setFieldLabel("Bairro");
		filter.addField(nomeBuscaTextField);
		filter.addField(codigoBuscaTextField); 
//		TODO filter.addField(estadoBuscaComboBox); 
		filter.addField(municipioBuscaTextField); 
		filter.addField(bairroBuscaTextField); 
		
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
	public SimpleGrid<CampusDTO> getGrid() {
		return gridPanel;
	}

	@Override
	public void setProxy(RpcProxy<PagingLoadResult<CampusDTO>> proxy) {
		gridPanel.setProxy(proxy);
	}
	
	@Override
	public TextField<String> getNomeBuscaTextField() {
		return nomeBuscaTextField;
	}

	@Override
	public TextField<String> getCodigoBuscaTextField() {
		return codigoBuscaTextField;
	}

	@Override
	public Button getSubmitBuscaButton() {
		return filter.getSubmitButton();
	}

	@Override
	public Button getResetBuscaButton() {
		return filter.getResetButton();
	}

	@Override
	public EstadoComboBox getEstadoBuscaComboBox() {
		return estadoBuscaComboBox;
	}

	@Override
	public TextField<String> getMunicipioBuscaTextField() {
		return municipioBuscaTextField;
	}

	@Override
	public TextField<String> getBairroBuscaTextField() {
		return bairroBuscaTextField;
	}
	
}
