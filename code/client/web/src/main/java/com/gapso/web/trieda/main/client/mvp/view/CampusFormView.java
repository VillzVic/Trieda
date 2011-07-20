package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.FieldSet;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FlowLayout;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.gapso.web.trieda.main.client.mvp.presenter.CampusFormPresenter;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.EstadoComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.UniqueDomain;
import com.gapso.web.trieda.shared.util.view.UniqueTextField;

public class CampusFormView extends MyComposite implements CampusFormPresenter.Display {

	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private TextField<String> nomeTF;
	private TextField<String> codigoTF;
	private EstadoComboBox estadoCB;
	private TextField<String> municipioTF;
	private TextField<String> bairroTF;
	private NumberField valorCreditoNF;
	private CheckBox publicadoCB;
	private CenarioDTO cenarioDTO;
	private CampusDTO campusDTO;
	
	public CampusFormView( CenarioDTO cenarioDTO )
	{
		this( cenarioDTO, new CampusDTO() );
	}

	public CampusFormView( CenarioDTO cenarioDTO, CampusDTO campusDTO )
	{
		this.cenarioDTO = cenarioDTO;
		this.campusDTO = campusDTO;
		initUI();
	}
	
	private void initUI()
	{
		String title = ( ( campusDTO.getId() == null )? "Inserção de Campus" : "Edição de Campus" );
		simpleModal = new SimpleModal( title, Resources.DEFAULTS.campus16() );
		simpleModal.setHeight( 350 );
		simpleModal.setWidth( 320 );
		createForm();
		simpleModal.setContent( formPanel );
	}

	private void createForm()
	{
		FormData formData = new FormData( "-20" );

		formPanel = new FormPanel();
		formPanel.setHeaderVisible(false);
		formPanel.setLayout(new FlowLayout());
		
		FieldSet geralFS = new FieldSet();
		FormLayout formLayout = new FormLayout(LabelAlign.RIGHT);
		formLayout.setLabelWidth(75);
		geralFS.setLayout(formLayout);
		geralFS.setHeading("Informações Gerais");
		
		codigoTF = new UniqueTextField(cenarioDTO, UniqueDomain.CAMPI);
		codigoTF.setName(CampusDTO.PROPERTY_CODIGO);
		codigoTF.setValue(campusDTO.getCodigo());
		codigoTF.setFieldLabel("Código");
		codigoTF.setAllowBlank(false);
		codigoTF.setMinLength(1);
		codigoTF.setMaxLength(20);
		codigoTF.setEmptyText("Preencha o código");
		geralFS.add(codigoTF, formData);
		
		nomeTF = new TextField<String>();
		nomeTF.setName(CampusDTO.PROPERTY_NOME);
		nomeTF.setValue(campusDTO.getNome());
		nomeTF.setFieldLabel("Nome");
		nomeTF.setAllowBlank(false);
		nomeTF.setMinLength(1);
		nomeTF.setMaxLength(50);
		nomeTF.setEmptyText("Preencha o nome");
		geralFS.add( nomeTF, formData );
		
		valorCreditoNF = new NumberField();
		valorCreditoNF.setName(CampusDTO.PROPERTY_VALOR_CREDITO);
		valorCreditoNF.setValue( campusDTO.getValorCredito().getDoubleValue() );
		valorCreditoNF.setFieldLabel("Custo (R$)");
		valorCreditoNF.setAllowBlank(false);
		valorCreditoNF.setAllowDecimals(true);
		valorCreditoNF.setMaxValue(999999);
		valorCreditoNF.setEmptyText("Custo médio do crédito (R$)");
		geralFS.add(valorCreditoNF, formData);

		publicadoCB = new CheckBox();
//		if(campusDTO.getPublicado() != null) {
//			publicadoCB.setEnabled(campusDTO.getPublicado());
//		}

		publicadoCB.setName(CampusDTO.PROPERTY_PUBLICADO);
		if(campusDTO.getPublicado() != null) {
			publicadoCB.setValue(campusDTO.getPublicado());
		}

		publicadoCB.setFieldLabel("Publicar?");
		publicadoCB.setLabelSeparator("");
		geralFS.add(publicadoCB, formData);

		formPanel.add(geralFS, formData);

		FieldSet enderecoFS = new FieldSet();
		formLayout = new FormLayout(LabelAlign.RIGHT);
		formLayout.setLabelWidth(75);
		enderecoFS.setLayout(formLayout);
		enderecoFS.setHeading("Endereço");

		estadoCB = new EstadoComboBox();
		estadoCB.setName(CampusDTO.PROPERTY_ESTADO);
		estadoCB.setValue(campusDTO.getEstado());
		estadoCB.setFieldLabel("Estado");
		estadoCB.setEmptyText("Selecione o estado");
		enderecoFS.add(estadoCB, formData);

		municipioTF = new TextField<String>();
		municipioTF.setName(CampusDTO.PROPERTY_MUNICIPIO);
		municipioTF.setValue(campusDTO.getMunicipio());
		municipioTF.setFieldLabel("Município");
		municipioTF.setMaxLength(20);
		municipioTF.setEmptyText("Preencha o município");
		enderecoFS.add(municipioTF, formData);

		bairroTF = new TextField<String>();
		bairroTF.setName(CampusDTO.PROPERTY_BAIRRO);
		bairroTF.setValue(campusDTO.getBairro());
		bairroTF.setFieldLabel("Bairro");
		bairroTF.setMaxLength(20);
		bairroTF.setEmptyText("Preencha o bairro");
		enderecoFS.add(bairroTF, formData);

		formPanel.add(enderecoFS, formData);

		FormButtonBinding binding = new FormButtonBinding( formPanel );
		binding.addButton( simpleModal.getSalvarBt() );
		
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
	public NumberField getValorCreditoNumberField() {
		return valorCreditoNF;
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
	@Override
	public CheckBox getPublicadoCheckBox() {
		return publicadoCB;
	}
	

}
