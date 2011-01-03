package com.gapso.web.trieda.client.mvp.view;

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
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.RowData;
import com.extjs.gxt.ui.client.widget.layout.RowLayout;
import com.gapso.web.trieda.client.mvp.model.CursoDTO;
import com.gapso.web.trieda.client.mvp.model.DisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.EquivalenciaDTO;
import com.gapso.web.trieda.client.mvp.presenter.EquivalenciaFormPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.CursoComboBox;
import com.gapso.web.trieda.client.util.view.DisciplinaComboBox;
import com.gapso.web.trieda.client.util.view.SimpleModal;
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
		String title = "Inserção de Turno";
		simpleModal = new SimpleModal(title, Resources.DEFAULTS.turnos16());
		simpleModal.setHeight(500);
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
		
		ContentPanel panelLists = new ContentPanel(new RowLayout(Orientation.HORIZONTAL));
		panelLists.setHeaderVisible(false);
		panelLists.setHeight(330);
		panelLists.setBodyBorder(false);
		
		ContentPanel cursoListPanel = new ContentPanel(new FitLayout());
		cursoListPanel.setHeading("Curso(s)");
		ListStore<CursoDTO> store1 = new ListStore<CursoDTO>();
		store1.setDefaultSort("codigo", SortDir.ASC);
		cursosList = new ListView<CursoDTO>(store1);
		cursosList.setDisplayProperty("codigo");
		cursoListPanel.add(cursosList);
		
		ContentPanel naoAssociadasListPanel = new ContentPanel(new FitLayout());
		naoAssociadasListPanel.setHeading("Disciplina(s) não associadas a Equivalência");
		ListStore<DisciplinaDTO> store2 = new ListStore<DisciplinaDTO>();
		store2.setDefaultSort("codigo", SortDir.ASC);
		disciplinasNaoPertencesList = new ListView<DisciplinaDTO>(store2);
		disciplinasNaoPertencesList.setDisplayProperty("codigo");
		naoAssociadasListPanel.add(disciplinasNaoPertencesList);
		
		ContentPanel associadasListPanel = new ContentPanel(new FitLayout());
		associadasListPanel.setHeading("Disciplina(s) associadas a Equivalência");
		ListStore<DisciplinaDTO> store3 = new ListStore<DisciplinaDTO>();
		store3.setDefaultSort("codigo", SortDir.ASC);
		disciplinasPertencesList = new ListView<DisciplinaDTO>(store3);
		disciplinasPertencesList.setDisplayProperty("codigo");
		associadasListPanel.add(disciplinasPertencesList);
		
		panelLists.add(cursoListPanel, new RowData(.2, 1, new Margins(0, 0, 0, 0)));
		panelLists.add(getAtualizaSalasDoAndarButtonsPanel(), new RowData(42, 1, new Margins(0, 5, 0, 5)));
		panelLists.add(naoAssociadasListPanel, new RowData(.4, 1, new Margins(0, 0, 0, 0)));
		panelLists.add(getAtualizaSalasButtonsPanel(), new RowData(42, 1, new Margins(0, 5, 0, 5)));
		panelLists.add(associadasListPanel, new RowData(.4, 1, new Margins(0, 0, 0, 0)));
		
		formPanel.add(panelLists);
		
		FormButtonBinding binding = new FormButtonBinding(formPanel);
		binding.addButton(simpleModal.getSalvarBt());
		
		simpleModal.setFocusWidget(cursoComboBox);
	}
	
	private LayoutContainer getAtualizaSalasDoAndarButtonsPanel() {
		ContentPanel panel = new ContentPanel();
		panel.setHeaderVisible(false);
		panel.setBodyBorder(false);
		panel.setBodyStyle("display: table-cell; vertical-align: middle");
		panel.setLayout(new RowLayout(Orientation.VERTICAL));
		
		atualizaDisciplinasDoCursoBT = new Button();
		atualizaDisciplinasDoCursoBT.setSize(30, 50);
		atualizaDisciplinasDoCursoBT.setIcon(AbstractImagePrototype.create(Resources.DEFAULTS.toRight24()));
				
		RowData rowData = new RowData(-1, -1, new Margins(4, 0, 4, 0));
		
		panel.add(atualizaDisciplinasDoCursoBT, rowData);
		
		return panel;
	}
	
	private LayoutContainer getAtualizaSalasButtonsPanel() {
		ContentPanel panel = new ContentPanel();
		panel.setHeaderVisible(false);
		panel.setBodyBorder(false);
		panel.setBodyStyle("display: table-cell; vertical-align: middle");
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