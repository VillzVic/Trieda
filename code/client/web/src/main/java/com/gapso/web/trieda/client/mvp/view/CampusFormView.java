package com.gapso.web.trieda.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FieldSet;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FlowLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.presenter.CampusFormPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.EstadoComboBox;
import com.gapso.web.trieda.client.util.view.SimpleModal;

public class CampusFormView extends MyComposite implements CampusFormPresenter.Display {

	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private TextField<String> nomeTF;
	private TextField<String> codigoTF;
	private EstadoComboBox estadoCB;
	private TextField<String> municipioTF;
	private TextField<String> bairroTF;
	private CampusDTO campusDTO;
	
	public CampusFormView(CampusDTO campusDTO) {
		this.campusDTO = campusDTO;
		initUI();
		// TODO
//		initComponent(simpleModal);
//		setParent(null);
	}
	
	private void initUI() {
		String title = (campusDTO.getId() == null)? "Inserção de Campus" : "Edição de Campus";
		simpleModal = new SimpleModal(title, Resources.DEFAULTS.campus16());
		simpleModal.setHeight(295);
		createForm();
		simpleModal.setContent(formPanel);
	}

	private void createForm() {
		FormData formData = new FormData("-20");
		
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);
		formPanel.setLayout(new FlowLayout());
		
		FieldSet geralFS = new FieldSet();
		FormLayout formLayout = new FormLayout(LabelAlign.RIGHT);
		formLayout.setLabelWidth(75);
		geralFS.setLayout(formLayout);
		geralFS.setHeading("Informações Gerais");
		
		nomeTF = new TextField<String>();
		nomeTF.setName("nome");
		nomeTF.setValue(campusDTO.getNome());
		nomeTF.setFieldLabel("Nome");
		nomeTF.setAllowBlank(false);
		nomeTF.setMinLength(1);
		nomeTF.setMaxLength(50);
		geralFS.add(nomeTF, formData);
		
		codigoTF = new TextField<String>();
		codigoTF.setName("codigo");
		codigoTF.setValue(campusDTO.getCodigo());
		codigoTF.setFieldLabel("Código");
		codigoTF.setAllowBlank(false);
		codigoTF.setMinLength(1);
		codigoTF.setMaxLength(20);
		geralFS.add(codigoTF, formData);
		
		formPanel.add(geralFS, formData);
		
		FieldSet enderecoFS = new FieldSet();
		formLayout = new FormLayout(LabelAlign.RIGHT);
		formLayout.setLabelWidth(75);
		enderecoFS.setLayout(formLayout);
		enderecoFS.setHeading("Endereço");
		
		estadoCB = new EstadoComboBox();
		estadoCB.setName("estado");
		estadoCB.setValue(campusDTO.getEstado());
		estadoCB.setFieldLabel("Estado");
		enderecoFS.add(estadoCB, formData);
		
		municipioTF = new TextField<String>();
		municipioTF.setName("municipio");
		municipioTF.setValue(campusDTO.getMunicipio());
		municipioTF.setFieldLabel("Município");
		municipioTF.setMaxLength(20);
		enderecoFS.add(municipioTF, formData);
		
		bairroTF = new TextField<String>();
		bairroTF.setName("bairro");
		bairroTF.setValue(campusDTO.getBairro());
		bairroTF.setFieldLabel("Bairro");
		bairroTF.setMaxLength(20);
		enderecoFS.add(bairroTF, formData);
		
		formPanel.add(enderecoFS, formData);
		
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
	public CampusDTO getCampusDTO() {
		return campusDTO;
	}

	@Override
	public EstadoComboBox getEstadoComboBox() {
		return estadoCB;
	}

	@Override
	public TextField<String> getMunicipioTextField() {
		return municipioTF;
	}

	@Override
	public TextField<String> getBairroTextField() {
		return bairroTF;
	}
	

}
