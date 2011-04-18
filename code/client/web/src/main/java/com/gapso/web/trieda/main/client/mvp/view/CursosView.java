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
import com.gapso.web.trieda.main.client.mvp.presenter.CursosPresenter;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleFilter;
import com.gapso.web.trieda.shared.util.view.SimpleGrid;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;
import com.gapso.web.trieda.shared.util.view.TipoCursoComboBox;

public class CursosView extends MyComposite implements CursosPresenter.Display {

	private SimpleToolBar toolBar;
	private SimpleGrid<CursoDTO> gridPanel;
	private SimpleFilter filter;
	private TextField<String> nomeBuscaTextField;
	private TextField<String> codigoBuscaTextField;
	private TipoCursoComboBox tipoCursoBuscaComboBox;
	private Button curriculosBT;
	private ContentPanel panel;
	private GTabItem tabItem;
	
	public CursosView() {
		initUI();
	}
	
	private void initUI() {
		panel = new ContentPanel(new BorderLayout());
		panel.setHeading("Master Data » Cursos");
		createToolBar();
		createGrid();
		createFilter();
		createTabItem();
		initComponent(tabItem);
	}
	
	private void createTabItem() {
		tabItem = new GTabItem("Cursos", Resources.DEFAULTS.curso16());
		tabItem.setContent(panel);
	}
	
	private void createToolBar() {
		toolBar = new SimpleToolBar(this);
		toolBar.add(new SeparatorToolItem());
		curriculosBT = toolBar.createButton("Matrizes Curriculares", Resources.DEFAULTS.matrizCurricular16());
		toolBar.add(curriculosBT);
		panel.setTopComponent(toolBar);
	}
	
	private void createGrid() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
	    bld.setMargins(new Margins(5, 5, 5, 5));
	    
	    gridPanel = new SimpleGrid<CursoDTO>(getColumnList(), this);
	    panel.add(gridPanel, bld);
	}

	public List<ColumnConfig> getColumnList() {
		List<ColumnConfig> list = new ArrayList<ColumnConfig>();
		list.add(new ColumnConfig(CursoDTO.PROPERTY_NOME, "Nome", 100));
		list.add(new ColumnConfig(CursoDTO.PROPERTY_CODIGO, "Código", 100));
		list.add(new ColumnConfig(CursoDTO.PROPERTY_TIPO_STRING, "Tipo", 100));
		list.add(new ColumnConfig(CursoDTO.PROPERTY_NUM_MIN_DOUTORES, "% Min. PhD", 100));
		list.add(new ColumnConfig(CursoDTO.PROPERTY_NUM_MIN_MESTRES, "% Min. MSc", 100));
		list.add(new ColumnConfig(CursoDTO.PROPERTY_MAX_DISCIPLINAS_PELO_PROFESSOR, "Max. Disc. por Prof.", 110));
		list.add(new CheckColumnConfig(CursoDTO.PROPERTY_ADM_MAIS_DE_UMA_DISCIPLINA, "Permite mais de uma Disc. por Prof.?", 200));
		return list;
	}

	private void createFilter() {
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.EAST);
		bld.setMargins(new Margins(5, 5, 5, 0));
		bld.setCollapsible(true);
		
		filter = new SimpleFilter();
		tipoCursoBuscaComboBox = new TipoCursoComboBox();
		tipoCursoBuscaComboBox.setFieldLabel("Tipo");
		nomeBuscaTextField = new TextField<String>();
		nomeBuscaTextField.setFieldLabel("Nome");
		codigoBuscaTextField = new TextField<String>();
		codigoBuscaTextField.setFieldLabel("Código");
		
		filter.addField(tipoCursoBuscaComboBox);
		filter.addField(nomeBuscaTextField);
		filter.addField(codigoBuscaTextField);
		
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
	public SimpleGrid<CursoDTO> getGrid() {
		return gridPanel;
	}
	
	@Override
	public void setProxy(RpcProxy<PagingLoadResult<CursoDTO>> proxy) {
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
	public TipoCursoComboBox getTipoCursoBuscaComboBox() {
		return tipoCursoBuscaComboBox;
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
	public Button getCurriculosButton() {
		return curriculosBT;
	}

}