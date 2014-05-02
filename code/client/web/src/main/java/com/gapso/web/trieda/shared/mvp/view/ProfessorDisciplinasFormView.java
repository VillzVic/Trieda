package com.gapso.web.trieda.shared.mvp.view;

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
import com.extjs.gxt.ui.client.widget.layout.HBoxLayout;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayoutData;
import com.extjs.gxt.ui.client.widget.layout.RowData;
import com.extjs.gxt.ui.client.widget.layout.RowLayout;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayout.HBoxLayoutAlign;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.mvp.presenter.ProfessorDisciplinasFormPresenter;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.ProfessorComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class ProfessorDisciplinasFormView 
	extends MyComposite implements ProfessorDisciplinasFormPresenter.Display {

	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private LayoutContainer panelLists;
	
	private ListView<DisciplinaDTO> naoSelecionadoList;
	private ListView<DisciplinaDTO> selecionadoList;
	
	private Button adicionaBT;
	private Button removeBT;
	
	private ProfessorComboBox professorCB;

	private UsuarioDTO usuario;
	private CenarioDTO cenarioDTO;
	
	public ProfessorDisciplinasFormView( CenarioDTO cenarioDTO, 
		UsuarioDTO usuario)
	{
		this.usuario = usuario;
		this.cenarioDTO = cenarioDTO;
		initUI();
	}

	private void initUI()
	{
		String title = "Adição de Habilitações em Massa";

		simpleModal = new SimpleModal( title, Resources.DEFAULTS.professor16() );
		simpleModal.setHeight( 400 );
		simpleModal.setWidth( 500 );
		createForm();
		simpleModal.setContent( formPanel );
	}

	private void createForm()
	{
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);

		professorCB = new ProfessorComboBox(cenarioDTO, usuario.isProfessor());
		professorCB.setAllowBlank(false);
		formPanel.add(professorCB, formData);
		
		HBoxLayout layout2 = new HBoxLayout();  
		layout2.setHBoxLayoutAlign(HBoxLayoutAlign.MIDDLE);  
		layout2.setPadding(new Padding(5));  
		panelLists = new LayoutContainer(layout2);

		ContentPanel naoSelecionadoListPanel = new ContentPanel(new FitLayout());
		naoSelecionadoListPanel.setHeadingHtml("Disciplinas não selecionadas");
		naoSelecionadoListPanel.setWidth(190);
		naoSelecionadoListPanel.setHeight(260);
		naoSelecionadoList = new ListView<DisciplinaDTO>();
		naoSelecionadoList.setDisplayProperty(DisciplinaDTO.PROPERTY_DISPLAY_TEXT);
		naoSelecionadoList.getSelectionModel().setSelectionMode(SelectionMode.MULTI);
		naoSelecionadoListPanel.add(naoSelecionadoList);

		ContentPanel selecionadoListPanel = new ContentPanel(new FitLayout());
		selecionadoListPanel.setHeadingHtml("Disciplinas selecionadas");
		selecionadoListPanel.setWidth(190);
		selecionadoListPanel.setHeight(260);
		selecionadoList = new ListView<DisciplinaDTO>();
		selecionadoList.setDisplayProperty(DisciplinaDTO.PROPERTY_DISPLAY_TEXT);
		selecionadoList.getSelectionModel().setSelectionMode(SelectionMode.MULTI);
		selecionadoListPanel.add(selecionadoList);

		panelLists.add(naoSelecionadoListPanel,new HBoxLayoutData(new Margins(5)));
		panelLists.add(getAtualizaSalasButtonsPanel(),new HBoxLayoutData(new Margins(5)));
		panelLists.add(selecionadoListPanel,new HBoxLayoutData(new Margins(5)));

		BorderLayoutData bld = new BorderLayoutData(LayoutRegion.CENTER);
		bld.setMargins(new Margins(0, 0, 0, 0));
		formPanel.setBodyBorder(false);
		formPanel.add(panelLists, bld);

		FormButtonBinding binding = new FormButtonBinding(formPanel);
		binding.addButton(simpleModal.getSalvarBt());

		simpleModal.setFocusWidget(professorCB);
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

	public boolean isValid()
	{
		return formPanel.isValid();
	}

	@Override
	public Button getSalvarButton()
	{
		return simpleModal.getSalvarBt();
	}

	@Override
	public SimpleModal getSimpleModal()
	{
		return simpleModal;
	}

	@Override
	public ProfessorComboBox getProfessorComboBox()
	{
		return professorCB;
	}

	@Override
	public ListView< DisciplinaDTO > getNaoSelecionadoList()
	{
		return naoSelecionadoList;
	}

	@Override
	public ListView< DisciplinaDTO > getSelecionadoList()
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
}
