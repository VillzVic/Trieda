package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.Style.Orientation;
import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.store.ListStore;
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
import com.gapso.web.trieda.main.client.mvp.presenter.EquivalenciaFormPresenter;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.EquivalenciaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CursoComboBox;
import com.gapso.web.trieda.shared.util.view.DisciplinaComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class EquivalenciaFormView extends MyComposite implements EquivalenciaFormPresenter.Display {

	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private CursoComboBox cursoComboBox;
	private DisciplinaComboBox disciplinaComboBox;
	private ListView<CursoDTO> cursosList;
	private ListView<DisciplinaDTO> disciplinasNaoPertencesList;
	private ListView<DisciplinaDTO> disciplinasPertencesList;
	private Button atualizaDisciplinasDoCursoBT;
	private Button adicionaDisciplinaBT;
	private Button removeDisciplinaBT;
	private EquivalenciaDTO equivalenciaDTO;
	
	public EquivalenciaFormView(EquivalenciaDTO equivalenciaDTO) {
		this.equivalenciaDTO = equivalenciaDTO;
		initUI();
		// TODO
//		initComponent(simpleModal);
//		setParent(null);
	}
	
	private void initUI() {
		String title = "Inserção de Equivalências";
		simpleModal = new SimpleModal(title, Resources.DEFAULTS.equivalencia16());
		simpleModal.setHeight(470);
		simpleModal.setWidth(600);
		createForm();
		simpleModal.setContent(formPanel);
	}

	private void createForm() {
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);
		
		cursoComboBox = new CursoComboBox();
		formPanel.add(cursoComboBox, formData);
		
		disciplinaComboBox = new DisciplinaComboBox();
		disciplinaComboBox.setAllowBlank(false);
		formPanel.add(disciplinaComboBox, formData);
		
		LayoutContainer panelLists = new LayoutContainer();
        HBoxLayout layout = new HBoxLayout();  
        layout.setPadding(new Padding(5));  
        layout.setHBoxLayoutAlign(HBoxLayoutAlign.TOP);  
        panelLists.setLayout(layout);  
		panelLists.setHeight(330);
		
		ContentPanel cursoListPanel = new ContentPanel(new FitLayout());
		cursoListPanel.setWidth(95);
		cursoListPanel.setHeight(320);
		cursoListPanel.setHeading("Curso(s)");
		ListStore<CursoDTO> store1 = new ListStore<CursoDTO>();
		store1.setDefaultSort(CursoDTO.PROPERTY_CODIGO, SortDir.ASC);
		cursosList = new ListView<CursoDTO>(store1);
		cursosList.setDisplayProperty(CursoDTO.PROPERTY_CODIGO);
		cursoListPanel.add(cursosList);
		
		ContentPanel naoAssociadasListPanel = new ContentPanel(new FitLayout());
		naoAssociadasListPanel.setWidth(190);
		naoAssociadasListPanel.setHeight(320);
		naoAssociadasListPanel.setHeading("Disciplina(s) não associadas a Equivalência");
		ListStore<DisciplinaDTO> store2 = new ListStore<DisciplinaDTO>();
		store2.setDefaultSort(DisciplinaDTO.PROPERTY_CODIGO, SortDir.ASC);
		disciplinasNaoPertencesList = new ListView<DisciplinaDTO>(store2);
		disciplinasNaoPertencesList.setDisplayProperty(DisciplinaDTO.PROPERTY_CODIGO);
		naoAssociadasListPanel.add(disciplinasNaoPertencesList);
		
		ContentPanel associadasListPanel = new ContentPanel(new FitLayout());
		associadasListPanel.setWidth(190);
		associadasListPanel.setHeight(320);
		associadasListPanel.setHeading("Disciplina(s) associadas a Equivalência");
		ListStore<DisciplinaDTO> store3 = new ListStore<DisciplinaDTO>();
		store3.setDefaultSort(DisciplinaDTO.PROPERTY_CODIGO, SortDir.ASC);
		disciplinasPertencesList = new ListView<DisciplinaDTO>(store3);
		disciplinasPertencesList.setDisplayProperty(DisciplinaDTO.PROPERTY_CODIGO);
		associadasListPanel.add(disciplinasPertencesList);
		
		panelLists.add(cursoListPanel, new HBoxLayoutData(new Margins(0, 0, 0, 0)));
		panelLists.add(getAtualizaSalasDoAndarButtonsPanel(), new HBoxLayoutData(new Margins(0, 5, 0, 5)));
		panelLists.add(naoAssociadasListPanel, new HBoxLayoutData(new Margins(0, 0, 0, 0)));
		panelLists.add(getAtualizaSalasButtonsPanel(), new HBoxLayoutData(new Margins(0, 5, 0, 5)));
		panelLists.add(associadasListPanel, new HBoxLayoutData(new Margins(0, 0, 0, 0)));
		
		formPanel.add(panelLists);
		
		FormButtonBinding binding = new FormButtonBinding(formPanel);
		binding.addButton(simpleModal.getSalvarBt());
		
		simpleModal.setFocusWidget(cursoComboBox);
	}
	
	private LayoutContainer getAtualizaSalasDoAndarButtonsPanel() {
		LayoutContainer panel = new LayoutContainer();
		panel.setLayout(new RowLayout(Orientation.VERTICAL));
		
		atualizaDisciplinasDoCursoBT = new Button();
		atualizaDisciplinasDoCursoBT.setSize(30, 50);
		atualizaDisciplinasDoCursoBT.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.toRight24()));
				
		RowData rowData = new RowData(-1, -1, new Margins(4, 0, 4, 0));
		
		panel.add(atualizaDisciplinasDoCursoBT, rowData);
		panel.setWidth(30);
		panel.setHeight(320);
		return panel;
	}
	
	private LayoutContainer getAtualizaSalasButtonsPanel() {
		LayoutContainer panel = new LayoutContainer();
		panel.setLayout(new RowLayout(Orientation.VERTICAL));
		
		adicionaDisciplinaBT = new Button();
		adicionaDisciplinaBT.setSize(30, 50);
		adicionaDisciplinaBT.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.toRight24()));
		
		removeDisciplinaBT = new Button();
		removeDisciplinaBT.setSize(30, 50);
		removeDisciplinaBT.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.toLeft24()));
		
		RowData rowData = new RowData(-1, -1, new Margins(4, 0, 4, 0));
		
		panel.add(adicionaDisciplinaBT, rowData);
		panel.add(removeDisciplinaBT, rowData);
		panel.setWidth(30);
		panel.setHeight(320);
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
	public SimpleModal getSimpleModal() {
		return simpleModal;
	}

	@Override
	public CursoComboBox getCursoComboBox() {
		return cursoComboBox;
	}

	@Override
	public DisciplinaComboBox getDisciplinaComboBox() {
		return disciplinaComboBox;
	}

	@Override
	public ListView<CursoDTO> getCursosList() {
		return cursosList;
	}

	@Override
	public ListView<DisciplinaDTO> getDisciplinasNaoPertencesList() {
		return disciplinasNaoPertencesList;
	}

	@Override
	public ListView<DisciplinaDTO> getDisciplinasPertencesList() {
		return disciplinasPertencesList;
	}

	@Override
	public EquivalenciaDTO getEquivalenciaDTO() {
		return equivalenciaDTO;
	}
 
	@Override
	public Button getAtualizaDisciplinasDoCursoBT() {
		return atualizaDisciplinasDoCursoBT;
	}

	@Override
	public Button getAdicionaDisciplinasBT() {
		return adicionaDisciplinaBT;
	}

	@Override
	public Button getRemoveDisciplinasBT() {
		return removeDisciplinaBT;
	}

	

}
