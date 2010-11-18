package com.gapso.web.trieda.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.gapso.web.trieda.client.mvp.model.CenarioDTO;
import com.gapso.web.trieda.client.mvp.presenter.CenarioEditarFormPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.SimpleModal;

public class CenarioEditarFormView extends MyComposite implements CenarioEditarFormPresenter.Display {

	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private CheckBox oficialCB;
	private TextField<String> nomeTF;
	private NumberField anoTF;
	private NumberField semestreTF;
	private TextField<String> comentarioTF;
	private CenarioDTO cenarioDTO;
	
	public CenarioEditarFormView(CenarioDTO cenarioDTO) {
		this.cenarioDTO = cenarioDTO;
		initUI();
		// TODO
//		initComponent(simpleModal);
//		setParent(null);
	}
	
	private void initUI() {
		String title = "Edição de Cenário";
		simpleModal = new SimpleModal(title, Resources.DEFAULTS.cenario16());
		simpleModal.setHeight(220);
		simpleModal.setWidth(400);
		createForm();
		simpleModal.setContent(formPanel);
	}

	private void createForm() {
		FormData formData = new FormData("-20");
		
		FormLayout formLayout = new FormLayout(LabelAlign.RIGHT);
		formLayout.setLabelWidth(75);
		
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);
		formPanel.setLayout(formLayout);
		
		oficialCB = new CheckBox();
		oficialCB.setName("oficial");
		oficialCB.setValue(cenarioDTO.getOficial());
		oficialCB.setFieldLabel("Oficial");
		formPanel.add(oficialCB, formData);
		
		nomeTF = new TextField<String>();
		nomeTF.setName("nome");
		nomeTF.setValue(cenarioDTO.getNome());
		nomeTF.setFieldLabel("Nome");
		nomeTF.setAllowBlank(false);
		nomeTF.setMinLength(5);
		nomeTF.setMaxLength(20);
		formPanel.add(nomeTF, formData);
		
		anoTF = new NumberField();
		anoTF.setName("ano");
		anoTF.setValue(cenarioDTO.getAno());
		anoTF.setFieldLabel("Ano");
		anoTF.setAllowDecimals(false);
		anoTF.setAllowBlank(false);
		anoTF.setMinValue(1);
		anoTF.setMaxValue(9999);
		formPanel.add(anoTF, formData);
		
		semestreTF = new NumberField();
		semestreTF.setName("semestre");
		semestreTF.setValue(cenarioDTO.getSemestre());
		semestreTF.setFieldLabel("Semestre");
		semestreTF.setAllowDecimals(false);
		semestreTF.setAllowBlank(false);
		semestreTF.setMinValue(1);
		semestreTF.setMaxValue(12);
		formPanel.add(semestreTF, formData);
		
		comentarioTF = new TextField<String>();
		comentarioTF.setName("comentario");
		comentarioTF.setValue(cenarioDTO.getComentario());
		comentarioTF.setFieldLabel("Comentário");
		comentarioTF.setMaxLength(255);
		formPanel.add(comentarioTF, formData);
		formPanel.add(formPanel, formData);
		
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
	public SimpleModal getSimpleModal() {
		return simpleModal;
	}
	
	@Override
	public CheckBox getOficialCheckBox() {
		return oficialCB;
	}
	
	@Override
	public TextField<String> getNomeTextField() {
		return nomeTF;
	}
	
	@Override
	public NumberField getAnoTextField() {
		return anoTF;
	}
	
	@Override
	public NumberField getSemestreTextField() {
		return semestreTF;
	}
	
	@Override
	public TextField<String> getComentarioTextField() {
		return comentarioTF;
	}

	@Override
	public CenarioDTO getCenarioDTO() {
		return cenarioDTO;
	}

}
