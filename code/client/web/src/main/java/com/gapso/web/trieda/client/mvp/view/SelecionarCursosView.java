package com.gapso.web.trieda.client.mvp.view;

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
import com.gapso.web.trieda.client.mvp.presenter.SelecionarCursosPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.CampusComboBox;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class SelecionarCursosView extends MyComposite implements SelecionarCursosPresenter.Display {

	private SimpleModal simpleModal;
	
	private ContentPanel panel;
	private ContentPanel panelLists;
	
	private CampusComboBox campusCB;
	
	private ListView<CursoDTO> naoSelecionadoList;
	private ListView<CursoDTO> selecionadoList;
	
	private Button adicionaBT;
	private Button removeBT;
	
	
	public SelecionarCursosView() {
		initUI();
		// TODO
		// initComponent(simpleModal);
	}

	private void initUI() {
		simpleModal = new SimpleModal("Fechar", null, "Selecionar cursos", Resources.DEFAULTS.curso16());
		simpleModal.setWidth(600);
		simpleModal.setHeight(400);
		createForm();
		simpleModal.setContent(panel);
	}
	
	private void createForm() {
		panel = new ContentPanel(new BorderLayout());
		panel.setHeaderVisible(false);
		
		FormData formData = new FormData("100%");
		FormPanel formPanel = new FormPanel();
		formPanel.setBodyBorder(false);
		formPanel.setLabelWidth(100);
		formPanel.setLabelAlign(LabelAlign.RIGHT);
		formPanel.setHeaderVisible(false);
		formPanel.setAutoHeight(true);
		
		campusCB = new CampusComboBox();
		formPanel.add(campusCB, formData);
		
		panel.setTopComponent(formPanel);
		
		panelLists = new ContentPanel(new RowLayout(Orientation.HORIZONTAL));
		panelLists.setHeaderVisible(false);
		panelLists.setBodyBorder(false);
		
		ContentPanel naoSelecionadoListPanel = new ContentPanel(new FitLayout());
		naoSelecionadoListPanel.setHeading("Cursos não selecionado");
		naoSelecionadoList = new ListView<CursoDTO>();
		naoSelecionadoList.disable();
		naoSelecionadoList.setDisplayProperty(CursoDTO.PROPERTY_DISPLAY_TEXT);
		naoSelecionadoList.getSelectionModel().setSelectionMode(SelectionMode.SINGLE);
		naoSelecionadoListPanel.add(naoSelecionadoList);
		
		ContentPanel selecionadoListPanel = new ContentPanel(new FitLayout());
		selecionadoListPanel.setHeading("Cursos selecionados");
		selecionadoList = new ListView<CursoDTO>();
		selecionadoList.setDisplayProperty(CursoDTO.PROPERTY_DISPLAY_TEXT);
		selecionadoList.getSelectionModel().setSelectionMode(SelectionMode.SINGLE);
		selecionadoListPanel.add(selecionadoList);
		
		panelLists.add(naoSelecionadoListPanel, new RowData(.5, 1, new Margins(0, 0, 10, 10)));
		panelLists.add(getAtualizaSalasButtonsPanel(), new RowData(30, 1, new Margins(0, 0, 0, 0)));
		panelLists.add(selecionadoListPanel, new RowData(.5, 1, new Margins(0, 10, 10, 0)));
		
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
	public CampusComboBox getCampusComboBox() {
		return campusCB;
	}

	@Override
	public ListView<CursoDTO> getNaoSelecionadoList() {
		return naoSelecionadoList;
	}

	@Override
	public ListView<CursoDTO> getSelecionadoList() {
		return selecionadoList;
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
	public SimpleModal getSimpleModal() {
		return simpleModal;
	}

	@Override
	public Button getFecharBT() {
		return simpleModal.getSalvarBt();
	}
}
