package com.gapso.web.trieda.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.CurriculoDTO;
import com.gapso.web.trieda.client.mvp.model.CursoDTO;
import com.gapso.web.trieda.client.mvp.model.DemandaDTO;
import com.gapso.web.trieda.client.mvp.model.DisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.gapso.web.trieda.client.mvp.presenter.DemandaFormPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.CampusComboBox;
import com.gapso.web.trieda.client.util.view.CurriculoComboBox;
import com.gapso.web.trieda.client.util.view.CursoComboBox;
import com.gapso.web.trieda.client.util.view.DisciplinaComboBox;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.gapso.web.trieda.client.util.view.TurnoComboBox;

public class DemandaFormView extends MyComposite implements DemandaFormPresenter.Display {

	private SimpleModal simpleModal;
	private FormPanel formPanel;

	private CampusComboBox campusCB;
	private CursoComboBox cursoCB;
	private CurriculoComboBox curriculoCB;
	private TurnoComboBox turnoCB;
	private DisciplinaComboBox disciplinaCB;
	private NumberField demandaTF;
	
	private DemandaDTO demandaDTO;
	private CampusDTO campusDTO;
	private CursoDTO cursoDTO;
	private CurriculoDTO curriculoDTO;
	private TurnoDTO turnoDTO;
	private DisciplinaDTO disciplinaDTO;
	
	public DemandaFormView(DemandaDTO demandaDTO, CampusDTO campusDTO, CursoDTO cursoDTO, CurriculoDTO curriculoDTO, TurnoDTO turnoDTO, DisciplinaDTO disciplinaDTO) {
		this.demandaDTO = demandaDTO;
		this.campusDTO = campusDTO;
		this.cursoDTO = cursoDTO;
		this.curriculoDTO = curriculoDTO;
		this.turnoDTO = turnoDTO;
		this.disciplinaDTO = disciplinaDTO;
		initUI();
		// TODO
//		initComponent(simpleModal);
//		setParent(null);
	}
	
	private void initUI() {
		String title = (demandaDTO.getId() == null)? "Inserção de Demanda" : "Edição de Demanda";
		simpleModal = new SimpleModal(title, Resources.DEFAULTS.demanda16());
		simpleModal.setHeight(280);
		createForm();
		simpleModal.setContent(formPanel);
	}

	private void createForm() {
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);
		
		campusCB = new CampusComboBox();
		campusCB.setAllowBlank(false);
		campusCB.setValue(campusDTO);
		campusCB.setEmptyText("Selecione o campus");
		formPanel.add(campusCB, formData);
		
		cursoCB = new CursoComboBox();
		cursoCB.setAllowBlank(false);
		cursoCB.setValue(cursoDTO);
		cursoCB.setEmptyText("Selecione o curso");
		formPanel.add(cursoCB, formData);
		
		curriculoCB = new CurriculoComboBox();
		curriculoCB.setAllowBlank(false);
		curriculoCB.setValue(curriculoDTO);
		curriculoCB.setEmptyText("Selecione o curriculo");
		formPanel.add(curriculoCB, formData);
		
		turnoCB = new TurnoComboBox();
		turnoCB.setAllowBlank(false);
		turnoCB.setValue(turnoDTO);
		turnoCB.setEmptyText("Selecione o turno");
		formPanel.add(turnoCB, formData);
		
		disciplinaCB = new DisciplinaComboBox();
		disciplinaCB.setAllowBlank(false);
		disciplinaCB.setValue(disciplinaDTO);
		disciplinaCB.setEmptyText("Selecione a disciplina");
		formPanel.add(disciplinaCB, formData);

		demandaTF = new NumberField();
		demandaTF.setValue(demandaDTO.getDemanda());
		demandaTF.setFieldLabel("Quantidade");
		demandaTF.setAllowBlank(false);
		demandaTF.setAllowNegative(false);
		demandaTF.setAllowDecimals(false);
		demandaTF.setEmptyText("Selecione a quantidade");
		formPanel.add(demandaTF, formData);
		
		FormButtonBinding binding = new FormButtonBinding(formPanel);
		binding.addButton(simpleModal.getSalvarBt());
		
		simpleModal.setFocusWidget(campusCB);
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
	public DemandaDTO getDemandaDTO() {
		return demandaDTO;
	}

	@Override
	public CampusComboBox getCampusComboBox() {
		return campusCB;
	}

	@Override
	public CursoComboBox getCursoComboBox() {
		return cursoCB;
	}

	@Override
	public CurriculoComboBox getCurriculoComboBox() {
		return curriculoCB;
	}

	@Override
	public TurnoComboBox getTurnoComboBox() {
		return turnoCB;
	}

	@Override
	public DisciplinaComboBox getDisciplinaComboBox() {
		return disciplinaCB;
	}

	@Override
	public NumberField getDemandaTextField() {
		return demandaTF;
	}

}
