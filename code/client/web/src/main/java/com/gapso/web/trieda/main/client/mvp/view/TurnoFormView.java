package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.main.client.mvp.presenter.TurnoFormPresenter;
import com.gapso.web.trieda.main.client.util.resources.Resources;
import com.gapso.web.trieda.main.client.util.view.SimpleModal;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;

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
		String title = (turnoDTO.getId() == null)? "Inserção de Turno" : "Edição de Turno";
		simpleModal = new SimpleModal(title, Resources.DEFAULTS.turno16());
		simpleModal.setHeight(138);
		createForm();
		simpleModal.setContent(formPanel);
	}

	private void createForm() {
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);
		
		nomeTF = new TextField<String>();
		nomeTF.setName(TurnoDTO.PROPERTY_NOME);
		nomeTF.setValue(turnoDTO.getNome());
		nomeTF.setFieldLabel("Nome");
		nomeTF.setAllowBlank(false);
		nomeTF.setMinLength(1);
		nomeTF.setMaxLength(50);
		nomeTF.setEmptyText("Preencha o nome");
		formPanel.add(nomeTF, formData);
		
		tempoTF = new NumberField();
		tempoTF.setName(TurnoDTO.PROPERTY_TEMPO);
		tempoTF.setValue(turnoDTO.getTempo());
		tempoTF.setFieldLabel("Duração da Aula (min)");
		tempoTF.setAllowBlank(false);
		tempoTF.setAllowDecimals(false);
		tempoTF.setAllowNegative(false);
		tempoTF.setMinValue(1);
		tempoTF.setMaxValue(1000);
		tempoTF.setEmptyText("Preencha a duração de aula do turno");
		formPanel.add(tempoTF, formData);
		
		FormButtonBinding binding = new FormButtonBinding(formPanel);
		binding.addButton(simpleModal.getSalvarBt());
		
		simpleModal.setFocusWidget(nomeTF);
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
