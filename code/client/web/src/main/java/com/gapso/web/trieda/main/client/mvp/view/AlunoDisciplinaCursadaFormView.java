package com.gapso.web.trieda.main.client.mvp.view;

import java.util.Arrays;
import java.util.List;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.store.Store;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.util.Padding;
import com.extjs.gxt.ui.client.widget.CheckBoxListView;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.StoreFilterField;
import com.extjs.gxt.ui.client.widget.layout.FitLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayout;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayoutData;
import com.extjs.gxt.ui.client.widget.layout.HBoxLayout.HBoxLayoutAlign;
import com.extjs.gxt.ui.client.widget.toolbar.LabelToolItem;
import com.extjs.gxt.ui.client.widget.toolbar.ToolBar;
import com.gapso.web.trieda.main.client.mvp.presenter.AlunoDisciplinaCursadaFormPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.AlunosComboBox;
import com.gapso.web.trieda.shared.util.view.CurriculoComboBox;
import com.gapso.web.trieda.shared.util.view.DisciplinaAutoCompleteBox;
import com.gapso.web.trieda.shared.util.view.SimpleModal;

public class AlunoDisciplinaCursadaFormView
	extends MyComposite
	implements AlunoDisciplinaCursadaFormPresenter.Display
{
	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private CurriculoComboBox curriculoCB;
	private AlunosComboBox alunoCB;
	private DisciplinaAutoCompleteBox disciplinaPreRequisitoCB;
	private CheckBoxListView<CurriculoDisciplinaDTO> disciplinasList;
	private ContentPanel disciplinasListPanel;
	private CenarioDTO cenarioDTO;
	
	public AlunoDisciplinaCursadaFormView( CenarioDTO cenarioDTO )
	{
		this.cenarioDTO = cenarioDTO;
	
		initUI();
	}
	
	private void initUI() {
		String title = "Inserção de Disciplinas Cursadas";
		simpleModal = new SimpleModal(title, Resources.DEFAULTS.disciplina16());
		simpleModal.setHeight(400);
		simpleModal.setWidth(440);
		createForm();
		simpleModal.setContent(formPanel);
	}
	
	private void createForm() {
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);
		
		alunoCB = new AlunosComboBox(cenarioDTO);
		alunoCB.setAllowBlank(false);
		formPanel.add(alunoCB, formData);
		
		curriculoCB = new CurriculoComboBox(cenarioDTO);
		curriculoCB.setName(DisciplinaDTO.PROPERTY_USA_DOMINGO);
		curriculoCB.setFieldLabel("Matriz Curricular");
		curriculoCB.setAllowBlank(false);
		formPanel.add(curriculoCB, formData);
		
		LayoutContainer panelLists = new LayoutContainer();
		HBoxLayout layout = new HBoxLayout();
		layout.setPadding(new Padding(5));
		layout.setHBoxLayoutAlign(HBoxLayoutAlign.TOP);
		panelLists.setLayout(layout);
		panelLists.setHeight(270);	
		
		disciplinasListPanel = new ContentPanel(new FitLayout());
		disciplinasListPanel.setWidth(380);
		disciplinasListPanel.setHeight(240);
		disciplinasListPanel.setHeadingHtml("Disciplinas não cursadas");
		disciplinasListPanel.setEnabled(false);
		ListStore<CurriculoDisciplinaDTO> store1 = new ListStore<CurriculoDisciplinaDTO>();
		store1.setDefaultSort(CurriculoDisciplinaDTO.PROPERTY_DISCIPLINA_STRING, SortDir.ASC);
		disciplinasList = new CheckBoxListView<CurriculoDisciplinaDTO>();
		disciplinasList.setStore(store1);
		disciplinasList.setDisplayProperty(CurriculoDisciplinaDTO.PROPERTY_DISCIPLINA_STRING + "} - Período: {" + 
				CurriculoDisciplinaDTO.PROPERTY_PERIODO);
		disciplinasListPanel.add(disciplinasList);
		
		ToolBar filter = new ToolBar();
		filter.add(new LabelToolItem("Filtro Período(s):"));
		
		StoreFilterField<CurriculoDisciplinaDTO> field = new StoreFilterField<CurriculoDisciplinaDTO>() {

			@Override
			protected boolean doSelect(Store<CurriculoDisciplinaDTO> store, CurriculoDisciplinaDTO parent,
					CurriculoDisciplinaDTO record, String property, String filter) {
				List<String> periodos = Arrays.asList(filter.split("[,\\s]\\s*"));
				if(periodos.contains(record.getPeriodo().toString())){
					return true;
				}
				return false;
			}
			
			@Override  
		    protected void onFilter() {  
				super.onFilter();  
				disciplinasList.getSelectionModel().select(0, false);  
		    }  
			
		};
		field.setWidth(160);
		field.setEmptyText("Digite um ou mais periodos");
		filter.add(field);
		disciplinasListPanel.setTopComponent(filter);
		disciplinasListPanel.setHeadingHtml("Disciplinas Não Cursadas");
		field.bind(store1);

		panelLists.add(disciplinasListPanel, new HBoxLayoutData(new Margins(0, 0, 0,
				0)));
		formPanel.add(panelLists);
	
		FormButtonBinding binding = new FormButtonBinding(formPanel);
		binding.addButton(simpleModal.getSalvarBt());
	
		simpleModal.setFocusWidget(curriculoCB);
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
	public CurriculoComboBox getCurriculoComboBox() {
		return curriculoCB;
	}
	
	@Override
	public AlunosComboBox getAlunoComboBox() {
		return alunoCB;
	}
	
	@Override
	public DisciplinaAutoCompleteBox getDisciplinaPreRequisitoComboBox() {
		return disciplinaPreRequisitoCB;
	}
	
	@Override
	public CheckBoxListView<CurriculoDisciplinaDTO> getDisciplinasList() {
		return disciplinasList;
	}
	
	@Override
	public ContentPanel getDisciplinasListPanel() {
		return disciplinasListPanel;
	}
}
