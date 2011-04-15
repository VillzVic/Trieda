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
import com.gapso.web.trieda.main.client.mvp.presenter.VincularAreasTitulacaoPresenter;
import com.gapso.web.trieda.main.client.util.resources.Resources;
import com.gapso.web.trieda.main.client.util.view.CursoComboBox;
import com.gapso.web.trieda.main.client.util.view.GTabItem;
import com.gapso.web.trieda.shared.dtos.AreaTitulacaoDTO;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class VincularAreasTitulacaoView extends MyComposite implements VincularAreasTitulacaoPresenter.Display {

	private ContentPanel panel;
	private ContentPanel panelLists;
	
	private CursoComboBox cursoCB;
	
	private ListView<AreaTitulacaoDTO> naoVinculadaList;
	private ListView<AreaTitulacaoDTO> vinculadaList;
	
	private Button adicionaBT;
	private Button removeBT;
	
	private GTabItem tabItem;
	
	public VincularAreasTitulacaoView() {
		initUI();
		createTabItem();
		createForm();
		initComponent(tabItem);
	}

	private void initUI() {
		panel = new ContentPanel(new BorderLayout());
		panel.setHeading("Master Data » Vincular Áreas de Titulação");
	}

	private void createTabItem() {
		tabItem = new GTabItem("Vincular Áreas de Titulação", Resources.DEFAULTS.areaTitulacao16());
		tabItem.setContent(panel);
	}
	
	private void createForm() {
		FormData formData = new FormData("100%");
		FormPanel formPanel = new FormPanel();
		formPanel.setBodyBorder(false);
		formPanel.setLabelWidth(100);
		formPanel.setLabelAlign(LabelAlign.RIGHT);
		formPanel.setHeaderVisible(false);
		formPanel.setAutoHeight(true);
		
		cursoCB = new CursoComboBox();
		formPanel.add(cursoCB, formData);
		
		panel.setTopComponent(formPanel);
		
		panelLists = new ContentPanel(new RowLayout(Orientation.HORIZONTAL));
		panelLists.setHeaderVisible(false);
		panelLists.setBodyBorder(false);
		
		ContentPanel naoVinculadaListPanel = new ContentPanel(new FitLayout());
		naoVinculadaListPanel.setHeading("Área(s) de Titulação NÃO vinculada(s) ao Curso");
		naoVinculadaList = new ListView<AreaTitulacaoDTO>();
		naoVinculadaList.disable();
		naoVinculadaList.setDisplayProperty(AreaTitulacaoDTO.PROPERTY_DISPLAY_TEXT);
		naoVinculadaList.getSelectionModel().setSelectionMode(SelectionMode.MULTI);
		naoVinculadaListPanel.add(naoVinculadaList);
		
		ContentPanel vinculadaListPanel = new ContentPanel(new FitLayout());
		vinculadaListPanel.setHeading("Área(s) de Titulação vinculada(s) ao Curso");
		vinculadaList = new ListView<AreaTitulacaoDTO>();
		vinculadaList.disable();
		vinculadaList.setDisplayProperty(AreaTitulacaoDTO.PROPERTY_DISPLAY_TEXT);
		vinculadaList.getSelectionModel().setSelectionMode(SelectionMode.MULTI);
		vinculadaListPanel.add(vinculadaList);
		
		panelLists.add(naoVinculadaListPanel, new RowData(.5, 1, new Margins(0, 0, 10, 10)));
		panelLists.add(getAtualizaSalasButtonsPanel(), new RowData(30, 1, new Margins(0, 0, 0, 0)));
		panelLists.add(vinculadaListPanel, new RowData(.5, 1, new Margins(0, 10, 10, 0)));
		
		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
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
		adicionaBT.setEnabled(false);
		adicionaBT.setSize(30, 50);
		adicionaBT.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.toRight24()));
		
		removeBT = new Button();
		removeBT.setEnabled(false);
		removeBT.setSize(30, 50);
		removeBT.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.toLeft24()));
		
		RowData rowData = new RowData(-1, -1, new Margins(4, 0, 4, 0));
		
		panel.add(adicionaBT, rowData);
		panel.add(removeBT, rowData);
		
		return panel;
	}
	
	@Override
	public CursoComboBox getCursoComboBox() {
		return cursoCB;
	}

	@Override
	public ListView<AreaTitulacaoDTO> getNaoVinculadaList() {
		return naoVinculadaList;
	}

	@Override
	public ListView<AreaTitulacaoDTO> getVinculadaList() {
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
}
