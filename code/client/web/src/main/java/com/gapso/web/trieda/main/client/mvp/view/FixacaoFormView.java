package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.extjs.gxt.ui.client.store.ListStore;
import com.extjs.gxt.ui.client.store.StoreEvent;
import com.extjs.gxt.ui.client.event.ComponentEvent;
import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.util.Padding;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.CheckBoxGroup;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.VBoxLayout;
import com.extjs.gxt.ui.client.widget.layout.VBoxLayout.VBoxLayoutAlign;
import com.extjs.gxt.ui.client.widget.layout.VBoxLayoutData;
import com.gapso.web.trieda.main.client.mvp.presenter.FixacaoFormPresenter;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.FixacaoDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.TurmaDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.OtimizacaoDisciplinasComboBox;
import com.gapso.web.trieda.shared.util.view.OtimizacaoProfessorComboBox;
import com.gapso.web.trieda.shared.util.view.OtimizacaoTurmaComboBox;
import com.gapso.web.trieda.shared.util.view.OtimizacaoSalasComboBox;
import com.gapso.web.trieda.shared.util.view.SemanaLetivaDoCenarioGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.OtimizacaoUnidadeComboBox;
import com.gapso.web.trieda.shared.util.view.OtimizacaoCampusComboBox;
import com.extjs.gxt.ui.client.event.SelectionChangedEvent;
import com.extjs.gxt.ui.client.event.SelectionChangedListener;


