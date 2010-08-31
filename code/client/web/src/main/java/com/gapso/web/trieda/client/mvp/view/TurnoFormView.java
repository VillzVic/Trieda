package com.gapso.web.trieda.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.gapso.web.trieda.client.mvp.presenter.TurnoFormPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.SimpleModal;

public class TurnoFormView extends MyComposite implements TurnoFormPresenter.Display {

	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private TextField<String> nomeTF;
	private NumberField tempoTF;
	private TurnoDTO turnoDTO;
	
	public TurnoFormView(TurnoDTO turnoDTO) {
		this.turnoDTO = turnoDTO;
		initUI();
		// TODO
//		initComponent(simpleModal);
//		setParent(null);
	}
	
	private void initUI() {
		simpleModal = new SimpleModal("Turno", Resources.DEFAULTS.turnos16());
		simpleModal.setHeight(138);
		createForm();
		simpleModal.setContent(formPanel);
	}

	private void createForm() {
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);
		
		nomeTF = new TextField<String>();
		nomeTF.setName("nome");
		nomeTF.setValue(turnoDTO.getNome());
		nomeTF.setFieldLabel("Nome");
		nomeTF.setAllowBlank(false);
		nomeTF.setMaxLength(50);
		formPanel.add(nomeTF, formData);
		
		tempoTF = new NumberField();
		tempoTF.setName("tempo");
		tempoTF.setValue(turnoDTO.getTempo());
		tempoTF.setFieldLabel("Tempo");
		tempoTF.setAllowBlank(false);
		tempoTF.setAllowDecimals(false);
		tempoTF.setAllowNegative(false);
		tempoTF.setMinValue(1);
		tempoTF.setMaxValue(1000);
		formPanel.add(tempoTF, formData);
		
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
	public NumberField getTempoTextField() {
		return tempoTF;
	}

	@Override
	public TurnoDTO getTurnoDTO() {
		return turnoDTO;
	}
	

}
