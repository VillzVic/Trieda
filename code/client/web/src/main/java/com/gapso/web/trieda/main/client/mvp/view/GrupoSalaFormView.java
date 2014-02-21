package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.main.client.mvp.presenter.GrupoSalaFormPresenter;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.GrupoSalaDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.UnidadeComboBox;
import com.gapso.web.trieda.shared.util.view.UniqueDomain;
import com.gapso.web.trieda.shared.util.view.UniqueTextField;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class GrupoSalaFormView extends MyComposite implements GrupoSalaFormPresenter.Display {

	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private TextField<String> codigoTF;
	private TextField<String> nomeTF;
	private UnidadeComboBox unidadeCB;
	private GrupoSalaDTO grupoSalaDTO;
	private UnidadeDTO unidadeDTO;
	private CampusDTO campusDTO;
	private CenarioDTO cenarioDTO;
	private Button salvarEAssociarBT;

	public GrupoSalaFormView( GrupoSalaDTO grupoSalaDTO, CampusDTO campusDTO,
		UnidadeDTO unidadeDTO, CenarioDTO cenarioDTO )
	{
		this.grupoSalaDTO = grupoSalaDTO;
		this.unidadeDTO = unidadeDTO;
		this.campusDTO = campusDTO;
		this.cenarioDTO = cenarioDTO;

		initUI();
	}
	
	private void initUI()
	{
		String title = ( !isEdited()? "Inserção de Grupo de Ambientes" : "Edição de Grupo de Ambientes" );
		simpleModal = new SimpleModal( title, Resources.DEFAULTS.sala16() );
		simpleModal.setHeight( 190 );
		simpleModal.setWidth( 350 );

		createForm();
		simpleModal.setContent( formPanel );
	}

	private boolean isEdited()
	{
		return ( grupoSalaDTO.getId() != null );
	}
	
	private void createForm()
	{
		FormData formData = new FormData("-20");
		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);

		codigoTF = new UniqueTextField(	cenarioDTO, UniqueDomain.GRUPO_SALA );
		codigoTF.setName(GrupoSalaDTO.PROPERTY_CODIGO);
		codigoTF.setValue(grupoSalaDTO.getCodigo());
		codigoTF.setFieldLabel("Código");
		codigoTF.setAllowBlank(false);
		codigoTF.setMinLength(1);
		codigoTF.setMaxLength(20);
		codigoTF.setEmptyText("Preencha o código");
		formPanel.add(codigoTF, formData);
		
		nomeTF = new TextField<String>();
		nomeTF.setName(GrupoSalaDTO.PROPERTY_NOME);
		nomeTF.setValue(grupoSalaDTO.getNome());
		nomeTF.setFieldLabel("Nome");
		nomeTF.setAllowBlank(false);
		nomeTF.setMinLength(1);
		nomeTF.setMaxLength(50);
		nomeTF.setEmptyText("Preencha o nome");
		formPanel.add(nomeTF, formData);
		
		CampusComboBox campusCB = new CampusComboBox(cenarioDTO);
		campusCB.setEnabled(!isEdited());
		campusCB.setValue(campusDTO);
		formPanel.add(campusCB, formData);
		
		unidadeCB = new UnidadeComboBox(campusCB);
		unidadeCB.setName("unidade");
		unidadeCB.setFieldLabel("Unidade");
		unidadeCB.setAllowBlank(false);
		unidadeCB.setValue(unidadeDTO);
		unidadeCB.setEmptyText("Selecione a unidade");
		unidadeCB.setEnabled(!isEdited());
		formPanel.add(unidadeCB, formData);
		
		salvarEAssociarBT = new Button("Salvar e Associar Ambientes", AbstractImagePrototype.create(Resources.DEFAULTS.save16()));
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
