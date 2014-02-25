package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.main.client.mvp.presenter.ProfessorFormPresenter;
import com.gapso.web.trieda.shared.dtos.AreaTitulacaoDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.TipoContratoDTO;
import com.gapso.web.trieda.shared.dtos.TitulacaoDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.AreaTitulacaoComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.TipoContratoComboBox;
import com.gapso.web.trieda.shared.util.view.TitulacaoComboBox;
import com.gapso.web.trieda.shared.util.view.UniqueDomain;
import com.gapso.web.trieda.shared.util.view.UniqueTextField;

public class ProfessorFormView
	extends MyComposite
	implements ProfessorFormPresenter.Display
{
	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private TextField< String > cpfTF;
	private TextField< String > nomeTF;
	private TipoContratoComboBox tipoContratoCB;
	private NumberField cargaHorariaMaxNF;
	private NumberField cargaHorariaMinNF;
	private TitulacaoComboBox titulacaoCB;
	private AreaTitulacaoComboBox areaTitulacaoCB;
	private NumberField creditoAnteriorNF;
	private NumberField valorCreditoNF;
	private NumberField maxDiasSemanaNF;
	private NumberField minCreditosDiaNF;
	private ProfessorDTO professorDTO;
	private TipoContratoDTO tipoContratoDTO;
	private TitulacaoDTO titulacaoDTO;
	private AreaTitulacaoDTO areaTitulacaoDTO;
	private CenarioDTO cenarioDTO;

	public ProfessorFormView( CenarioDTO cenarioDTO )
	{
		this( new ProfessorDTO(), null, null, null, cenarioDTO );
	}

	public ProfessorFormView( ProfessorDTO professorDTO, TipoContratoDTO tipoContratoDTO,
		TitulacaoDTO titulacaoDTO, AreaTitulacaoDTO areaTitulacaoDTO, CenarioDTO cenarioDTO )
	{
		this.professorDTO = professorDTO;
		this.tipoContratoDTO = tipoContratoDTO;
		this.titulacaoDTO = titulacaoDTO;
		this.areaTitulacaoDTO = areaTitulacaoDTO;
		this.cenarioDTO = cenarioDTO;

		initUI();
	}
	
	private void initUI()
	{
		String title = ( ( this.professorDTO.getId() == null ) ?
			"Inserção de Professor" : "Edição de Professor" );

		this.simpleModal = new SimpleModal(
			title, Resources.DEFAULTS.professor16() );

		this.simpleModal.setHeight( 500 );
		createForm();
		this.simpleModal.setContent( this.formPanel );
	}

	private void createForm()
	{
		FormData formData = new FormData( "-20" );
		this.formPanel = new FormPanel();
		this.formPanel.setHeaderVisible( false );

		this.cpfTF = new UniqueTextField( this.cenarioDTO, UniqueDomain.PROFESSOR );
		this.cpfTF.setName( ProfessorDTO.PROPERTY_CPF );
		this.cpfTF.setValue( professorDTO.getCpf() );
		this.cpfTF.setFieldLabel( "CPF" );
		this.cpfTF.setAllowBlank( false );
		this.cpfTF.setMaxLength( 14 );
		this.cpfTF.setMinLength( 14 );
		this.cpfTF.setEmptyText( "Preencha o CPF" );
		this.formPanel.add( this.cpfTF, formData );

		this.nomeTF = new TextField< String >();
		this.nomeTF.setName( ProfessorDTO.PROPERTY_NOME );
		this.nomeTF.setValue( this.professorDTO.getNome() );
		this.nomeTF.setFieldLabel( "Nome" );
		this.nomeTF.setAllowBlank( false );
		this.nomeTF.setMinLength( 3 );
		this.nomeTF.setMaxLength( 50 );
		this.nomeTF.setEmptyText( "Preencha o nome" );
		this.formPanel.add( this.nomeTF, formData );

		this.tipoContratoCB = new TipoContratoComboBox( cenarioDTO );
		this.tipoContratoCB.setAllowBlank( false );
		this.tipoContratoCB.setValue( this.tipoContratoDTO );
		this.formPanel.add( this.tipoContratoCB, formData );

		this.cargaHorariaMaxNF = new NumberField();
		this.cargaHorariaMaxNF.setName( ProfessorDTO.PROPERTY_CARGA_HORARIA_MAX );
		this.cargaHorariaMaxNF.setValue( this.professorDTO.getCargaHorariaMax() );
		this.cargaHorariaMaxNF.setFieldLabel( "Carga Horária Máx." );
		this.cargaHorariaMaxNF.setAllowBlank( false );
		this.cargaHorariaMaxNF.setAllowDecimals( false );
		this.cargaHorariaMaxNF.setMaxValue( 99 );
		this.cargaHorariaMaxNF.setEmptyText( "Somente números" );
		this.formPanel.add( this.cargaHorariaMaxNF, formData );

		this.cargaHorariaMinNF = new NumberField();
		this.cargaHorariaMinNF.setName( ProfessorDTO.PROPERTY_CARGA_HORARIA_MIN );
		this.cargaHorariaMinNF.setValue( this.professorDTO.getCargaHorariaMin() );
		this.cargaHorariaMinNF.setFieldLabel( "Carga Horária Min." );
		this.cargaHorariaMinNF.setAllowBlank( false );
		this.cargaHorariaMinNF.setAllowDecimals( false );
		this.cargaHorariaMinNF.setMaxValue( 99 );
		this.cargaHorariaMinNF.setEmptyText( "Somente números" );
		this.formPanel.add( this.cargaHorariaMinNF, formData );

		this.titulacaoCB = new TitulacaoComboBox( cenarioDTO );
		this.titulacaoCB.setAllowBlank( false );
		this.titulacaoCB.setValue( this.titulacaoDTO );
		this.formPanel.add( this.titulacaoCB, formData );

		this.areaTitulacaoCB = new AreaTitulacaoComboBox();
		this.areaTitulacaoCB.setValue( this.areaTitulacaoDTO );
		this.formPanel.add( this.areaTitulacaoCB, formData );

		this.creditoAnteriorNF = new NumberField();
		this.creditoAnteriorNF.setName( ProfessorDTO.PROPERTY_CREDITO_ANTERIOR );
		this.creditoAnteriorNF.setValue( this.professorDTO.getCreditoAnterior() );
		this.creditoAnteriorNF.setFieldLabel( "Carga Horária Anterior" );
		this.creditoAnteriorNF.setAllowBlank( false );
		this.creditoAnteriorNF.setAllowDecimals( false );
		this.creditoAnteriorNF.setMaxValue( 99 );
		this.creditoAnteriorNF.setEmptyText( "Somente números" );
		this.formPanel.add( this.creditoAnteriorNF, formData );

		this.valorCreditoNF = new NumberField();
		this.valorCreditoNF.setName( ProfessorDTO.PROPERTY_VALOR_CREDITO );
		this.valorCreditoNF.setValue( this.professorDTO.getValorCredito().getDoubleValue() );
		this.valorCreditoNF.setFieldLabel( "Crédito (R$)" );
		this.valorCreditoNF.setAllowBlank( false );
		this.valorCreditoNF.setAllowDecimals( true );
		this.valorCreditoNF.setMaxValue( 999999 );
		this.valorCreditoNF.setEmptyText( "Somente números" );
		this.formPanel.add( this.valorCreditoNF, formData );
		
		this.maxDiasSemanaNF = new NumberField();
		this.maxDiasSemanaNF.setName( ProfessorDTO.PROPERTY_MAX_DIAS_SEMANA );
		this.maxDiasSemanaNF.setValue( this.professorDTO.getMaxDiasSemana() );
		this.maxDiasSemanaNF.setFieldLabel( "Máx. Dias por Semana" );
		this.maxDiasSemanaNF.setAllowBlank( false );
		this.maxDiasSemanaNF.setAllowDecimals( false );
		this.maxDiasSemanaNF.setMaxValue( 7 );
		this.maxDiasSemanaNF.setEmptyText( "Somente números" );
		this.formPanel.add( this.maxDiasSemanaNF, formData );
		
		this.minCreditosDiaNF = new NumberField();
		this.minCreditosDiaNF.setName( ProfessorDTO.PROPERTY_MIN_CREDITOS_DIA );
		this.minCreditosDiaNF.setValue( this.professorDTO.getMinCreditosDia() );
		this.minCreditosDiaNF.setFieldLabel( "Min. Creditos Diários" );
		this.minCreditosDiaNF.setAllowBlank( false );
		this.minCreditosDiaNF.setAllowDecimals( false );
		this.minCreditosDiaNF.setMaxValue( 100 );
		this.minCreditosDiaNF.setEmptyText( "Somente números" );
		this.formPanel.add( this.minCreditosDiaNF, formData );

		FormButtonBinding binding = new FormButtonBinding( this.formPanel );
		binding.addButton( this.simpleModal.getSalvarBt() );

		this.simpleModal.setFocusWidget( this.cpfTF );
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
	public SimpleModal getSimpleModal()
	{
		return this.simpleModal;
	}

	@Override
	public TextField< String > getNomeTextField()
	{
		return this.nomeTF;
	}

	@Override
	public TextField< String > getCpfTextField()
	{
		return this.cpfTF;
	}

	@Override
	public TipoContratoComboBox getTipoContratoComboBox()
	{
		return this.tipoContratoCB;
	}

	@Override
	public NumberField getCargaHorariaMaxNumberField()
	{
		return this.cargaHorariaMaxNF;
	}

	@Override
	public NumberField getCargaHorariaMinNumberField()
	{
		return this.cargaHorariaMinNF;
	}

	@Override
	public TitulacaoComboBox getTitulacaoComboBox()
	{
		return this.titulacaoCB;
	}

	@Override
	public AreaTitulacaoComboBox getAreaTitulacaoComboBox()
	{
		return this.areaTitulacaoCB;
	}

	@Override
	public NumberField getCreditoAnteriorNumberField()
	{
		return this.creditoAnteriorNF;
	}

	@Override
	public NumberField getValorCreditoNumberField()
	{
		return this.valorCreditoNF;
	}

	@Override
	public NumberField getMaxDiasSemanaNumberField()
	{
		return this.maxDiasSemanaNF;
	}
	
	@Override
	public NumberField getMinCreditosDiaNumberField()
	{
		return this.minCreditosDiaNF;
	}
	
	@Override
	public ProfessorDTO getProfessorDTO()
	{
		return this.professorDTO;
	}
}
