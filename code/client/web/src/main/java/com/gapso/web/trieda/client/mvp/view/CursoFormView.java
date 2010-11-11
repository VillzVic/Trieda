package com.gapso.web.trieda.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.client.mvp.model.CursoDTO;
import com.gapso.web.trieda.client.mvp.model.TipoCursoDTO;
import com.gapso.web.trieda.client.mvp.presenter.CursoFormPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.gapso.web.trieda.client.util.view.TipoCursoComboBox;

public class CursoFormView extends MyComposite implements CursoFormPresenter.Display {

	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private TextField<String> nomeTF;
	private TextField<String> codigoTF;
	private TipoCursoComboBox tipoCursoCB;
	private NumberField numMinDoutoresTextField;
	private NumberField numMinMestresTextField;
	private NumberField maxDisciplinasPeloProfessorTextField;
	private CheckBox admMaisDeUmDisciplinaCheckBox;
	private CursoDTO cursoDTO;
	private TipoCursoDTO tipoCursoDTO;
	
	public CursoFormView(CursoDTO cursoDTO, TipoCursoDTO tipoCursoDTO) {
		this.cursoDTO = cursoDTO;
		this.tipoCursoDTO = tipoCursoDTO;
		initUI();
		// TODO
//		initComponent(simpleModal);
//		setParent(null);
	}
	
	private void initUI() {
		String title = (cursoDTO.getId() == null)? "Inserção de Curso" : "Edição de Curso";
		simpleModal = new SimpleModal(title, Resources.DEFAULTS.curso16());
		simpleModal.setHeight(300);
		createForm();
		simpleModal.setContent(formPanel);
	}

	private void createForm() {
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);
		
		nomeTF = new TextField<String>();
		nomeTF.setName("nome");
		nomeTF.setValue(cursoDTO.getNome());
		nomeTF.setFieldLabel("Nome");
		nomeTF.setAllowBlank(false);
		nomeTF.setMinLength(5);
		nomeTF.setMaxLength(20);
		formPanel.add(nomeTF, formData);
		
		codigoTF = new TextField<String>();
		codigoTF.setName("codigo");
		codigoTF.setValue(cursoDTO.getCodigo());
		codigoTF.setFieldLabel("Código");
		codigoTF.setAllowBlank(false);
		codigoTF.setMinLength(3);
		codigoTF.setMaxLength(20);
		formPanel.add(codigoTF, formData);
		
		tipoCursoCB = new TipoCursoComboBox();
		tipoCursoCB.setName("tipoCurso");
		tipoCursoCB.setFieldLabel("Tipo");
		tipoCursoCB.setAllowBlank(false);
		tipoCursoCB.setValue(tipoCursoDTO);
		formPanel.add(tipoCursoCB, formData);
		
		numMinDoutoresTextField = new NumberField();
		numMinDoutoresTextField.setName("numMinDoutores");
		numMinDoutoresTextField.setValue(cursoDTO.getNumMinDoutores());
		numMinDoutoresTextField.setFieldLabel("% Min. PhD");
		numMinDoutoresTextField.setAllowBlank(false);
		numMinDoutoresTextField.setAllowDecimals(false);
		numMinDoutoresTextField.setMaxValue(99);
		formPanel.add(numMinDoutoresTextField, formData);
		
		numMinMestresTextField = new NumberField();
		numMinMestresTextField.setName("numMinMestres");
		numMinMestresTextField.setValue(cursoDTO.getNumMinMestres());
		numMinMestresTextField.setFieldLabel("% Min. MSc");
		numMinMestresTextField.setAllowBlank(false);
		numMinMestresTextField.setAllowDecimals(false);
		numMinMestresTextField.setMaxValue(99);
		formPanel.add(numMinMestresTextField, formData);
		
		maxDisciplinasPeloProfessorTextField = new NumberField();
		maxDisciplinasPeloProfessorTextField.setName("maxDisciplinasPeloProfessor");
		maxDisciplinasPeloProfessorTextField.setValue(cursoDTO.getMaxDisciplinasPeloProfessor());
		maxDisciplinasPeloProfessorTextField.setFieldLabel("Max. Disc. por Prof.");
		maxDisciplinasPeloProfessorTextField.setAllowBlank(false);
		maxDisciplinasPeloProfessorTextField.setAllowDecimals(false);
		maxDisciplinasPeloProfessorTextField.setMaxValue(99);
		formPanel.add(maxDisciplinasPeloProfessorTextField, formData);
		
		admMaisDeUmDisciplinaCheckBox = new CheckBox();
		admMaisDeUmDisciplinaCheckBox.setName("admMaisDeUmDisciplina");
		admMaisDeUmDisciplinaCheckBox.setValue(cursoDTO.getAdmMaisDeUmDisciplina());
		admMaisDeUmDisciplinaCheckBox.setFieldLabel("Permite mais de uma Disc. por Prof.?");
		formPanel.add(admMaisDeUmDisciplinaCheckBox, formData);
		
		FormButtonBinding binding = new FormButtonBinding(formPanel);
		binding.addButton(simpleModal.getSalvarBt());
	}
	
	public boolean isValid() {
		return formPanel.isValid();
	}
	
	@Override
	public Button getSalvarButton() {
		return simpleModal.getSalvarBt();
	}

	@Override
	public TextField<String> getNomeTextField() {
		return nomeTF;
	}
	
	@Override
	public SimpleModal getSimpleModal() {
		return simpleModal;
	}

	@Override
	public TextField<String> getCodigoTextField() {
		return codigoTF;
	}

	@Override
	public CursoDTO getCursoDTO() {
		return cursoDTO;
	}

	@Override
	public TipoCursoComboBox getTipoCursoComboBox() {
		return tipoCursoCB;
	}

	@Override
	public NumberField getNumMinDoutoresTextField() {
		return numMinDoutoresTextField;
	}

	@Override
	public NumberField getNumMinMestresTextField() {
		return numMinMestresTextField;
	}

	@Override
	public NumberField getMaxDisciplinasPeloProfessorTextField() {
		return maxDisciplinasPeloProfessorTextField;
	}

	@Override
	public CheckBox getAdmMaisDeUmDisciplinaCheckBox() {
		return admMaisDeUmDisciplinaCheckBox;
	}
	

}
