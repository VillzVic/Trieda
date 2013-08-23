package com.gapso.web.trieda.main.client.mvp.view;

import java.util.List;

import com.extjs.gxt.ui.client.GXT;
import com.extjs.gxt.ui.client.Style.Orientation;
import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.store.Store;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.util.Padding;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.ListView;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.StoreFilterField;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayout;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayout.HBoxLayoutAlign;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayoutData;
import com.extjs.gxt.ui.client.widget.layout.RowData;
import com.extjs.gxt.ui.client.widget.layout.RowLayout;
import com.extjs.gxt.ui.client.widget.toolbar.LabelToolItem;
import com.extjs.gxt.ui.client.widget.toolbar.ToolBar;
import com.gapso.web.trieda.main.client.mvp.presenter.EquivalenciaFormPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.EquivalenciaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.DisciplinaComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class EquivalenciaFormView extends MyComposite implements
		EquivalenciaFormPresenter.Display {

	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private DisciplinaComboBox disciplinaCursouComboBox;
	private DisciplinaComboBox disciplinaEliminaComboBox;
	private CheckBox equivalenciaGeralCheckBox;
	private ListView<CursoDTO> cursosList;
	private ListView<CursoDTO> cursosPertencesList;
	private Button adicionaCursoBT;
	private Button removeCursoBT;
	private EquivalenciaDTO equivalenciaDTO;
	private DisciplinaDTO disciplinaEliminaDTO;
	private DisciplinaDTO disciplinaCursouDTO;
	private List<CursoDTO> cursosDTO;
	private ContentPanel associadosListPanel;
	private ContentPanel cursosListPanel;
	private CenarioDTO cenarioDTO;

	public EquivalenciaFormView(CenarioDTO cenarioDTO, EquivalenciaDTO equivalenciaDTO) {
		this(cenarioDTO, equivalenciaDTO, null, null, null);
	}
	
	public EquivalenciaFormView(CenarioDTO cenarioDTO, EquivalenciaDTO equivalenciaDTO,
			List<CursoDTO> cursosDTO, DisciplinaDTO disciplinaCursouDTO, DisciplinaDTO disciplinaEliminaDTO) {
		this.equivalenciaDTO = equivalenciaDTO;
		this.disciplinaCursouDTO = disciplinaCursouDTO;
		this.disciplinaEliminaDTO = disciplinaEliminaDTO;
		this.cursosDTO = cursosDTO;
		this.cenarioDTO = cenarioDTO;
		initUI();
	}


	private void initUI() {
		String title = (equivalenciaDTO.getId() == null) ? "Inserção de Equivalências"
				: "Edição de Equivalências";
		simpleModal = new SimpleModal(title,
				Resources.DEFAULTS.equivalencia16());
		simpleModal.setHeight(470);
		simpleModal.setWidth(480);
		createForm();
		simpleModal.setContent(formPanel);
	}

	private void createForm() {
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);

		disciplinaCursouComboBox = new DisciplinaComboBox(cenarioDTO);
		disciplinaCursouComboBox.setAllowBlank(false);
		disciplinaCursouComboBox.setFieldLabel("Disciplina (Cursou)");
		disciplinaCursouComboBox.setValue(disciplinaCursouDTO);
		formPanel.add(disciplinaCursouComboBox, formData);
		
		disciplinaEliminaComboBox = new DisciplinaComboBox(cenarioDTO);
		disciplinaEliminaComboBox.setAllowBlank(false);
		disciplinaEliminaComboBox.setFieldLabel("Disciplina (Elimina)");
		disciplinaEliminaComboBox.setValue(disciplinaEliminaDTO);
		formPanel.add(disciplinaEliminaComboBox, formData);

		equivalenciaGeralCheckBox = new CheckBox();
		equivalenciaGeralCheckBox.setFieldLabel("Equivalencia Geral");
		equivalenciaGeralCheckBox.setValue( equivalenciaDTO.getEquivalenciaGeral() == null
				? true : equivalenciaDTO.getEquivalenciaGeral());
		formPanel.add(equivalenciaGeralCheckBox, formData);
		
		LayoutContainer panelLists = new LayoutContainer();
		HBoxLayout layout = new HBoxLayout();
		layout.setPadding(new Padding(5));
		layout.setHBoxLayoutAlign(HBoxLayoutAlign.TOP);
		panelLists.setLayout(layout);
		panelLists.setHeight(270);	
		
		String aria = GXT.isAriaEnabled() ? " role='option' aria-selected='false' " : "";
		
		cursosListPanel = new ContentPanel(new FitLayout());
		cursosListPanel.setWidth(200);
		cursosListPanel.setHeight(250);
		cursosListPanel.setHeading("Todos os Curso");
		cursosListPanel.setEnabled(!equivalenciaGeralCheckBox.getValue());
		ListStore<CursoDTO> store1 = new ListStore<CursoDTO>();
		store1.setDefaultSort(CursoDTO.PROPERTY_NOME, SortDir.ASC);
		cursosList = new ListView<CursoDTO>(store1);
		cursosList.setTemplate("<tpl for=\".\"><div class='x-view-item' " + aria + ">{" + CursoDTO.PROPERTY_NOME 
				+ "}({" + CursoDTO.PROPERTY_CODIGO + "})" + "</div></tpl>");
		cursosListPanel.add(cursosList);
		
		ToolBar filter = new ToolBar();
		filter.add(new LabelToolItem("Filtro:"));
		
		StoreFilterField<CursoDTO> field = new StoreFilterField<CursoDTO>() {

			@Override
			protected boolean doSelect(Store<CursoDTO> store, CursoDTO parent,
					CursoDTO record, String property, String filter) {
				String name = record.getNome().toLowerCase();
				if (name.indexOf(filter.toLowerCase()) != -1) {
					return true;
				}
				return false;
			}
			
			@Override  
		    protected void onFilter() {  
				super.onFilter();  
				cursosList.getSelectionModel().select(0, false);  
		    }  
			
		};
		field.setWidth(150);
		filter.add(field);
		cursosListPanel.setTopComponent(filter);
		field.bind(store1);
		
		associadosListPanel = new ContentPanel(new FitLayout());
		associadosListPanel.setWidth(200);
		associadosListPanel.setHeight(250);
		associadosListPanel
				.setHeading("Curso(s) associados a Equivalência");
		associadosListPanel.setEnabled(!equivalenciaGeralCheckBox.getValue());
		ListStore<CursoDTO> store3 = new ListStore<CursoDTO>();
		if (cursosDTO != null )
			store3.add(cursosDTO);
		store3.setDefaultSort(CursoDTO.PROPERTY_CODIGO, SortDir.ASC);
		cursosPertencesList = new ListView<CursoDTO>(store3);
		cursosPertencesList.setTemplate("<tpl for=\".\"><div class='x-view-item' " + aria + ">{" + CursoDTO.PROPERTY_NOME 
				+ "}({" + CursoDTO.PROPERTY_CODIGO + "})" + "</div></tpl>");
		associadosListPanel.add(cursosPertencesList);

		panelLists.add(cursosListPanel, new HBoxLayoutData(new Margins(0, 0, 0,
				0)));
		panelLists.add(getAtualizaSalasButtonsPanel(), new HBoxLayoutData(
				new Margins(0, 5, 0, 5)));
		panelLists.add(associadosListPanel, new HBoxLayoutData(new Margins(0,
				0, 0, 0)));
		formPanel.add(panelLists);
		
		FormButtonBinding binding = new FormButtonBinding(formPanel);
		binding.addButton(simpleModal.getSalvarBt());

		simpleModal.setFocusWidget(disciplinaCursouComboBox);
		
		
	}
	
	private LayoutContainer getAtualizaSalasButtonsPanel() {
		LayoutContainer panel = new LayoutContainer();
		panel.setLayout(new RowLayout(Orientation.VERTICAL));

		adicionaCursoBT = new Button();
		adicionaCursoBT.setSize(30, 50);
		adicionaCursoBT.setIcon(AbstractImagePrototype
				.create(Resources.DEFAULTS.toRight24()));
		adicionaCursoBT.setEnabled(!equivalenciaGeralCheckBox.getValue());

		removeCursoBT = new Button();
		removeCursoBT.setSize(30, 50);
		removeCursoBT.setIcon(AbstractImagePrototype
				.create(Resources.DEFAULTS.toLeft24()));
		removeCursoBT.setEnabled(!equivalenciaGeralCheckBox.getValue());

		RowData rowData = new RowData(-1, -1, new Margins(4, 0, 4, 0));

		panel.add(adicionaCursoBT, rowData);
		panel.add(removeCursoBT, rowData);
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
	public DisciplinaComboBox getDisciplinaCursouComboBox() {
		return disciplinaCursouComboBox;
	}
	
	@Override
	public DisciplinaComboBox getDisciplinaEliminaComboBox() {
		return disciplinaEliminaComboBox;
	}
	
	@Override
	public CheckBox getEquivalenciaGeralCheckBox() {
		return equivalenciaGeralCheckBox;
	}

	@Override
	public EquivalenciaDTO getEquivalenciaDTO() {
		return equivalenciaDTO;
	}
	
	@Override
	public ListView<CursoDTO> getCursosList() {
		return cursosList;
	}
	
	@Override
	public ListView<CursoDTO> getCursosPertencesList() {
		return cursosPertencesList;
	}
	
	@Override
	public Button getAdicionaCursoBT() {
		return adicionaCursoBT;
	}

	@Override
	public Button getRemoveCursoBT() {
		return removeCursoBT;
	}
	
	@Override
	public ContentPanel getAssociadosListPanel() {
		return associadosListPanel;
	}

	@Override
	public ContentPanel getCursosListPanel() {
		return cursosListPanel;
	}
	
}
