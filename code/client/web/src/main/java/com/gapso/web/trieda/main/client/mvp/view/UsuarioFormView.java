package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FieldSet;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.gapso.web.trieda.main.client.mvp.presenter.UsuarioFormPresenter;
import com.gapso.web.trieda.shared.dtos.UsuarioDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.ProfessorComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleModal;

public class UsuarioFormView extends MyComposite implements UsuarioFormPresenter.Display {

	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private TextField<String> nomeTF;
	private TextField<String> emailTF;
	private TextField<String> usernameTF;
	private TextField<String> passwordTF;
	private ProfessorComboBox professorCB;
	private UsuarioDTO usuarioDTO;
	
	public UsuarioFormView(UsuarioDTO usuarioDTO) {
		this.usuarioDTO = usuarioDTO;
		initUI();
		// TODO
//		initComponent(simpleModal);
//		setParent(null);
	}
	
	private void initUI() {
		String title = (usuarioDTO.getVersion() == null)? "Inserção de Usuário" : "Edição de Usuário";
		simpleModal = new SimpleModal(title, Resources.DEFAULTS.turno16());
		simpleModal.setHeight(255);
		createForm();
		simpleModal.setContent(formPanel);
	}

	private void createForm() {
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);
		
		nomeTF = new TextField<String>();
		nomeTF.setValue(usuarioDTO.getNome());
		nomeTF.setFieldLabel("Nome");
		nomeTF.setAllowBlank(false);
		nomeTF.setMaxLength(20);
		nomeTF.setEmptyText("Preencha o nome");
		formPanel.add(nomeTF, formData);
		
		emailTF = new TextField<String>();
		emailTF.setValue(usuarioDTO.getEmail());
		emailTF.setFieldLabel("Email");
		emailTF.setAllowBlank(false);
		emailTF.setMinLength(5);
		emailTF.setMaxLength(100);
		emailTF.setEmptyText("Preencha o email");
		formPanel.add(emailTF, formData);
		
		usernameTF = new TextField<String>();
		usernameTF.setValue(usuarioDTO.getUsername());
		usernameTF.setFieldLabel("Username");
		usernameTF.setAllowBlank(false);
		usernameTF.setMinLength(5);
		usernameTF.setMaxLength(20);
		usernameTF.setEmptyText("Preencha o username");
		usernameTF.setReadOnly(usuarioDTO.getVersion() != null);
		formPanel.add(usernameTF, formData);
		
		passwordTF = new TextField<String>();
		passwordTF.setFieldLabel("Password");
		passwordTF.setAllowBlank(usuarioDTO.getVersion() != null);
		passwordTF.setMinLength(5);
		passwordTF.setMaxLength(20);
		passwordTF.setEmptyText("Preencha o password");
		passwordTF.setPassword(true);
		formPanel.add(passwordTF, formData);
		
	    FieldSet fieldSet = new FieldSet();
	    FormLayout layout = new FormLayout();  
	    layout.setLabelWidth(70);
	    fieldSet.setLayout(layout);
	    fieldSet.setHeading("Professor?");  
	    fieldSet.setCheckboxToggle(true);
	    fieldSet.setExpanded(false);
	    
	    professorCB = new ProfessorComboBox();
//		professorCB.setValue(usuarioDTO.isProfessor());
		fieldSet.add(professorCB, formData);
		
		formPanel.add(fieldSet);
		
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
	public TextField<String> getEmailTextField() {
		return emailTF;
	}
	
	@Override
	public TextField<String> getUsernameTextField() {
		return usernameTF;
	}
	
	@Override
	public TextField<String> getPasswordTextField() {
		return passwordTF;
	}
	
	@Override
	public SimpleModal getSimpleModal() {
		return simpleModal;
	}

	@Override
	public UsuarioDTO getUsuarioDTO() {
		return usuarioDTO;
	}

	@Override
	public ProfessorComboBox getProfessorComboBox() {
		return professorCB;
	}
	
}
