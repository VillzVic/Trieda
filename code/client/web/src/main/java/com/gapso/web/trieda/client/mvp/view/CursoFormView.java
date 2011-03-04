package com.gapso.web.trieda.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.client.mvp.model.CursoDTO;
import com.gapso.web.trieda.client.mvp.presenter.CursoFormPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.gapso.web.trieda.client.util.view.TipoCursoComboBox;
import com.gapso.web.trieda.shared.dtos.TipoCursoDTO;

public class CursoFormView extends MyComposite implements CursoFormPresenter.Display {

	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private TextField<String> nomeTF;
	private TextField<String> codigoTF;
	private TipoCursoComboBox tipoCursoCB;
	private NumberField numMinDoutoresTF;
	private NumberField numMinMestresTF;
	private NumberField maxDisciplinasPeloProfessorTF;
	private CheckBox admMaisDeUmDisciplinaCB;
	private CursoDTO cursoDTO;
	private TipoCursoDTO tipoCursoDTO;
	
	public CursoFormView() {
		this(new CursoDTO(), null);
	}
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
		
		codigoTF = new TextField<String>();
		codigoTF.setName("codigo");
		codigoTF.setValue(cursoDTO.getCodigo());
		codigoTF.setFieldLabel("Código");
		codigoTF.setAllowBlank(false);
		codigoTF.setMinLength(1);
		codigoTF.setMaxLength(50);
		codigoTF.setEmptyText("Preencha o código");
		formPanel.add(codigoTF, formData);
		
		nomeTF = new TextField<String>();
		nomeTF.setName("nome");
		nomeTF.setValue(cursoDTO.getNome());
		nomeTF.setFieldLabel("Nome");
		nomeTF.setAllowBlank(false);
		nomeTF.setMinLength(1);
		nomeTF.setMaxLength(50);
		nomeTF.setEmptyText("Preencha o nome");
		formPanel.add(nomeTF, formData);
		
		tipoCursoCB = new TipoCursoComboBox();
		tipoCursoCB.setName("tipoCurso");
		tipoCursoCB.setFieldLabel("Tipo");
		tipoCursoCB.setAllowBlank(false);
		tipoCursoCB.setValue(tipoCursoDTO);
		tipoCursoCB.setEmptyText("Selecione um tipo de curso");
		formPanel.add(tipoCursoCB, formData);
		
		numMinMestresTF = new NumberField();
		numMinMestresTF.setName("numMinMestres");
		numMinMestresTF.setValue(cursoDTO.getNumMinMestres());
		numMinMestresTF.setFieldLabel("% Min. MSc");
		numMinMestresTF.setAllowBlank(false);
		numMinMestresTF.setAllowDecimals(false);
		numMinMestresTF.setMaxValue(100);
		numMinMestresTF.setEmptyText("Preencha a porcentagem mínima de mestres");
		formPanel.add(numMinMestresTF, formData);
		
		numMinDoutoresTF = new NumberField();
		numMinDoutoresTF.setName("numMinDoutores");
		numMinDoutoresTF.setValue(cursoDTO.getNumMinDoutores());
		numMinDoutoresTF.setFieldLabel("% Min. PhD");
		numMinDoutoresTF.setAllowBlank(false);
		numMinDoutoresTF.setAllowDecimals(false);
		numMinDoutoresTF.setMaxValue(100);
		numMinDoutoresTF.setEmptyText("Preencha a porcentagem mínima de doutores");
		formPanel.add(numMinDoutoresTF, formData);
		
		maxDisciplinasPeloProfessorTF = new NumberField();
		maxDisciplinasPeloProfessorTF.setName("maxDisciplinasPeloProfessor");
		maxDisciplinasPeloProfessorTF.setValue(cursoDTO.getMaxDisciplinasPeloProfessor());
		maxDisciplinasPeloProfessorTF.setFieldLabel("Max. Disc. por Prof.");
		maxDisciplinasPeloProfessorTF.setAllowBlank(false);
		maxDisciplinasPeloProfessorTF.setAllowDecimals(false);
		maxDisciplinasPeloProfessorTF.setMaxValue(99);
		maxDisciplinasPeloProfessorTF.setEmptyText("Número máximo de disciplina por professor");
		formPanel.add(maxDisciplinasPeloProfessorTF, formData);
		
		admMaisDeUmDisciplinaCB = new CheckBox();
		admMaisDeUmDisciplinaCB.setName("admMaisDeUmDisciplina");
		admMaisDeUmDisciplinaCB.setValue(cursoDTO.getAdmMaisDeUmDisciplina());
		admMaisDeUmDisciplinaCB.setFieldLabel("Permite mais de uma Disc. por Prof.?");
		formPanel.add(admMaisDeUmDisciplinaCB, formData);
		
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
		return numMinDoutoresTF;
	}

	@Override
	public NumberField getNumMinMestresTextField() {
		return numMinMestresTF;
	}

	@Override
	public NumberField getMaxDisciplinasPeloProfessorTextField() {
		return maxDisciplinasPeloProfessorTF;
	}

	@Override
	public CheckBox getAdmMaisDeUmDisciplinaCheckBox() {
		return admMaisDeUmDisciplinaCB;
	}
	

}
