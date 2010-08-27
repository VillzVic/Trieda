package com.gapso.web.trieda.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.client.mvp.presenter.TurnoFormPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.SimpleModal;

public class TurnoFormView extends MyComposite implements TurnoFormPresenter.Display {

	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private TextField<String> nomeTF;
	private NumberField tempoTF;
	
	public TurnoFormView() {
		initUI();
		// TODO
		getParent();
//		initComponent(simpleModal);
//		setParent(null);
	}
	
	private void initUI() {
		simpleModal = new SimpleModal("Turno", Resources.DEFAULTS.turnos16());
		createForm();
		simpleModal.setContent(formPanel);
	}

	private void createForm() {
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);
		
		nomeTF = new TextField<String>();
		nomeTF.setName("nome");
		nomeTF.setFieldLabel("Nome");
		nomeTF.setAllowBlank(false);
		formPanel.add(nomeTF, formData);
		
		tempoTF = new NumberField();
		tempoTF.setName("tempo");
		tempoTF.setFieldLabel("Tempo");
		tempoTF.setAllowBlank(false);
		formPanel.add(tempoTF, formData);
		
		FormButtonBinding binding = new FormButtonBinding(formPanel);
		binding.addButton(simpleModal.getSalvarBt());
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
	public NumberField getTempoTextField() {
		return tempoTF;
	}

}
