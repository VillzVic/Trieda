package com.gapso.web.trieda.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.client.mvp.model.DisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.ProfessorDTO;
import com.gapso.web.trieda.client.mvp.model.ProfessorDisciplinaDTO;
import com.gapso.web.trieda.client.mvp.presenter.ProfessorDisciplinaFormPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.DisciplinaComboBox;
import com.gapso.web.trieda.client.util.view.ProfessorComboBox;
import com.gapso.web.trieda.client.util.view.SimpleModal;

public class ProfessorDisciplinaFormView extends MyComposite implements ProfessorDisciplinaFormPresenter.Display {

	private SimpleModal simpleModal;
	private FormPanel formPanel;
	
	private ProfessorComboBox professorCB;
	private DisciplinaComboBox disciplinaCB;
	private NumberField preferenciaNF;
	private NumberField notaDesempenhoNF;

	private ProfessorDisciplinaDTO professorDisciplinaDTO;
	private ProfessorDTO professorDTO;
	private DisciplinaDTO disciplinaDTO;
	
	public ProfessorDisciplinaFormView(ProfessorDisciplinaDTO professorDisciplinaDTO, ProfessorDTO professorDTO, DisciplinaDTO disciplinaDTO) {
		this.professorDisciplinaDTO = professorDisciplinaDTO;
		this.professorDTO = professorDTO;
		this.disciplinaDTO = disciplinaDTO;
		initUI();
		// TODO
//		initComponent(simpleModal);
//		setParent(null);
	}
	
	private void initUI() {
		String title = (professorDisciplinaDTO.getId() == null)? "Edição de Habilitação" : "Cadastro de Habilitação do Professore";
		simpleModal = new SimpleModal(title, Resources.DEFAULTS.professor16());
		simpleModal.setHeight(200);
		createForm();
		simpleModal.setContent(formPanel);
	}

	private void createForm() {
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);
		
		professorCB = new ProfessorComboBox();
		professorCB.setAllowBlank(false);
		professorCB.setValue(professorDTO);
		formPanel.add(professorCB, formData);
		
		disciplinaCB = new DisciplinaComboBox();
		disciplinaCB.setAllowBlank(false);
		disciplinaCB.setValue(disciplinaDTO);
		formPanel.add(disciplinaCB, formData);
		
		preferenciaNF = new NumberField();
		preferenciaNF.setName("preferencia");
		preferenciaNF.setValue(professorDisciplinaDTO.getPreferencia());
		preferenciaNF.setFieldLabel("Preferência");
		preferenciaNF.setAllowBlank(false);
		preferenciaNF.setAllowNegative(false);
		preferenciaNF.setAllowDecimals(false);
		preferenciaNF.setMaxValue(10);
		preferenciaNF.setEmptyText("Preencha de 0 à 10");
		formPanel.add(preferenciaNF, formData);
		
		notaDesempenhoNF = new NumberField();
		notaDesempenhoNF.setName("notaDesempenho");
		notaDesempenhoNF.setValue(professorDisciplinaDTO.getNotaDesempenho());
		notaDesempenhoNF.setFieldLabel("Nota de Desempenho");
		notaDesempenhoNF.setAllowBlank(false);
		notaDesempenhoNF.setAllowNegative(false);
		notaDesempenhoNF.setAllowDecimals(false);
		notaDesempenhoNF.setMaxValue(10);
		notaDesempenhoNF.setEmptyText("Preencha de 0 à 10");
		formPanel.add(notaDesempenhoNF, formData);
		
		FormButtonBinding binding = new FormButtonBinding(formPanel);
		binding.addButton(simpleModal.getSalvarBt());
		
		simpleModal.setFocusWidget(professorCB);
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
	public ProfessorComboBox getProfessorComboBox() {
		return professorCB;
	}
	
	@Override
	public DisciplinaComboBox getDisciplinaComboBox() {
		return disciplinaCB;
	}

	@Override
	public NumberField getPreferenciaNumberField() {
		return preferenciaNF;
	}
	
	@Override
	public NumberField getNotaDesempenhoNumberField() {
		return notaDesempenhoNF;
	}
	
	@Override
	public ProfessorDisciplinaDTO getProfessorDisciplinaDTO() {
		return professorDisciplinaDTO;
	}
	

}