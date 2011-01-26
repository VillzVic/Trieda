package com.gapso.web.trieda.client.mvp.view;

import com.extjs.gxt.ui.client.Style.LayoutRegion;
import com.extjs.gxt.ui.client.Style.Orientation;
import com.extjs.gxt.ui.client.Style.SelectionMode;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.util.Padding;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.ListView;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.layout.BorderLayoutData;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.RowData;
import com.extjs.gxt.ui.client.widget.layout.RowLayout;
import com.extjs.gxt.ui.client.widget.layout.VBoxLayout;
import com.extjs.gxt.ui.client.widget.layout.VBoxLayoutData;
import com.extjs.gxt.ui.client.widget.layout.VBoxLayout.VBoxLayoutAlign;
import com.gapso.web.trieda.client.mvp.model.ProfessorCampusDTO;
import com.gapso.web.trieda.client.mvp.model.ProfessorDTO;
import com.gapso.web.trieda.client.mvp.presenter.CampusProfessorFormPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.CampusComboBox;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class CampusProfessorFormView extends MyComposite implements CampusProfessorFormPresenter.Display {

	private SimpleModal simpleModal;
	private LayoutContainer container;
	private FormPanel formPanel;
	private ContentPanel panelLists;
	private CampusComboBox campusCB;
	private ListView<ProfessorDTO> professorNaoAssociadoList;
	private ListView<ProfessorDTO> professorAssociadoList;
	private Button adicionaBT;
	private Button removeBT;
	private ProfessorCampusDTO professorCampusDTO;
	
	public CampusProfessorFormView(ProfessorCampusDTO professorCampusDTO) {
		this.professorCampusDTO = professorCampusDTO;
		initUI();
		// TODO
//		initComponent(simpleModal);
//		setParent(null);
	}
	
	private void initUI() {
		String title = (professorCampusDTO.getId() == null)? "Cadastro de professores no campus" : "Edição de professores no campus";
		simpleModal = new SimpleModal(title, Resources.DEFAULTS.professor16());
		simpleModal.setHeight(400);
		simpleModal.setWidth(600);
		createForm();
		simpleModal.setContent(container);
	}

	private void createForm() {
		container = new LayoutContainer();
		VBoxLayout layout = new VBoxLayout();  
		layout.setPadding(new Padding(5));  
		layout.setVBoxLayoutAlign(VBoxLayoutAlign.STRETCH);  
		container.setLayout(layout);  
		
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);
		
		panelLists = new ContentPanel(new RowLayout(Orientation.HORIZONTAL));
		panelLists.setHeaderVisible(false);
		panelLists.setBodyBorder(false);
		
		ContentPanel naoVinculadaListPanel = new ContentPanel(new FitLayout());
		naoVinculadaListPanel.setHeading("Área(s) de Titulação NÃO vinculada(s) ao Curso");
		professorNaoAssociadoList = new ListView<ProfessorDTO>();
		professorNaoAssociadoList.disable();
		professorNaoAssociadoList.setDisplayProperty("codigo");
		professorNaoAssociadoList.getSelectionModel().setSelectionMode(SelectionMode.SINGLE);
		naoVinculadaListPanel.add(professorNaoAssociadoList);
		
		ContentPanel vinculadaListPanel = new ContentPanel(new FitLayout());
		vinculadaListPanel.setHeading("Área(s) de Titulação vinculada(s) ao Curso");
		professorAssociadoList = new ListView<ProfessorDTO>();
		professorAssociadoList.disable();
		professorAssociadoList.setDisplayProperty("codigo");
		professorAssociadoList.getSelectionModel().setSelectionMode(SelectionMode.SINGLE);
		vinculadaListPanel.add(professorAssociadoList);
		
		panelLists.add(naoVinculadaListPanel, new RowData(.5, 1, new Margins(0)));
		panelLists.add(getAtualizaProfessoresButtonsPanel(), new RowData(30, 1, new Margins(0)));
		panelLists.add(vinculadaListPanel, new RowData(.5, 1, new Margins(0)));
		
		container.add(formPanel, new VBoxLayoutData(new Margins(0, 0, 5, 0)));
		VBoxLayoutData flex = new VBoxLayoutData(new Margins(0));  
		flex.setFlex(1);  
		container.add(panelLists, flex);
		
		campusCB = new CampusComboBox();
		formPanel.add(campusCB, formData);
		
		FormButtonBinding binding = new FormButtonBinding(formPanel);
		binding.addButton(simpleModal.getSalvarBt());
		
		simpleModal.setFocusWidget(campusCB);
	}
	
	private LayoutContainer getAtualizaProfessoresButtonsPanel() {
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
	
	public boolean isValid() {
		return formPanel.isValid();
	}
	
	@Override
	public Button getSalvarButton() {
		return simpleModal.getSalvarBt();
	}

	@Override
	public CampusComboBox getCampusComboBox() {
		return campusCB;
	}
	
	@Override
	public SimpleModal getSimpleModal() {
		return simpleModal;
	}

	@Override
	public ListView<ProfessorDTO> getProfessorNaoAssociadoList() {
		return professorNaoAssociadoList;
	}

	@Override
	public ListView<ProfessorDTO> getProfessorAssociadoList() {
		return professorAssociadoList;
	}

	@Override
	public ProfessorCampusDTO getProfessorCampusDTO() {
		return professorCampusDTO;
	}


}
