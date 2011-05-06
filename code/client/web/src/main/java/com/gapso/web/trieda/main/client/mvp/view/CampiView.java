package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.extjs.gxt.ui.client.data.RpcProxy;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.grid.CheckColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.toolbar.SeparatorToolItem;
import com.gapso.web.trieda.main.client.mvp.presenter.CampiPresenter;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.EstadoComboBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleFilter;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;

public class CampiView extends MyComposite implements CampiPresenter.Display {

	private SimpleToolBar toolBar;
	private SimpleGrid<CampusDTO> gridPanel;
	private SimpleFilter filter;
	private TextField<String> nomeBuscaTextField;
	private TextField<String> codigoBuscaTextField;
	private EstadoComboBox estadoBuscaComboBox;
	private TextField<String> municipioBuscaTextField;
	private TextField<String> bairroBuscaTextField;
	private Button unidadesDeslocamentoBT;
	private Button disponibilidadeBT;
	private ContentPanel panel;
	private GTabItem tabItem;
	
	public CampiView() {
		initUI();
	}
	
	private void initUI() {
		panel = new ContentPanel(new BorderLayout());
		panel.setHeading(getI18nConstants().campiHeadingPanel());
		createToolBar();
		createGrid();
		createFilter();
		createTabItem();
		initComponent(tabItem);
	}
	
	private void createTabItem() {
		tabItem = new GTabItem(getI18nConstants().campi(), Resources.DEFAULTS.campus16());
		tabItem.setContent(panel);
	}
	
	private void createToolBar() {
		toolBar = new SimpleToolBar(this);
		toolBar.add(new SeparatorToolItem());
		unidadesDeslocamentoBT = toolBar.createButton(getI18nConstants().deslocamentoUnidadesCampus(), Resources.DEFAULTS.deslocamentoUnidade16());
		toolBar.add(unidadesDeslocamentoBT);
		disponibilidadeBT = toolBar.createButton(getI18nConstants().disponibilidadesSemanaLetiva(), Resources.DEFAULTS.disponibilidade16());
		toolBar.add(disponibilidadeBT);
		panel.setTopComponent(toolBar);
	}
	
	private void createGrid() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
	    bld.setMargins(new Margins(5, 5, 5, 5));
	    
	    gridPanel = new SimpleGrid<CampusDTO>(getColumnList(), this);
	    panel.add(gridPanel, bld);
	}

	public List<ColumnConfig> getColumnList() {
		List<ColumnConfig> list = new ArrayList<ColumnConfig>();
		list.add(new ColumnConfig(CampusDTO.PROPERTY_CODIGO, getI18nConstants().codigo(), 100));
		list.add(new ColumnConfig(CampusDTO.PROPERTY_NOME, getI18nConstants().nome(), 100));
		list.add(new ColumnConfig(CampusDTO.PROPERTY_VALOR_CREDITO, getI18nConstants().custoMedioCredito(), 170));
		list.add(new ColumnConfig(CampusDTO.PROPERTY_ESTADO, getI18nConstants().estado(), 100));
		list.add(new ColumnConfig(CampusDTO.PROPERTY_MUNICIPIO, getI18nConstants().municipio(), 100));
		list.add(new ColumnConfig(CampusDTO.PROPERTY_BAIRRO, getI18nConstants().bairro(), 100));
		list.add(new CheckColumnConfig(CampusDTO.PROPERTY_OTIMIZADO_TATICO, getI18nConstants().otimizadoTatico() + "?", 100));
		list.add(new CheckColumnConfig(CampusDTO.PROPERTY_OTIMIZADO_OPERACIONAL, getI18nConstants().otimizadoOperacional() + "?", 100));
		list.add(new CheckColumnConfig(CampusDTO.PROPERTY_PUBLICADO, getI18nConstants().publicado() + "?", 100));
		return list;
	}

	private void createFilter() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.EAST);
		bld.setMargins(new Margins(5, 5, 5, 0));
		bld.setCollapsible(true);
		
		filter = new SimpleFilter();
		nomeBuscaTextField = new TextField<String>();
		nomeBuscaTextField.setFieldLabel(getI18nConstants().nome());
		codigoBuscaTextField = new TextField<String>();
		codigoBuscaTextField.setFieldLabel(getI18nConstants().codigo());
		estadoBuscaComboBox = new EstadoComboBox();
		estadoBuscaComboBox.setFieldLabel(getI18nConstants().estado());
		municipioBuscaTextField = new TextField<String>();
		municipioBuscaTextField.setFieldLabel(getI18nConstants().municipio());
		bairroBuscaTextField = new TextField<String>();
		bairroBuscaTextField.setFieldLabel(getI18nConstants().bairro());
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

	@Override
	public Button getUnidadeDeslocamentosButton() {
		return unidadesDeslocamentoBT;
	}
	
	@Override
	public Button getDisponibilidadeButton() {
		return disponibilidadeBT;
	}
	
}
