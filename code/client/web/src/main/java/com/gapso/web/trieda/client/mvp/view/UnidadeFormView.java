package com.gapso.web.trieda.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.UnidadeDTO;
import com.gapso.web.trieda.client.mvp.presenter.UnidadeFormPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.CampusComboBox;
import com.gapso.web.trieda.client.util.view.SimpleModal;

public class UnidadeFormView extends MyComposite implements UnidadeFormPresenter.Display {

	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private TextField<String> nomeTF;
	private TextField<String> codigoTF;
	private CampusComboBox campusCB;
	private UnidadeDTO unidadeDTO;
	private CampusDTO campusDTO;
	
	public UnidadeFormView(UnidadeDTO unidadeDTO, CampusDTO campusDTO) {
		this.unidadeDTO = unidadeDTO;
		this.campusDTO = campusDTO;
		initUI();
		// TODO
//		initComponent(simpleModal);
//		setParent(null);
	}
	
	private void initUI() {
		String title = (unidadeDTO.getId() == null)? "Inserção de Unidade" : "Edição de Unidade";
		simpleModal = new SimpleModal(title, Resources.DEFAULTS.unidade16());
		simpleModal.setHeight(160);
		createForm();
		simpleModal.setContent(formPanel);
	}

	private void createForm() {
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);
		
		codigoTF = new TextField<String>();
		codigoTF.setName("codigo");
		codigoTF.setValue(unidadeDTO.getCodigo());
		codigoTF.setFieldLabel("Código");
		codigoTF.setAllowBlank(false);
		codigoTF.setMinLength(1);
		codigoTF.setMaxLength(20);
		formPanel.add(codigoTF, formData);
		
		nomeTF = new TextField<String>();
		nomeTF.setName("nome");
		nomeTF.setValue(unidadeDTO.getNome());
		nomeTF.setFieldLabel("Nome");
		nomeTF.setAllowBlank(false);
		nomeTF.setMinLength(1);
		nomeTF.setMaxLength(50);
		formPanel.add(nomeTF, formData);
		
		campusCB = new CampusComboBox();
		campusCB.setName("campus");
		campusCB.setFieldLabel("Campus");
		campusCB.setAllowBlank(false);
		campusCB.setValue(campusDTO);
		formPanel.add(campusCB, formData);
		
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
	public CampusComboBox getCampusComboBox() {
		return campusCB;
	}

	@Override
	public UnidadeDTO getUnidadeDTO() {
		return unidadeDTO;
	}
	

}
