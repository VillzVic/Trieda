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
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.EstadoComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.UniqueDomain;
import com.gapso.web.trieda.shared.util.view.UniqueTextField;

public class CampusFormView
	extends MyComposite
	implements CampusFormPresenter.Display
{
	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private TextField< String > nomeTF;
	private TextField< String > codigoTF;
	private EstadoComboBox estadoCB;
	private TextField< String > municipioTF;
	private TextField< String > bairroTF;
	private NumberField qtdLimiteProfessorVirtualNF;
	private NumberField valorMedioProfessorVirtualNF;
	private NumberField valorCreditoNF;
	private CheckBox publicadoCB;
	private CenarioDTO cenarioDTO;
	private CampusDTO campusDTO;
	private CheckBox habilitaConfiguracoesProfessorVirtualCB;
	

	public CampusFormView( CenarioDTO cenarioDTO )
	{
		this( cenarioDTO, new CampusDTO() );
	}

	public CampusFormView(
		CenarioDTO cenarioDTO, CampusDTO campusDTO )
	{
		this.cenarioDTO = cenarioDTO;
		this.campusDTO = campusDTO;

		initUI();
	}
	
	private void initUI()
	{
		String title = ( ( campusDTO.getId() == null ) ?
			"Inserção de Campus" : "Edição de Campus" );

		this.simpleModal = new SimpleModal(
			title, Resources.DEFAULTS.campus16() );
		this.simpleModal.setHeight( 430 );
		this.simpleModal.setWidth( 400 );
		createForm();
		this.simpleModal.setContent( this.formPanel );
	}

	private void createForm()
	{
		FormData formData = new FormData( "-20" );

		this.formPanel = new FormPanel();
		this.formPanel.setHeaderVisible(false);
		this.formPanel.setLayout(new FlowLayout());

		FieldSet geralFS = new FieldSet();
		FormLayout formLayout = new FormLayout( LabelAlign.RIGHT );
		formLayout.setLabelWidth( 160 );
		geralFS.setLayout( formLayout );
		geralFS.setHeadingHtml( "Informações Gerais" );

		this.codigoTF = new UniqueTextField( this.cenarioDTO, UniqueDomain.CAMPI );
		this.codigoTF.setName( CampusDTO.PROPERTY_CODIGO );
		this.codigoTF.setValue( this.campusDTO.getCodigo() );
		this.codigoTF.setFieldLabel( "Código" );
		this.codigoTF.setAllowBlank( false );
		this.codigoTF.setMinLength( 1 );
		this.codigoTF.setMaxLength( 20 );
		this.codigoTF.setEmptyText( "Preencha o código" );
		geralFS.add( this.codigoTF, formData );

		this.nomeTF = new TextField< String >();
		this.nomeTF.setName( CampusDTO.PROPERTY_NOME );
		this.nomeTF.setValue( this.campusDTO.getNome() );
		this.nomeTF.setFieldLabel( "Nome" );
		this.nomeTF.setAllowBlank( false );
		this.nomeTF.setMinLength( 1 );
		this.nomeTF.setMaxLength( 50 );
		this.nomeTF.setEmptyText( "Preencha o nome" );
		geralFS.add( this.nomeTF, formData );

		this.valorCreditoNF = new NumberField();
		this.valorCreditoNF.setName( CampusDTO.PROPERTY_VALOR_CREDITO );
		this.valorCreditoNF.setValue( this.campusDTO.getValorCredito().getDoubleValue() );
		this.valorCreditoNF.setFieldLabel( "Custo Médio do Crédito (R$)" );
		this.valorCreditoNF.setAllowBlank( false );
		this.valorCreditoNF.setAllowDecimals( true );
		this.valorCreditoNF.setMaxValue( 999999 );
		this.valorCreditoNF.setEmptyText( "Custo médio do crédito (R$)" );
		this.valorCreditoNF.setToolTip("Representa o custo médio ponderado do tempo de aula, ou seja, o custo docente médio do campus, incluindo encargos.");
		geralFS.add( this.valorCreditoNF, formData );

		this.publicadoCB = new CheckBox();
		this.publicadoCB.setName( CampusDTO.PROPERTY_PUBLICADO );
		if ( campusDTO.getPublicado() != null )
		{
			this.publicadoCB.setValue( this.campusDTO.getPublicado() );
		}

		this.publicadoCB.setFieldLabel( "Publicar?" );
		this.publicadoCB.setLabelSeparator( "" );
		this.publicadoCB.hide();
		geralFS.add( this.publicadoCB, formData );

		this.formPanel.add( geralFS, formData );

		FieldSet enderecoFS = new FieldSet();
		formLayout = new FormLayout( LabelAlign.RIGHT );
		formLayout.setLabelWidth(160);
		enderecoFS.setLayout( formLayout );
		enderecoFS.setHeadingHtml( "Endereço" );

		this.estadoCB = new EstadoComboBox();
		this.estadoCB.setName( CampusDTO.PROPERTY_ESTADO );
		this.estadoCB.setValue( this.campusDTO.getEstado() );
		this.estadoCB.setFieldLabel( "Estado" );
		this.estadoCB.setEmptyText( "Selecione o estado" );
		enderecoFS.add( this.estadoCB, formData );

		this.municipioTF = new TextField< String >();
		this.municipioTF.setName( CampusDTO.PROPERTY_MUNICIPIO );
		this.municipioTF.setValue( this.campusDTO.getMunicipio() );
		this.municipioTF.setFieldLabel( "Município" );
		this.municipioTF.setMaxLength( 20 );
		this.municipioTF.setEmptyText( "Preencha o município" );
		enderecoFS.add( this.municipioTF, formData );

		this.bairroTF = new TextField< String >();
		this.bairroTF.setName( CampusDTO.PROPERTY_BAIRRO );
		this.bairroTF.setValue( this.campusDTO.getBairro() );
		bairroTF.setFieldLabel( "Bairro" );
		bairroTF.setMaxLength( 20 );
		bairroTF.setEmptyText( "Preencha o bairro" );
		enderecoFS.add( this.bairroTF, formData );

		this.formPanel.add( enderecoFS, formData );
		
		
		FieldSet configFS = new FieldSet();
		
		formLayout = new FormLayout( LabelAlign.RIGHT );
		formLayout.setLabelWidth(160);
		configFS.setLayout( formLayout );
		configFS.setHeadingHtml( "Config. Professor Virtual" );
		
		this.habilitaConfiguracoesProfessorVirtualCB = new CheckBox();
		this.habilitaConfiguracoesProfessorVirtualCB.setName("Configurações Professor Virtual");
		this.habilitaConfiguracoesProfessorVirtualCB.setTitle("Configurações Professor Virtual");
		this.habilitaConfiguracoesProfessorVirtualCB.setFieldLabel("Permite Configurações");
		this.habilitaConfiguracoesProfessorVirtualCB.setValue(this.campusDTO.getQtdLimiteProfessorVirtual()!=null || this.campusDTO.getValorMedioProfessorVirtual().getDoubleValue()!=null);

		configFS.add( this.habilitaConfiguracoesProfessorVirtualCB, formData );
		
		this.qtdLimiteProfessorVirtualNF = new NumberField();
		this.qtdLimiteProfessorVirtualNF.setName( CampusDTO.PROPERTY_QTD_LIMITE_PROFESSOR_VIRTUAL );
		this.qtdLimiteProfessorVirtualNF.setValue( this.campusDTO.getQtdLimiteProfessorVirtual());
		this.qtdLimiteProfessorVirtualNF.setAllowBlank( true );
		this.qtdLimiteProfessorVirtualNF.setAllowDecimals( false );
		this.qtdLimiteProfessorVirtualNF.setMaxValue( 999999 );
		this.qtdLimiteProfessorVirtualNF.setFieldLabel( "Qtd Limite" );
		this.qtdLimiteProfessorVirtualNF.setEmptyText( "Quantidade" );
		this.qtdLimiteProfessorVirtualNF.setToolTip("Representa a quantidade limite de Professores Virturais gerados na Otimização para o Campus.");
		this.qtdLimiteProfessorVirtualNF.setEnabled(this.campusDTO.getQtdLimiteProfessorVirtual()!=null || this.campusDTO.getValorMedioProfessorVirtual().getDoubleValue()!=null);
		configFS.add( this.qtdLimiteProfessorVirtualNF, formData );
		
		this.valorMedioProfessorVirtualNF = new NumberField();
		this.valorMedioProfessorVirtualNF.setName( CampusDTO.PROPERTY_VALOR_MEDIO_PROFESSOR_VIRTUAL );
		
		if(!(this.campusDTO.getValorMedioProfessorVirtual()==null)){
			this.valorMedioProfessorVirtualNF.setValue( this.campusDTO.getValorMedioProfessorVirtual().getDoubleValue() );
		}
		
		this.valorMedioProfessorVirtualNF.setFieldLabel( "Custo Médio (R$)" );
		this.valorMedioProfessorVirtualNF.setAllowBlank( true );
		this.valorMedioProfessorVirtualNF.setAllowDecimals( true );
		this.valorMedioProfessorVirtualNF.setMaxValue( 999999 );
		this.valorMedioProfessorVirtualNF.setEmptyText( "Ex: 65,00" );
		this.valorMedioProfessorVirtualNF.setToolTip("Representa o custo médio do Professor Virtual.");
		this.valorMedioProfessorVirtualNF.setEnabled(this.campusDTO.getQtdLimiteProfessorVirtual()!=null || this.campusDTO.getValorMedioProfessorVirtual().getDoubleValue()!=null);
		configFS.add( this.valorMedioProfessorVirtualNF, formData );
		
		
		this.formPanel.add( configFS, formData );
		
		
		FormButtonBinding binding = new FormButtonBinding( this.formPanel );
		binding.addButton( this.simpleModal.getSalvarBt() );
		
		this.simpleModal.setFocusWidget( this.codigoTF );
	}

	public boolean isValid()
	{
		return this.formPanel.isValid();
	}
	
	@Override
	public Button getSalvarButton()
	{
		return this.simpleModal.getSalvarBt();
	}

	@Override
	public TextField< String > getNomeTextField()
	{
		return this.nomeTF;
	}
	
	@Override
	public SimpleModal getSimpleModal()
	{
		return this.simpleModal;
	}

	@Override
	public TextField< String > getCodigoTextField()
	{
		return this.codigoTF;
	}

	@Override
	public NumberField getValorCreditoNumberField()
	{
		return this.valorCreditoNF;
	}

	@Override
	public CampusDTO getCampusDTO()
	{
		return this.campusDTO;
	}

	@Override
	public EstadoComboBox getEstadoComboBox()
	{
		return this.estadoCB;
	}

	@Override
	public TextField< String > getMunicipioTextField()
	{
		return this.municipioTF;
	}

	@Override
	public TextField< String > getBairroTextField()
	{
		return this.bairroTF;
	}

	@Override
	public CheckBox getPublicadoCheckBox()
	{
		return this.publicadoCB;
	}
	
	@Override
	public NumberField getQtdLimiteProfessorVirtualNumberField()
	{
		return this.qtdLimiteProfessorVirtualNF;
	}
	
	@Override
	public NumberField getValorMedioProfessorVirtualNumberField()
	{
		return this.valorMedioProfessorVirtualNF;
	}
	
	@Override
	public CheckBox getHabilitaConfiguracoesProfessorVirtualCheckBox()
	{
		return this.habilitaConfiguracoesProfessorVirtualCB;
	}
		
}
