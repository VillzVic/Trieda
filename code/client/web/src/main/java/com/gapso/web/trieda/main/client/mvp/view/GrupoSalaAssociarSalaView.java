package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.Style.Orientation;
import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.ListView;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.RowData;
import com.extjs.gxt.ui.client.widget.layout.RowLayout;
import com.gapso.web.trieda.main.client.mvp.presenter.GrupoSalaAssociarSalaPresenter;
import com.gapso.web.trieda.main.client.util.resources.Resources;
import com.gapso.web.trieda.main.client.util.view.SimpleModal;
import com.gapso.web.trieda.shared.dtos.GrupoSalaDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class GrupoSalaAssociarSalaView extends MyComposite implements GrupoSalaAssociarSalaPresenter.Display {

	private SimpleModal simpleModal;
	private ContentPanel contentPanel;
	private ContentPanel panelLists;
	private TextField<String> nomeGrupoTF;
	private TextField<String> nomeCampusTF;
	private TextField<String> nomeUnidadeTF;
	private ListView<SalaDTO> andaresList;
	private ListView<SalaDTO> salasNaoPertencesList;
	private ListView<SalaDTO> salasPertencesList;
	private Button atualizaSalasDoAndarBT;
	private Button adicionaSalasBT;
	private Button removeSalasBT;
	private GrupoSalaDTO grupoSalaDTO;
	
	public GrupoSalaAssociarSalaView(GrupoSalaDTO grupoSalaDTO) {
		this.grupoSalaDTO = grupoSalaDTO;
		initUI();
		createForm();
		// TODO
//		initComponent(simpleModal);
	}
	
	private void initUI() {
		simpleModal = new SimpleModal("Associar Sala ao Grupo", Resources.DEFAULTS.sala16());
		simpleModal.setHeight(500);
		simpleModal.setWidth(600);
	}

	private void createForm() {
		
		FormData formData = new FormData("100%");
		FormPanel formPanel = new FormPanel();
		formPanel.setBodyBorder(false);
		formPanel.setLabelWidth(100);
		formPanel.setLabelAlign(LabelAlign.RIGHT);
		formPanel.setHeaderVisible(false);
		
		nomeGrupoTF = new TextField<String>();
		nomeGrupoTF.setValue(grupoSalaDTO.getNome());
		nomeGrupoTF.setFieldLabel("Nome do Grupo");
		nomeGrupoTF.setReadOnly(true);
		formPanel.add(nomeGrupoTF, formData);
		
		nomeCampusTF = new TextField<String>();
		nomeCampusTF.setValue(grupoSalaDTO.getCampusString());
		nomeCampusTF.setFieldLabel("Campus");
		nomeCampusTF.setReadOnly(true);
		formPanel.add(nomeCampusTF, formData);
		
		nomeUnidadeTF = new TextField<String>();
		nomeUnidadeTF.setValue(grupoSalaDTO.getUnidadeString());
		nomeUnidadeTF.setFieldLabel("Unidade");
		nomeUnidadeTF.setReadOnly(true);
		formPanel.add(nomeUnidadeTF, formData);
		
		FormButtonBinding binding = new FormButtonBinding(formPanel);
		binding.addButton(simpleModal.getSalvarBt());
		
		contentPanel = new ContentPanel(new RowLayout(Orientation.VERTICAL));
		contentPanel.setHeaderVisible(false);
		
		contentPanel.add(formPanel);
		
		panelLists = new ContentPanel(new RowLayout(Orientation.HORIZONTAL));
		panelLists.setHeaderVisible(false);
		panelLists.setHeight(330);
		panelLists.setBodyBorder(false);
		
		ContentPanel andaresListPanel = new ContentPanel(new FitLayout());
		andaresListPanel.setHeading("Andares");
		ListStore<SalaDTO> store = new ListStore<SalaDTO>();
		store.setDefaultSort(SalaDTO.PROPERTY_ANDAR, SortDir.ASC);
		andaresList = new ListView<SalaDTO>(store);
		andaresList.setDisplayProperty(SalaDTO.PROPERTY_ANDAR);
		andaresListPanel.add(andaresList);
		
		ContentPanel naoAssociadasListPanel = new ContentPanel(new FitLayout());
		naoAssociadasListPanel.setHeading("Sala(s) não associadas ao Grupo");
		store = new ListStore<SalaDTO>();
		store.setDefaultSort(SalaDTO.PROPERTY_CODIGO, SortDir.ASC);
		salasNaoPertencesList = new ListView<SalaDTO>(store);
		salasNaoPertencesList.setDisplayProperty(SalaDTO.PROPERTY_CODIGO);
		naoAssociadasListPanel.add(salasNaoPertencesList);
		
		ContentPanel associadasListPanel = new ContentPanel(new FitLayout());
		associadasListPanel.setHeading("Sala(s) associadas ao Grupo");
		store = new ListStore<SalaDTO>();
		store.setDefaultSort(SalaDTO.PROPERTY_CODIGO, SortDir.ASC);
		salasPertencesList = new ListView<SalaDTO>(store);
		salasPertencesList.setDisplayProperty(SalaDTO.PROPERTY_CODIGO);
		associadasListPanel.add(salasPertencesList);
		
		panelLists.add(andaresListPanel, new RowData(.2, 1, new Margins(0, 0, 0, 0)));
		panelLists.add(getAtualizaSalasDoAndarButtonsPanel(), new RowData(42, 1, new Margins(0, 5, 0, 5)));
		panelLists.add(naoAssociadasListPanel, new RowData(.4, 1, new Margins(0, 0, 0, 0)));
		panelLists.add(getAtualizaSalasButtonsPanel(), new RowData(42, 1, new Margins(0, 5, 0, 5)));
		panelLists.add(associadasListPanel, new RowData(.4, 1, new Margins(0, 0, 0, 0)));
		
		contentPanel.add(panelLists);
		
		simpleModal.setContent(contentPanel);
	}

	private LayoutContainer getAtualizaSalasDoAndarButtonsPanel() {
		ContentPanel panel = new ContentPanel();
		panel.setHeaderVisible(false);
		panel.setBodyBorder(false);
		panel.setBodyStyle("display: table-cell; vertical-align: middle");
		panel.setLayout(new RowLayout(Orientation.VERTICAL));
		
		atualizaSalasDoAndarBT = new Button();
		atualizaSalasDoAndarBT.setSize(30, 50);
		atualizaSalasDoAndarBT.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.toRight24()));
				
		RowData rowData = new RowData(-1, -1, new Margins(4, 0, 4, 0));
		
		panel.add(atualizaSalasDoAndarBT, rowData);
		
		return panel;
	}
	
	private LayoutContainer getAtualizaSalasButtonsPanel() {
		ContentPanel panel = new ContentPanel();
		panel.setHeaderVisible(false);
		panel.setBodyBorder(false);
		panel.setBodyStyle("display: table-cell; vertical-align: middle");
		panel.setLayout(new RowLayout(Orientation.VERTICAL));
		
		adicionaSalasBT = new Button();
		adicionaSalasBT.setSize(30, 50);
		adicionaSalasBT.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.toRight24()));
		
		removeSalasBT = new Button();
		removeSalasBT.setSize(30, 50);
		removeSalasBT.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.toLeft24()));
		
		RowData rowData = new RowData(-1, -1, new Margins(4, 0, 4, 0));
		
		panel.add(adicionaSalasBT, rowData);
		panel.add(removeSalasBT, rowData);
		
		return panel;
	}
	
	@Override
	public Button getSalvarButton() {
		return simpleModal.getSalvarBt();
	}
	
	@Override
	public GrupoSalaDTO getGrupoSalaDTO() {
		return grupoSalaDTO;
	}
	
	@Override
	public SimpleModal getSimpleModal() {
		return simpleModal;
	}

	@Override
	public ListView<SalaDTO> getAndaresList() {
		return andaresList;
	}

	@Override
	public ListView<SalaDTO> getSalasNaoPertencesList() {
		return salasNaoPertencesList;
	}

	@Override
	public ListView<SalaDTO> getSalasPertencesList() {
		return salasPertencesList;
	}

	@Override
	public Button getAtualizaSalasDoAndarBT() {
		return atualizaSalasDoAndarBT;
	}
	
	@Override
	public Button getAdicionaSalasBT() {
		return adicionaSalasBT;
	}
	
	@Override
	public Button getRemoveSalasBT() {
		return removeSalasBT;
	}

}
