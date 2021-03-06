package com.gapso.web.trieda.shared.mvp.view;

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
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayout;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayout.HBoxLayoutAlign;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayoutData;
import com.extjs.gxt.ui.client.widget.layout.RowData;
import com.extjs.gxt.ui.client.widget.layout.RowLayout;
import com.extjs.gxt.ui.client.widget.layout.VBoxLayout;
import com.extjs.gxt.ui.client.widget.layout.VBoxLayout.VBoxLayoutAlign;
import com.extjs.gxt.ui.client.widget.layout.VBoxLayoutData;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.mvp.presenter.CampusProfessorFormPresenter;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class CampusProfessorFormView extends MyComposite
	implements CampusProfessorFormPresenter.Display
{
	private SimpleModal simpleModal;
	private LayoutContainer container;
	private FormPanel formPanel;
	private LayoutContainer panelLists;
	private CampusComboBox campusCB;
	private ListView< ProfessorDTO > professorNaoAssociadoList;
	private ListView< ProfessorDTO > professorAssociadoList;
	private Button adicionaBT;
	private Button removeBT;
	private CampusDTO campusDTO;
	private CenarioDTO cenarioDTO;

	public CampusProfessorFormView( CenarioDTO cenarioDTO, CampusDTO campusDTO )
	{
		this.cenarioDTO = cenarioDTO;
		this.campusDTO = campusDTO;
		initUI();
	}

	private void initUI()
	{
		String title = "Professores no campus";
		simpleModal = new SimpleModal(
			title, Resources.DEFAULTS.professor16() );
		simpleModal.setHeight( 455 );
		simpleModal.setWidth( 595 );
		createForm();
		simpleModal.setContent( container );
	}

	private void createForm()
	{
		container = new LayoutContainer();
		VBoxLayout layout = new VBoxLayout();
		layout.setPadding(new Padding(5));
		layout.setVBoxLayoutAlign(VBoxLayoutAlign.STRETCH);
		container.setLayout(layout);

		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);

		panelLists = new LayoutContainer();
		HBoxLayout layout2 = new HBoxLayout();
		layout2.setHBoxLayoutAlign(HBoxLayoutAlign.TOP);
		layout2.setPadding(new Padding(5));
		panelLists.setLayout(layout2);

		ContentPanel naoVinculadaListPanel = new ContentPanel(new FitLayout());
		naoVinculadaListPanel.setWidth(267);
		naoVinculadaListPanel.setHeight(320);
		naoVinculadaListPanel
				.setHeadingHtml("Professor(es) NÃO associado(s) ao Campus");
		professorNaoAssociadoList = new ListView<ProfessorDTO>();
		professorNaoAssociadoList.disable();
		professorNaoAssociadoList
				.setDisplayProperty(ProfessorDTO.PROPERTY_NOME);
		professorNaoAssociadoList.getSelectionModel().setSelectionMode(
				SelectionMode.MULTI);
		naoVinculadaListPanel.add(professorNaoAssociadoList);

		ContentPanel vinculadaListPanel = new ContentPanel(new FitLayout());
		vinculadaListPanel.setWidth(267);
		vinculadaListPanel.setHeight(320);
		vinculadaListPanel.setHeadingHtml("Professor(es) associado(s) ao Campus");
		professorAssociadoList = new ListView<ProfessorDTO>();
		professorAssociadoList.disable();
		professorAssociadoList.setDisplayProperty(ProfessorDTO.PROPERTY_NOME);
		professorAssociadoList.getSelectionModel().setSelectionMode(
				SelectionMode.MULTI);
		vinculadaListPanel.add(professorAssociadoList);

		panelLists.add(naoVinculadaListPanel,
				new HBoxLayoutData(new Margins(0)));
		panelLists.add(getAtualizaProfessoresButtonsPanel(),
				new HBoxLayoutData(new Margins(0)));
		panelLists.add(vinculadaListPanel, new HBoxLayoutData(new Margins(0)));

		container.add(formPanel, new VBoxLayoutData(new Margins(0, 0, 5, 0)));
		VBoxLayoutData flex = new VBoxLayoutData(new Margins(0));
		flex.setFlex(1);
		container.add(panelLists, flex);

		campusCB = new CampusComboBox(cenarioDTO);
		campusCB.setValue(campusDTO);
		if (campusCB == null)
			campusCB.disable();
		formPanel.add(campusCB, formData);

		FormButtonBinding binding = new FormButtonBinding(formPanel);
		binding.addButton(simpleModal.getSalvarBt());

		simpleModal.setFocusWidget(campusCB);
	}

	private LayoutContainer getAtualizaProfessoresButtonsPanel()
	{
		LayoutContainer panel = new LayoutContainer();
		panel.setLayout(new RowLayout(Orientation.VERTICAL));

		adicionaBT = new Button();
		adicionaBT.setEnabled(false);
		adicionaBT.setSize(30, 50);
		adicionaBT.setIcon( AbstractImagePrototype.create(
			Resources.DEFAULTS.toRight24() ) );

		removeBT = new Button();
		removeBT.setEnabled(false);
		removeBT.setSize(30, 50);
		removeBT.setIcon( AbstractImagePrototype.create(
			Resources.DEFAULTS.toLeft24() ) );

		RowData rowData = new RowData( -1, -1, new Margins( 4, 0, 4, 0 ) );

		panel.add( adicionaBT, rowData );
		panel.add( removeBT, rowData );
		panel.setWidth( 30 );
		panel.setHeight( 320 );
		return panel;
	}

	public boolean isValid()
	{
		return ( campusCB.getValue() != null )
			&& ( professorAssociadoList.getStore().getModels().size() > 0 );
	}

	@Override
	public Button getSalvarButton()
	{
		return simpleModal.getSalvarBt();
	}

	@Override
	public CampusComboBox getCampusComboBox()
	{
		return campusCB;
	}

	@Override
	public SimpleModal getSimpleModal()
	{
		return simpleModal;
	}

	@Override
	public ListView< ProfessorDTO > getProfessorNaoAssociadoList()
	{
		return professorNaoAssociadoList;
	}

	@Override
	public ListView< ProfessorDTO > getProfessorAssociadoList()
	{
		return professorAssociadoList;
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
}
