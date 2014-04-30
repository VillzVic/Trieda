package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.Style.Orientation;
import com.extjs.gxt.ui.client.Style.SelectionMode;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.util.Padding;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.ListView;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.layout.BorderLayout;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayout;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayoutData;
import com.extjs.gxt.ui.client.widget.layout.RowData;
import com.extjs.gxt.ui.client.widget.layout.RowLayout;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayout.HBoxLayoutAlign;
import com.gapso.web.trieda.main.client.mvp.presenter.CompararCenariosFormPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class CompararCenariosFormView extends MyComposite implements CompararCenariosFormPresenter.Display {
	
	private SimpleModal simpleModal;
	
	private ContentPanel panel;
	private LayoutContainer panelLists;
	
	private ListView<CenarioDTO> naoSelecionadoList;
	private ListView<CenarioDTO> selecionadoList;
	
	private Button adicionaBT;
	private Button removeBT;
	
	public CompararCenariosFormView() {
		initUI();
	}

	private void initUI() {
		simpleModal = new SimpleModal("Comparar","Cancelar","Selecionar Cenários a Serem Comparados",Resources.DEFAULTS.cenario16());
		simpleModal.setHeight(455);
		simpleModal.setWidth(620);
		createForm();
		simpleModal.setContent(panel);
	}
	
	private void createForm() {
		panel = new ContentPanel(new BorderLayout());
		panel.setHeaderVisible(false);

		HBoxLayout layout2 = new HBoxLayout();  
		layout2.setHBoxLayoutAlign(HBoxLayoutAlign.MIDDLE);  
		layout2.setPadding(new Padding(5));  
		panelLists = new LayoutContainer(layout2);

		ContentPanel naoSelecionadoListPanel = new ContentPanel(new FitLayout());
		naoSelecionadoListPanel.setHeadingHtml("Cenários não selecionados");
		naoSelecionadoListPanel.setWidth(267);
		naoSelecionadoListPanel.setHeight(320);
		naoSelecionadoList = new ListView<CenarioDTO>();
		naoSelecionadoList.setDisplayProperty(CenarioDTO.PROPERTY_DISPLAY_TEXT);
		naoSelecionadoList.getSelectionModel().setSelectionMode(SelectionMode.MULTI);
		naoSelecionadoListPanel.add(naoSelecionadoList);

		ContentPanel selecionadoListPanel = new ContentPanel(new FitLayout());
		selecionadoListPanel.setHeadingHtml("Cenários selecionados");
		selecionadoListPanel.setWidth(267);
		selecionadoListPanel.setHeight(320);
		selecionadoList = new ListView<CenarioDTO>();
		selecionadoList.setDisplayProperty(CenarioDTO.PROPERTY_DISPLAY_TEXT);
		selecionadoList.getSelectionModel().setSelectionMode(SelectionMode.MULTI);
		selecionadoListPanel.add(selecionadoList);

		panelLists.add(naoSelecionadoListPanel,new HBoxLayoutData(new Margins(5)));
		panelLists.add(getAtualizaSalasButtonsPanel(),new HBoxLayoutData(new Margins(5)));
		panelLists.add(selecionadoListPanel,new HBoxLayoutData(new Margins(5)));

		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
		bld.setMargins(new Margins(0, 0, 0, 0));
		panel.setBodyBorder(false);
		panel.add(panelLists, bld);
	}

	private LayoutContainer getAtualizaSalasButtonsPanel()
	{
		ContentPanel panel = new ContentPanel();
		panel.setHeaderVisible(false);
		panel.setBodyBorder(false);
		panel.setBodyStyle( "display: table-cell; vertical-align: middle; background-color: #DFE8F6;" );
		panel.setLayout(new RowLayout(Orientation.VERTICAL));

		adicionaBT = new Button();
		adicionaBT.setSize(30, 50);
		adicionaBT.setIcon( AbstractImagePrototype.create( Resources.DEFAULTS.toRight24() ) );

		removeBT = new Button();
		removeBT.setSize(30, 50);
		removeBT.setIcon( AbstractImagePrototype.create( Resources.DEFAULTS.toLeft24() ) );

		RowData rowData = new RowData(-1, -1, new Margins(4, 0, 4, 0));

		panel.add(adicionaBT, rowData);
		panel.add(removeBT, rowData);

		return panel;
	}

	@Override
	public ListView< CenarioDTO > getNaoSelecionadoList()
	{
		return naoSelecionadoList;
	}

	@Override
	public ListView< CenarioDTO > getSelecionadoList()
	{
		return selecionadoList;
	}

	@Override
	public Button getAdicionaBT()
	{
		return adicionaBT;
	}

	@Override
	public Button getRemoveBT()
	{
		return removeBT;
	}

	@Override
	public SimpleModal getSimpleModal()
	{
		return simpleModal;
	}

	@Override
	public Button getFecharBT()
	{
		return simpleModal.getSalvarBt();
	}
}
