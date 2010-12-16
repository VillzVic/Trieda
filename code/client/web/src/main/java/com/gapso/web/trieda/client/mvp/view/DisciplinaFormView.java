package com.gapso.web.trieda.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.client.mvp.model.DisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.TipoDisciplinaDTO;
import com.gapso.web.trieda.client.mvp.presenter.DisciplinaFormPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.DificuldadeComboBox;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.gapso.web.trieda.client.util.view.TipoDisciplinaComboBox;

public class DisciplinaFormView extends MyComposite implements DisciplinaFormPresenter.Display {

	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private TextField<String> nomeTF;
	private TextField<String> codigoTF;
	private NumberField creditosTeoricoTF;
	private NumberField creditosPraticoTF;
	private CheckBox laboratorioCB;
	private TipoDisciplinaComboBox tipoDisciplinaCB;
	private DificuldadeComboBox dificuldadeCB;
	private NumberField maxAlunosTeoricoTF;
	private NumberField maxAlunosPraticoTF;
	private DisciplinaDTO disciplinaDTO;
	private TipoDisciplinaDTO tipoDisciplinaDTO;

	public DisciplinaFormView(DisciplinaDTO disciplinaDTO, TipoDisciplinaDTO tipoDisciplinaDTO) {
		this.disciplinaDTO = disciplinaDTO;
		this.tipoDisciplinaDTO = tipoDisciplinaDTO;
		initUI();
		// TODO
//		initComponent(simpleModal);
//		setParent(null);
	}
	
	private void initUI() {
		String title = (disciplinaDTO.getId() == null)? "Inserção de Disciplina" : "Edição de Disciplina";
		simpleModal = new SimpleModal(title, Resources.DEFAULTS.disciplina16());
		simpleModal.setHeight(400);
		createForm();
		simpleModal.setContent(formPanel);
	}

	private void createForm() {
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);
		
		nomeTF = new TextField<String>();
		nomeTF.setName("nome");
		nomeTF.setValue(disciplinaDTO.getNome());
		nomeTF.setFieldLabel("Nome");
		nomeTF.setAllowBlank(false);
		nomeTF.setMinLength(1);
		nomeTF.setMaxLength(50);
		formPanel.add(nomeTF, formData);
		
		codigoTF = new TextField<String>();
		codigoTF.setName("codigo");
		codigoTF.setValue(disciplinaDTO.getCodigo());
		codigoTF.setFieldLabel("Código");
		codigoTF.setAllowBlank(false);
		codigoTF.setMinLength(1);
		codigoTF.setMaxLength(20);
		formPanel.add(codigoTF, formData);
		
		creditosTeoricoTF = new NumberField();
		creditosTeoricoTF.setName("creditosTeorico");
		creditosTeoricoTF.setValue(disciplinaDTO.getCreditosTeorico());
		creditosTeoricoTF.setFieldLabel("Créditos Teóricos");
		creditosTeoricoTF.setAllowBlank(false);
		creditosTeoricoTF.setAllowDecimals(false);
		creditosTeoricoTF.setMaxValue(99);
		formPanel.add(creditosTeoricoTF, formData);
		
		creditosPraticoTF = new NumberField();
		creditosPraticoTF.setName("creditosPratico");
		creditosPraticoTF.setValue(disciplinaDTO.getCreditosPratico());
		creditosPraticoTF.setFieldLabel("Créditos Práticos");
		creditosPraticoTF.setAllowBlank(false);
		creditosPraticoTF.setAllowDecimals(false);
		creditosPraticoTF.setMaxValue(99);
		formPanel.add(creditosPraticoTF, formData);
		
		laboratorioCB = new CheckBox();
		laboratorioCB.setName("laboratorio");
		laboratorioCB.setValue(disciplinaDTO.getLaboratorio());
		laboratorioCB.setFieldLabel("Usa Laboratório");
		formPanel.add(laboratorioCB, formData);
		
		tipoDisciplinaCB = new TipoDisciplinaComboBox();
		tipoDisciplinaCB.setName("tipoDisciplina");
		tipoDisciplinaCB.setFieldLabel("Tipo de Disciplina");
		tipoDisciplinaCB.setAllowBlank(false);
		tipoDisciplinaCB.setValue(tipoDisciplinaDTO);
		formPanel.add(tipoDisciplinaCB, formData);
		
		dificuldadeCB = new DificuldadeComboBox();
		dificuldadeCB.setName("dificuldade");
		dificuldadeCB.setFieldLabel("Nível de Dificuldade");
		dificuldadeCB.setAllowBlank(false);
		dificuldadeCB.setValue(disciplinaDTO.getDificuldade());
		formPanel.add(dificuldadeCB, formData);
		
		maxAlunosTeoricoTF = new NumberField();
		maxAlunosTeoricoTF.setName("maxAlunosTeorico");
		maxAlunosTeoricoTF.setValue(disciplinaDTO.getMaxAlunosTeorico());
		maxAlunosTeoricoTF.setFieldLabel("Max. Alunos - Teórico");
		maxAlunosTeoricoTF.setAllowBlank(false);
		maxAlunosTeoricoTF.setAllowDecimals(false);
		maxAlunosTeoricoTF.setMaxValue(999);
		formPanel.add(maxAlunosTeoricoTF, formData);
		
		maxAlunosPraticoTF = new NumberField();
		maxAlunosPraticoTF.setName("maxAlunosPratico");
		maxAlunosPraticoTF.setValue(disciplinaDTO.getMaxAlunosPratico());
		maxAlunosPraticoTF.setFieldLabel("Max. Alunos - Prático");
		maxAlunosPraticoTF.setAllowBlank(false);
		maxAlunosPraticoTF.setAllowDecimals(false);
		maxAlunosPraticoTF.setMaxValue(999);
		formPanel.add(maxAlunosPraticoTF, formData);
		
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
	public DisciplinaDTO getDisciplinaDTO() {
		return disciplinaDTO;
	}

	@Override
	public TipoDisciplinaComboBox getTipoDisciplinaComboBox() {
		return tipoDisciplinaCB;
	}

	@Override
	public NumberField getCreditosTeoricoTextField() {
		return creditosTeoricoTF;
	}

	@Override
	public NumberField getCreditosPraticoTextField() {
		return creditosPraticoTF;
	}

	@Override
	public CheckBox getLaboratorioCheckBox() {
		return laboratorioCB;
	}

	@Override
	public DificuldadeComboBox getDificuldadeComboBox() {
		return dificuldadeCB;
	}

	@Override
	public NumberField getMaxAlunosTeoricoTextField() {
		return maxAlunosTeoricoTF;
	}

	@Override
	public NumberField getMaxAlunosPraticoTextField() {
		return maxAlunosPraticoTF;
	}

}
