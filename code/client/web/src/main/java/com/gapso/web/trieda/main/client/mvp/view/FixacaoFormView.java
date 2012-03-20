package com.gapso.web.trieda.main.client.mvp.view;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.event.ComponentEvent;
import com.extjs.gxt.ui.client.event.Events;
import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.util.Margins;
import com.extjs.gxt.ui.client.util.Padding;
import com.extjs.gxt.ui.client.widget.LayoutContainer;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.VBoxLayout;
import com.extjs.gxt.ui.client.widget.layout.VBoxLayout.VBoxLayoutAlign;
import com.extjs.gxt.ui.client.widget.layout.VBoxLayoutData;
import com.gapso.web.trieda.main.client.mvp.presenter.FixacaoFormPresenter;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.FixacaoDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.DisciplinaComboBox;
import com.gapso.web.trieda.shared.util.view.ProfessorComboBox;
import com.gapso.web.trieda.shared.util.view.SalaComboBox;
import com.gapso.web.trieda.shared.util.view.SemanaLetivaDoCenarioGrid;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.UnidadeComboBox;

public class FixacaoFormView extends MyComposite implements
		FixacaoFormPresenter.Display {

	private SimpleModal simpleModal;
	private LayoutContainer container;
	private FormPanel formPanel;
	private TextField<String> codigoTF;
	private TextField<String> descricaoTF;
	private ProfessorComboBox professorCB;
	private DisciplinaComboBox disciplinaCB;
	private CampusComboBox campusCB;
	private UnidadeComboBox unidadeCB;
	private SalaComboBox salaCB;
	private SemanaLetivaDoCenarioGrid<HorarioDisponivelCenarioDTO> grid;
	private boolean selectDefault;

	private FixacaoDTO fixacaoDTO;
	private ProfessorDTO professorDTO;
	private DisciplinaDTO disciplinaDTO;
	private CampusDTO campusDTO;
	private UnidadeDTO unidadeDTO;
	private SalaDTO salaDTO;
	private List<HorarioDisponivelCenarioDTO> listHorarios;

	public FixacaoFormView(FixacaoDTO fixacaoDTO, ProfessorDTO professorDTO,
			DisciplinaDTO disciplinaDTO, CampusDTO campusDTO,
			UnidadeDTO unidadeDTO, SalaDTO salaDTO,
			List<HorarioDisponivelCenarioDTO> listHorarios,
			Boolean selectDefault) {
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
		codigoTF.setAllowBlank(false);
		codigoTF.setMinLength(1);
		codigoTF.setMaxLength(50);
		codigoTF.setEmptyText("Preencha o código");
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

		professorCB = new ProfessorComboBox();
		professorCB.setValue(professorDTO);
		formPanel.add(professorCB, formData);

		disciplinaCB = new DisciplinaComboBox();
		disciplinaCB.setValue(disciplinaDTO);
		formPanel.add(disciplinaCB, formData);

		campusCB = new CampusComboBox();
		campusCB.setValue(campusDTO);
		formPanel.add(campusCB, formData);

		unidadeCB = new UnidadeComboBox(campusCB);
		unidadeCB.setValue(unidadeDTO);
		formPanel.add(unidadeCB, formData);

		salaCB = new SalaComboBox(unidadeCB);
		salaCB.setValue(salaDTO);
		formPanel.add(salaCB, formData);

		grid = new SemanaLetivaDoCenarioGrid<HorarioDisponivelCenarioDTO>(
				listHorarios, HorarioDisponivelCenarioDTO.PROPERTY_ID);
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

		simpleModal.setFocusWidget(codigoTF);
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
	public ProfessorComboBox getProfessorComboBox() {
		return professorCB;
	}

	@Override
	public DisciplinaComboBox getDisciplinaComboBox() {
		return disciplinaCB;
	}

	@Override
	public CampusComboBox getCampusComboBox() {
		return campusCB;
	}

	@Override
	public UnidadeComboBox getUnidadeComboBox() {
		return unidadeCB;
	}

	@Override
	public SalaComboBox getSalaComboBox() {
		return salaCB;
	}

	@Override
	public SemanaLetivaDoCenarioGrid<HorarioDisponivelCenarioDTO> getGrid() {
		return grid;
	}

}