public class FixacaoFormView extends MyComposite implements
		FixacaoFormPresenter.Display {

	private SimpleModal simpleModal;
	private LayoutContainer container;
	private FormPanel formPanel;
	private TextField<String> codigoTF;
	private TextField<String> descricaoTF;
	private OtimizacaoProfessorComboBox professorCB;
	private OtimizacaoDisciplinasComboBox disciplinaCB;
	private OtimizacaoTurmaComboBox turmaCB;
	private OtimizacaoCampusComboBox campusCB;
	private OtimizacaoUnidadeComboBox unidadeCB;
	private OtimizacaoSalasComboBox salaCB;
	private CheckBox diasEHorariosCB;
	private CheckBox ambienteCB;
	private SemanaLetivaDoCenarioGrid<HorarioDisponivelCenarioDTO> grid;
	
	private boolean selectDefault;
	private CenarioDTO cenarioDTO;
	private FixacaoDTO fixacaoDTO;
	private ProfessorDTO professorDTO;
	private DisciplinaDTO disciplinaDTO;
	private CampusDTO campusDTO;
	private UnidadeDTO unidadeDTO;
	private SalaDTO salaDTO;
	private List<HorarioDisponivelCenarioDTO> listHorarios;
	private AtendimentoOperacionalDTO turma;
	
	

	public FixacaoFormView(CenarioDTO cenarioDTO, FixacaoDTO fixacaoDTO,
			ProfessorDTO professorDTO, DisciplinaDTO disciplinaDTO,
			CampusDTO campusDTO, UnidadeDTO unidadeDTO, SalaDTO salaDTO,
			List<HorarioDisponivelCenarioDTO> listHorarios,	Boolean selectDefault) {
		this.cenarioDTO = cenarioDTO;
		this.fixacaoDTO = fixacaoDTO;
		this.professorDTO = professorDTO;
		this.disciplinaDTO = disciplinaDTO;
		this.campusDTO = campusDTO;
		this.unidadeDTO = unidadeDTO;
		this.salaDTO = salaDTO;
		this.listHorarios = (listHorarios == null) ? new ArrayList<HorarioDisponivelCenarioDTO>()
				: listHorarios;
		this.selectDefault = selectDefault;
		initUI();
	}

	private void initUI() {
		String title = (fixacaoDTO.getId() == null) ? "Inserção de Fixação"
				: "Edição de Fixação";
		simpleModal = new SimpleModal(title, Resources.DEFAULTS.fixacao16());
		simpleModal.setHeight(500);
		simpleModal.setWidth(617);
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

		codigoTF = new TextField<String>();
		codigoTF.setName(FixacaoDTO.PROPERTY_CODIGO);
		codigoTF.setValue(fixacaoDTO.getCodigo());
		codigoTF.setFieldLabel("Código");
		codigoTF.setAllowBlank(true);
		codigoTF.setMinLength(1);
		codigoTF.setMaxLength(50);
		codigoTF.setEmptyText("Preencha o código");
		codigoTF.hide();
		formPanel.add(codigoTF, formData);

		descricaoTF = new TextField<String>();
		descricaoTF.setName(FixacaoDTO.PROPERTY_DESCRICAO);
		descricaoTF.setValue(fixacaoDTO.getDescricao());
		descricaoTF.setFieldLabel("Descrição");
		descricaoTF.setAllowBlank(false);
		descricaoTF.setMinLength(1);
		descricaoTF.setMaxLength(50);
		descricaoTF.setEmptyText("Preencha a descrição");
		formPanel.add(descricaoTF, formData);

		
		professorCB = new OtimizacaoProfessorComboBox(cenarioDTO);
		professorCB.setValue(professorDTO);
		formPanel.add(professorCB, formData);

		
		disciplinaCB = new OtimizacaoDisciplinasComboBox(professorCB);
		disciplinaCB.setValue(disciplinaDTO);
		formPanel.add(disciplinaCB, formData);
		
		AtendimentoOperacionalDTO atendimento = new AtendimentoOperacionalDTO();
		atendimento.setTurma(fixacaoDTO.getTurmaString());
		turmaCB = new OtimizacaoTurmaComboBox(disciplinaCB);
		turmaCB.setValue(atendimento);
		formPanel.add(turmaCB, formData);
		
		
		campusCB = new OtimizacaoCampusComboBox(professorCB);
		campusCB.setValue(campusDTO);
		formPanel.add(campusCB, formData);

		
		unidadeCB = new OtimizacaoUnidadeComboBox(campusCB);
		unidadeCB.setValue(unidadeDTO);
		formPanel.add(unidadeCB, formData);

		
		salaCB = new OtimizacaoSalasComboBox(turmaCB, unidadeCB);
		salaCB.setValue(salaDTO);
		formPanel.add(salaCB, formData);
		
		
		diasEHorariosCB = new CheckBox();
		diasEHorariosCB.setBoxLabel("Dia e Horários");
		diasEHorariosCB.setName(fixacaoDTO.PROPERTY_FIXA_DIAS_HORARIOS);	
		diasEHorariosCB.setValue(this.fixacaoDTO.getFixaDiaEHorario());
		
	
		ambienteCB = new CheckBox();
		ambienteCB.setBoxLabel("Ambiente");
		ambienteCB.setName(fixacaoDTO.PROPERTY_FIXA_AMBIENTE);
		ambienteCB.setValue(this.fixacaoDTO.getFixaAmbiente());
					
		
		CheckBoxGroup checkGroup = new CheckBoxGroup();
		checkGroup.setFieldLabel("Fixar");
	    checkGroup.add(diasEHorariosCB);
	    checkGroup.add(ambienteCB);
				
		formPanel.add(checkGroup, formData);
		
		
		grid = new SemanaLetivaDoCenarioGrid<HorarioDisponivelCenarioDTO>(
				listHorarios, HorarioDisponivelCenarioDTO.PROPERTY_ID, this);
		grid.setSelectDefault(selectDefault);
		
		grid.addListener(Events.Render, new Listener<ComponentEvent>(){
			public void handleEvent(ComponentEvent be){
				grid.unCheckAllHeaders();
			}
		});
		container.add(formPanel, new VBoxLayoutData(new Margins(0, 0, 5, 0)));
		VBoxLayoutData flex = new VBoxLayoutData(new Margins(0));
		flex.setFlex(1);
		container.add(grid, flex);

		FormButtonBinding binding = new FormButtonBinding(formPanel);
		binding.addButton(simpleModal.getSalvarBt());

		simpleModal.setFocusWidget(descricaoTF);
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
	public FixacaoDTO getFixacaoDTO() {
		return fixacaoDTO;
	}

	@Override
	public TextField<String> getCodigoTextField() {
		return codigoTF;
	}

	@Override
	public TextField<String> getDescricaoTextField() {
		return descricaoTF;
	}

	@Override
	public OtimizacaoProfessorComboBox getProfessorComboBox() {
		return professorCB;
	}

	@Override
	public OtimizacaoDisciplinasComboBox getDisciplinaComboBox() {
		return disciplinaCB;
	}
	
	@Override
	public  OtimizacaoTurmaComboBox getTurmaComboBox() {
		return turmaCB;
	}

	@Override
	public OtimizacaoCampusComboBox getCampusComboBox() {
		return campusCB;
	}

	@Override
	public OtimizacaoUnidadeComboBox getUnidadeComboBox() {
		return unidadeCB;
	}

	@Override
	public OtimizacaoSalasComboBox getSalaComboBox() {
		return salaCB;
	}
	
	@Override
	public CheckBox getDiasEHorarios() {
		return diasEHorariosCB;
	}
	
	@Override
	public CheckBox getAmbiente() {
		return ambienteCB;
	}

	@Override
	public SemanaLetivaDoCenarioGrid<HorarioDisponivelCenarioDTO> getGrid() {
		return grid;
	}
}
