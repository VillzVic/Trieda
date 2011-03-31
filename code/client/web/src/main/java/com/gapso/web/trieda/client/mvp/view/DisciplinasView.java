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
import com.extjs.gxt.ui.client.widget.grid.CheckColumnConfig;
import com.extjs.gxt.ui.client.widget.grid.ColumnConfig;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.toolbar.SeparatorToolItem;
import com.gapso.web.trieda.client.mvp.presenter.DisciplinasPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.SimpleFilter;
import com.gapso.web.trieda.client.util.view.SimpleGrid;
import com.gapso.web.trieda.client.util.view.SimpleToolBar;
import com.gapso.web.trieda.client.util.view.TipoDisciplinaComboBox;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;

public class DisciplinasView extends MyComposite implements DisciplinasPresenter.Display {

	private SimpleToolBar toolBar;
	private SimpleGrid<DisciplinaDTO> gridPanel;
	private SimpleFilter filter;
	private TextField<String> nomeBuscaTextField;
	private TextField<String> codigoBuscaTextField;
	private TipoDisciplinaComboBox tipoDisciplinaBuscaComboBox;
	private Button disponibilidadeBT;
	private Button divisaoCreditoBT;
	private ContentPanel panel;
	private GTabItem tabItem;
	
	public DisciplinasView() {
		initUI();
	}
	
	private void initUI() {
		panel = new ContentPanel(new BorderLayout());
		panel.setHeading("Master Data » Disciplinas");
		createToolBar();
		createGrid();
		createFilter();
		createTabItem();
		initComponent(tabItem);
	}
	
	private void createTabItem() {
		tabItem = new GTabItem("Disciplinas", Resources.DEFAULTS.disciplina16());
		tabItem.setContent(panel);
	}
	
	private void createToolBar() {
		toolBar = new SimpleToolBar(this);
		toolBar.add(new SeparatorToolItem());
		divisaoCreditoBT = toolBar.createButton("Divisão de Créditos da disciplina", Resources.DEFAULTS.divisaoDeCreditos16());
		toolBar.add(divisaoCreditoBT);
		disponibilidadeBT = toolBar.createButton("Disponibilidade da Disciplina", Resources.DEFAULTS.disponibilidade16());
		toolBar.add(disponibilidadeBT);
		panel.setTopComponent(toolBar);
	}
	
	private void createGrid() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
	    bld.setMargins(new Margins(5, 5, 5, 5));
	    
	    gridPanel = new SimpleGrid<DisciplinaDTO>(getColumnList(), this);
	    panel.add(gridPanel, bld);
	}

	public List<ColumnConfig> getColumnList() {
		List<ColumnConfig> list = new ArrayList<ColumnConfig>();
		list.add(new ColumnConfig(DisciplinaDTO.PROPERTY_NOME, "Nome", 100));
		list.add(new ColumnConfig(DisciplinaDTO.PROPERTY_CODIGO, "Código", 100));
		list.add(new ColumnConfig(DisciplinaDTO.PROPERTY_CREDITOS_TEORICO, "Créditos Teóricos", 100));
		list.add(new ColumnConfig(DisciplinaDTO.PROPERTY_CREDITOS_PRATICO, "Créditos Práticos", 100));
		list.add(new CheckColumnConfig(DisciplinaDTO.PROPERTY_LABORATORIO, "Usa Laboratório?", 100));
		list.add(new ColumnConfig(DisciplinaDTO.PROPERTY_TIPO_STRING, "Tipo de disciplina", 100));
		list.add(new ColumnConfig(DisciplinaDTO.PROPERTY_DIFICULDADE, "Nível de Dificuldade", 100));
		list.add(new ColumnConfig(DisciplinaDTO.PROPERTY_MAX_ALUNOS_TEORICO, "Max. Alunos - Teórico", 100));
		list.add(new ColumnConfig(DisciplinaDTO.PROPERTY_MAX_ALUNOS_PRATICO, "Max. Alunos - Prático", 100));
		return list;
	}

	private void createFilter() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.EAST);
		bld.setMargins(new Margins(5, 5, 5, 0));
		bld.setCollapsible(true);
		
		filter = new SimpleFilter();
		nomeBuscaTextField = new TextField<String>();
		nomeBuscaTextField.setFieldLabel("Nome");
		codigoBuscaTextField = new TextField<String>();
		codigoBuscaTextField.setFieldLabel("Código");
		tipoDisciplinaBuscaComboBox = new TipoDisciplinaComboBox();
		tipoDisciplinaBuscaComboBox.setFieldLabel("Tipo de disciplina");
		
		filter.addField(nomeBuscaTextField);
		filter.addField(codigoBuscaTextField);
		filter.addField(tipoDisciplinaBuscaComboBox);
		
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
	public SimpleGrid<DisciplinaDTO> getGrid() {
		return gridPanel;
	}
	
	@Override
	public void setProxy(RpcProxy<PagingLoadResult<DisciplinaDTO>> proxy) {
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
	public TipoDisciplinaComboBox getTipoDisciplinaBuscaComboBox() {
		return tipoDisciplinaBuscaComboBox;
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
	public Button getDisponibilidadeButton() {
		return disponibilidadeBT;
	}
	
	@Override
	public Button getDivisaoCreditoButton() {
		return divisaoCreditoBT;
	}

}
