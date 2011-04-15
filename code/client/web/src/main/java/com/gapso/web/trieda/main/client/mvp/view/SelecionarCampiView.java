package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.Style.Orientation;
import com.extjs.gxt.ui.client.Style.SelectionMode;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.ListView;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.layout.RowData;
import com.extjs.gxt.ui.client.widget.layout.RowLayout;
import com.gapso.web.trieda.main.client.mvp.presenter.SelecionarCampiPresenter;
import com.gapso.web.trieda.main.client.util.resources.Resources;
import com.gapso.web.trieda.main.client.util.view.SimpleModal;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class SelecionarCampiView extends MyComposite implements SelecionarCampiPresenter.Display {

	private SimpleModal simpleModal;
	
	private ContentPanel panel;
	private ContentPanel panelLists;
	
	
	private ListView<CampusDTO> naoSelecionadoList;
	private ListView<CampusDTO> selecionadoList;
	
	private Button adicionaBT;
	private Button removeBT;
	
	
	public SelecionarCampiView() {
		initUI();
		// TODO
		// initComponent(simpleModal);
	}

	private void initUI() {
		simpleModal = new SimpleModal("Parâmetros de Planejamento", "Cancelar", "Selecionar campus para planejamento", Resources.DEFAULTS.campus16());
		simpleModal.setWidth(600);
		simpleModal.setHeight(400);
		createForm();
		simpleModal.setContent(panel);
	}
	
	private void createForm() {
		panel = new ContentPanel(new BorderLayout());
		panel.setHeaderVisible(false);
		
		panelLists = new ContentPanel(new RowLayout(Orientation.HORIZONTAL));
		panelLists.setHeaderVisible(false);
		panelLists.setBodyBorder(false);
		
		ContentPanel naoSelecionadoListPanel = new ContentPanel(new FitLayout());
		naoSelecionadoListPanel.setHeading("Campus não selecionado");
		naoSelecionadoList = new ListView<CampusDTO>();
		naoSelecionadoList.setDisplayProperty(CampusDTO.PROPERTY_DISPLAY_TEXT);
		naoSelecionadoList.getSelectionModel().setSelectionMode(SelectionMode.MULTI);
		naoSelecionadoListPanel.add(naoSelecionadoList);
		
		ContentPanel selecionadoListPanel = new ContentPanel(new FitLayout());
		selecionadoListPanel.setHeading("Campus selecionados");
		selecionadoList = new ListView<CampusDTO>();
		selecionadoList.setDisplayProperty(CampusDTO.PROPERTY_DISPLAY_TEXT);
		selecionadoList.getSelectionModel().setSelectionMode(SelectionMode.MULTI);
		selecionadoListPanel.add(selecionadoList);
		
		panelLists.add(naoSelecionadoListPanel, new RowData(.5, 1, new Margins(10, 0, 10, 10)));
		panelLists.add(getAtualizaSalasButtonsPanel(), new RowData(30, 1, new Margins(10, 0, 0, 0)));
		panelLists.add(selecionadoListPanel, new RowData(.5, 1, new Margins(10, 10, 10, 0)));
		
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
		adicionaBT.setSize(30, 50);
		adicionaBT.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.toRight24()));
		
		removeBT = new Button();
		removeBT.setSize(30, 50);
		removeBT.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.toLeft24()));
		
		RowData rowData = new RowData(-1, -1, new Margins(4, 0, 4, 0));
		
		panel.add(adicionaBT, rowData);
		panel.add(removeBT, rowData);
		
		return panel;
	}

	@Override
	public ListView<CampusDTO> getNaoSelecionadoList() {
		return naoSelecionadoList;
	}

	@Override
	public ListView<CampusDTO> getSelecionadoList() {
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
