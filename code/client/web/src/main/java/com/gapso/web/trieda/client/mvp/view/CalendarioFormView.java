package com.gapso.web.trieda.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.client.mvp.model.CalendarioDTO;
import com.gapso.web.trieda.client.mvp.presenter.CalendarioFormPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.SimpleModal;

public class CalendarioFormView extends MyComposite implements CalendarioFormPresenter.Display {

	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private TextField<String> codigoTF;
	private TextField<String> descricaoTF;
	private CalendarioDTO calendarioDTO;
	
	public CalendarioFormView(CalendarioDTO calendarioDTO) {
		this.calendarioDTO = calendarioDTO;
		initUI();
		// TODO
//		initComponent(simpleModal);
//		setParent(null);
	}
	
	private void initUI() {
		simpleModal = new SimpleModal("Calendario", Resources.DEFAULTS.calendario16());
		createForm();
		simpleModal.setContent(formPanel);
	}

	private void createForm() {
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);
		
		codigoTF = new TextField<String>();
		codigoTF.setName("codigo");
		codigoTF.setValue(calendarioDTO.getCodigo());
		codigoTF.setFieldLabel("Codigo");
		codigoTF.setAllowBlank(false);
		codigoTF.setMinLength(3);
		codigoTF.setMaxLength(20);
		formPanel.add(codigoTF, formData);
		
		descricaoTF = new TextField<String>();
		descricaoTF.setName("descricao");
		descricaoTF.setValue(calendarioDTO.getDescricao());
		descricaoTF.setFieldLabel("Descrição");
		descricaoTF.setMaxLength(255);
		formPanel.add(descricaoTF, formData);
		
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
	public TextField<String> getCodigoTextField() {
		return codigoTF;
	}
	
	@Override
	public SimpleModal getSimpleModal() {
		return simpleModal;
	}

	@Override
	public TextField<String> getDescricaoTextField() {
		return descricaoTF;
	}

	@Override
	public CalendarioDTO getCalendarioDTO() {
		return calendarioDTO;
	}
	

}
