package com.gapso.web.trieda.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.client.mvp.model.GrupoSalaDTO;
import com.gapso.web.trieda.client.mvp.model.UnidadeDTO;
import com.gapso.web.trieda.client.mvp.presenter.GrupoSalaFormPresenter;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.CampusComboBox;
import com.gapso.web.trieda.client.util.view.SimpleModal;
import com.gapso.web.trieda.client.util.view.UnidadeComboBox;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class GrupoSalaFormView extends MyComposite implements GrupoSalaFormPresenter.Display {

	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private TextField<String> codigoTF;
	private TextField<String> nomeTF;
	private UnidadeComboBox unidadeCB;
	private GrupoSalaDTO grupoSalaDTO;
	private UnidadeDTO unidadeDTO;
	private Button salvarEAssociarBT;
	
	public GrupoSalaFormView(GrupoSalaDTO grupoSalaDTO, UnidadeDTO unidadeDTO) {
		this.grupoSalaDTO = grupoSalaDTO;
		this.unidadeDTO = unidadeDTO;
		initUI();
		// TODO
//		initComponent(simpleModal);
	}
	
	private void initUI() {
		String title = (grupoSalaDTO.getId() == null)? "Inserção de Grupo de Sala" : "Edição de Grupo de Sala";
		simpleModal = new SimpleModal(title, Resources.DEFAULTS.sala16());
		simpleModal.setHeight(190);
		simpleModal.setWidth(350);
		createForm();
		simpleModal.setContent(formPanel);
	}

	private void createForm() {
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);
		
		codigoTF = new TextField<String>();
		codigoTF.setName("codigo");
		codigoTF.setValue(grupoSalaDTO.getCodigo());
		codigoTF.setFieldLabel("Código");
		codigoTF.setAllowBlank(false);
		codigoTF.setMinLength(1);
		codigoTF.setMaxLength(20);
		codigoTF.setEmptyText("Preencha o código");
		formPanel.add(codigoTF, formData);
		
		CampusComboBox campusCB = new CampusComboBox();
		formPanel.add(campusCB, formData);
		
		unidadeCB = new UnidadeComboBox(campusCB);
		unidadeCB.setName("unidade");
		unidadeCB.setFieldLabel("Unidade");
		unidadeCB.setAllowBlank(false);
		unidadeCB.setValue(unidadeDTO);
		unidadeCB.setEmptyText("Selecione a unidade");
		formPanel.add(unidadeCB, formData);
		
		nomeTF = new TextField<String>();
		nomeTF.setName("nome");
		nomeTF.setValue(grupoSalaDTO.getNome());
		nomeTF.setFieldLabel("Nome");
		nomeTF.setAllowBlank(false);
		nomeTF.setMinLength(1);
		nomeTF.setMaxLength(50);
		nomeTF.setEmptyText("Preencha o nome");
		formPanel.add(nomeTF, formData);
		
		salvarEAssociarBT = new Button("Salvar e Associar Salas", AbstractImagePrototype.create(Resources.DEFAULTS.save16()));
		simpleModal.addButton(salvarEAssociarBT);
		
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
	public TextField<String> getCodigoTextField() {
		return codigoTF;
	}

	@Override
	public TextField<String> getNomeTextField() {
		return nomeTF;
	}
	
	@Override
	public UnidadeComboBox getUnidadeComboBox() {
		return unidadeCB;
	}
	
	@Override
	public GrupoSalaDTO getSalaDTO() {
		return grupoSalaDTO;
	}
	
	@Override
	public SimpleModal getSimpleModal() {
		return simpleModal;
	}

	@Override
	public Button getSalvarEAssociarButton() {
		return salvarEAssociarBT;
	}

}
