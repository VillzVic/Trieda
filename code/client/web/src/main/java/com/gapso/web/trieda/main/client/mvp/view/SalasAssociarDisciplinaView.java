package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.Style.Orientation;
import com.extjs.gxt.ui.client.Style.SelectionMode;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.ListView;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.RowData;
import com.extjs.gxt.ui.client.widget.layout.RowLayout;
import com.extjs.gxt.ui.client.widget.menu.MenuItem;
import com.extjs.gxt.ui.client.widget.toolbar.SeparatorToolItem;
import com.gapso.web.trieda.main.client.mvp.presenter.SalasAssociarDisciplinaPresenter;
import com.gapso.web.trieda.shared.dtos.AreaTitulacaoDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.DisciplinaAutoCompleteBox;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.gapso.web.trieda.shared.util.view.SimpleToolBar;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class SalasAssociarDisciplinaView extends MyComposite implements
	SalasAssociarDisciplinaPresenter.Display
{
	
	private SimpleToolBar toolBar;
	private ContentPanel panel;
	private ContentPanel panelLists;
	
	private DisciplinaAutoCompleteBox disciplinaCB;
	private DisciplinaDTO disciplinaDTO;
	private CenarioDTO cenarioDTO;
	
	private ListView<SalaDTO> naoVinculadaList;
	private ListView<SalaDTO> vinculadaList;
	
	private Button adicionaBT;
	private Button removeBT;
	private Button associarDisciplinasPraticasLaboratoriosBT;
	
	private GTabItem tabItem;
	
	public SalasAssociarDisciplinaView( CenarioDTO cenarioDTO, DisciplinaDTO disciplinaDTO ) {
		this.disciplinaDTO = disciplinaDTO;
		this.cenarioDTO = cenarioDTO;
		initUI();
		createToolBar();
		createTabItem();
		createForm();
		initComponent(tabItem);
	}
	
	private void initUI() {
		panel = new ContentPanel(new BorderLayout());
		panel.setHeadingHtml(cenarioDTO.getNome() + " » Associação de Disciplinas à Ambientes");
	}
	
	private void createTabItem() {
		tabItem = new GTabItem("Associação de Disciplinas à Ambientes", Resources.DEFAULTS.associacaoDisciplinaSala16());
		tabItem.setContent(panel);
	}
	
	private void createToolBar() {
		toolBar = new SimpleToolBar(false, false, false, true, true, this);
		this.associarDisciplinasPraticasLaboratoriosBT = this.toolBar.createButton("Associar Disciplinas a Laboratórios",Resources.DEFAULTS.associacaoDisciplinaSala16());
		toolBar.add(new SeparatorToolItem());
		this.toolBar.add(this.associarDisciplinasPraticasLaboratoriosBT);
		panel.setTopComponent(toolBar);
	}
	
	private void createForm() {
		FormData formData = new FormData("100%");
		FormPanel formPanel = new FormPanel();
		formPanel.setBodyBorder(false);
		formPanel.setLabelWidth(100);
		formPanel.setLabelAlign(LabelAlign.RIGHT);
		formPanel.setHeaderVisible(false);
		formPanel.setAutoHeight(true);
		
		disciplinaCB = new DisciplinaAutoCompleteBox(cenarioDTO);
		disciplinaCB.setValue(this.disciplinaDTO);
		formPanel.add(disciplinaCB, formData);
		
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.NORTH);
		bld.setMargins(new Margins(0, 0, 0, 0));
		bld.setSize(42);
		panel.add(formPanel, bld);
		
		panelLists = new ContentPanel(new RowLayout(Orientation.HORIZONTAL));
		panelLists.setHeaderVisible(false);
		panelLists.setBodyBorder(false);
		
		ContentPanel naoVinculadaListPanel = new ContentPanel(new FitLayout());
		naoVinculadaListPanel.setHeadingHtml("Ambiente(s) NÃO vinculado(s) a Disciplina");
		naoVinculadaList = new ListView<SalaDTO>();
		if(disciplinaDTO.getId() == null)
			naoVinculadaList.disable();
		naoVinculadaList.setDisplayProperty(AreaTitulacaoDTO.PROPERTY_DISPLAY_TEXT);
		naoVinculadaList.getSelectionModel().setSelectionMode(SelectionMode.MULTI);
		naoVinculadaListPanel.add(naoVinculadaList);
		
		ContentPanel vinculadaListPanel = new ContentPanel(new FitLayout());
		vinculadaListPanel.setHeadingHtml("Ambiente(s) vinculado(s) a Disciplina");
		vinculadaList = new ListView<SalaDTO>();
		if(disciplinaDTO.getId() == null)
			vinculadaList.disable();
		vinculadaList.setDisplayProperty(AreaTitulacaoDTO.PROPERTY_DISPLAY_TEXT);
		vinculadaList.getSelectionModel().setSelectionMode(SelectionMode.MULTI);
		vinculadaListPanel.add(vinculadaList);
		
		panelLists.add(naoVinculadaListPanel, new RowData(.5, 1, new Margins(0, 0, 10, 10)));
		panelLists.add(getAtualizaSalasButtonsPanel(), new RowData(30, 1, new Margins(0, 0, 0, 0)));
		panelLists.add(vinculadaListPanel, new RowData(.5, 1, new Margins(0, 10, 10, 0)));
		
		bld = new BorderLayoutData(LayoutRegion.CENTER);
		bld.setMargins(new Margins(0, 0, 0, 0));
		panel.setBodyBorder(false);
		panelLists.setBodyStyle("background-color: #DFE8F6;");
		panel.add(panelLists, bld);
	}
	
	private LayoutContainer getAtualizaSalasButtonsPanel() {
		ContentPanel panel = new ContentPanel();
		panel.setHeaderVisible(false);
		panel.setBodyBorder(false);
		panel.setBodyStyle("display: table-cell; vertical-align: middle; background-color: #DFE8F6;");
		panel.setLayout(new RowLayout(Orientation.VERTICAL));
		
		adicionaBT = new Button();
		adicionaBT.setEnabled((disciplinaDTO.getId() == null) ? false : true);
		adicionaBT.setSize(30, 50);
		adicionaBT.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.toRight24()));
		
		removeBT = new Button();
		removeBT.setEnabled((disciplinaDTO.getId() == null) ? false : true);
		removeBT.setSize(30, 50);
		removeBT.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.toLeft24()));
		
		RowData rowData = new RowData(-1, -1, new Margins(4, 0, 4, 0));
		
		panel.add(adicionaBT, rowData);
		panel.add(removeBT, rowData);
		
		return panel;
	}
	
	@Override
	public DisciplinaAutoCompleteBox getDisciplinaComboBox() {
		return disciplinaCB;
	}
	
	@Override
		public ListView<SalaDTO> getNaoVinculadaList() {
		return naoVinculadaList;
	}
	
	@Override
		public ListView<SalaDTO> getVinculadaList() {
		return vinculadaList;
	}
	
	@Override
		public Button getAdicionaBT() {
		return adicionaBT;
	}
	
	@Override
		public Button getRemoveBT() {
		return removeBT;
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
	public DisciplinaDTO getDisciplinaDTO() {
		return disciplinaDTO;
	}

	@Override
	public Button getAssociarDisciplinasPraticasLaboratoriosBT()
	{
		return this.associarDisciplinasPraticasLaboratoriosBT;
	}
}
