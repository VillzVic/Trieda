package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextArea;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.main.client.mvp.presenter.SalaFormPresenter;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.TipoSalaDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.CampusComboBox;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.TipoSalaComboBox;
import com.gapso.web.trieda.shared.util.view.UnidadeComboBox;
import com.gapso.web.trieda.shared.util.view.UniqueDomain;
import com.gapso.web.trieda.shared.util.view.UniqueTextField;

public class SalaFormView extends MyComposite implements
	SalaFormPresenter.Display
{
	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private TextField< String > codigoTF;
	private TextField< String > numeroTF;
	private TextArea descricaoTA;
	private TextField< String > andarTF;
	private NumberField capacidadeInstaladaNF;
	private NumberField capacidadeMaxNF;
	private NumberField custoOperacaoCredNF;
	private CheckBox externaCB;
	private UnidadeComboBox unidadeCB;
	private TipoSalaComboBox tipoSalaCB;
	private SalaDTO salaDTO;
	private CampusDTO campusDTO;
	private UnidadeDTO unidadeDTO;
	private TipoSalaDTO tipoSalaDTO;
	private CenarioDTO cenarioDTO;

	public SalaFormView( CenarioDTO cenarioDTO )
	{
		this( new SalaDTO(), null, null, null, cenarioDTO );
	}

	public SalaFormView( SalaDTO salaDTO, CampusDTO campusDTO,
		UnidadeDTO unidadeDTO, TipoSalaDTO tipoSalaDTO, CenarioDTO cenarioDTO )
	{
		this.salaDTO = salaDTO;
		this.campusDTO = campusDTO;
		this.unidadeDTO = unidadeDTO;
		this.tipoSalaDTO = tipoSalaDTO;
		this.cenarioDTO = cenarioDTO;

		initUI();
	}

	private void initUI()
	{
		String title = (salaDTO.getId() == null) ?
			( getI18nConstants().insercaoDe() + getI18nConstants().sala() ) :
			( getI18nConstants().edicaoDe() + getI18nConstants().sala() );

		simpleModal = new SimpleModal( title, Resources.DEFAULTS.sala16() );
		simpleModal.setHeight( 435 );
		simpleModal.setWidth( 343 );

		createForm();
		simpleModal.setContent( formPanel );
	}

	private void createForm()
	{
		FormData formData = new FormData( "-20" );
		formPanel = new FormPanel();
		formPanel.setHeaderVisible( false );
		formPanel.setLabelWidth(120);

		codigoTF = new UniqueTextField( cenarioDTO, UniqueDomain.SALA );
		codigoTF.setName( SalaDTO.PROPERTY_CODIGO );
		codigoTF.setValue( salaDTO.getCodigo() );
		codigoTF.setFieldLabel( getI18nConstants().codigo() );
		codigoTF.setAllowBlank( false );
		codigoTF.setMinLength( 1 );
		codigoTF.setMaxLength( 20 );
		codigoTF.setEmptyText( getI18nConstants().preenchaO() + getI18nConstants().codigo() );
		formPanel.add( codigoTF, formData );

		CampusComboBox campusCB = new CampusComboBox(cenarioDTO);
		campusCB.setValue( campusDTO );
		formPanel.add( campusCB, formData );

		unidadeCB = new UnidadeComboBox( campusCB );
		unidadeCB.setName( "unidade" );
		unidadeCB.setFieldLabel( getI18nConstants().unidade() );
		unidadeCB.setAllowBlank( false );
		unidadeCB.setValue( unidadeDTO );
		unidadeCB.setEmptyText( getI18nConstants().selecioneA() + getI18nConstants().unidade() );
		formPanel.add( unidadeCB, formData );

		tipoSalaCB = new TipoSalaComboBox( cenarioDTO );
		tipoSalaCB.setName( "tipoSala" );
		tipoSalaCB.setFieldLabel( getI18nConstants().tipo() );
		tipoSalaCB.setAllowBlank( false );
		tipoSalaCB.setValue( tipoSalaDTO );
		tipoSalaCB.setEmptyText( getI18nConstants().selecioneO() + getI18nConstants().tipo() );
		formPanel.add( tipoSalaCB, formData );

		numeroTF = new TextField< String >();
		numeroTF.setName( SalaDTO.PROPERTY_NUMERO );
		numeroTF.setValue( salaDTO.getNumero() );
		numeroTF.setFieldLabel( getI18nConstants().numero() );
		numeroTF.setAllowBlank( false );
		numeroTF.setMinLength( 1 );
		numeroTF.setMaxLength( 20 );
		numeroTF.setEmptyText( getI18nConstants().preenchaO() + getI18nConstants().numero() );
		formPanel.add( numeroTF, formData );

		descricaoTA = new TextArea();
		descricaoTA.setName( SalaDTO.PROPERTY_DESCRICAO );
		descricaoTA.setValue( salaDTO.getDescricao() );
		descricaoTA.setFieldLabel( getI18nConstants().descricao() );
		descricaoTA.setMaxLength( 255 );
		descricaoTA.setEmptyText( getI18nConstants().preenchaA() + getI18nConstants().descricao() );
		formPanel.add( descricaoTA, formData );

		andarTF = new TextField< String >();
		andarTF.setName( SalaDTO.PROPERTY_ANDAR );
		andarTF.setValue( salaDTO.getAndar() );
		andarTF.setFieldLabel( getI18nConstants().andar() );
		andarTF.setAllowBlank( false );
		andarTF.setMinLength( 1 );
		andarTF.setMaxLength( 20 );
		andarTF.setEmptyText( getI18nConstants().preenchaO() + getI18nConstants().andar() );
		formPanel.add( andarTF, formData );

		capacidadeInstaladaNF = new NumberField();
		capacidadeInstaladaNF.setName( SalaDTO.PROPERTY_CAPACIDADE_INSTALADA );
		capacidadeInstaladaNF.setValue( salaDTO.getCapacidadeInstalada() );
		capacidadeInstaladaNF.setFieldLabel( getI18nConstants().capacidadeInstalada() );
		capacidadeInstaladaNF.setAllowBlank( false );
		capacidadeInstaladaNF.setMinValue(1);
		capacidadeInstaladaNF.setMinLength( 1 );
		capacidadeInstaladaNF.setMaxLength( 20 );
		capacidadeInstaladaNF.setEmptyText( getI18nConstants().preenchaA() + getI18nConstants().capacidadeInstalada() );
		capacidadeInstaladaNF.setToolTip("É a capacidade do ambiente em número de alunos, em seu uso normal.");
		formPanel.add( capacidadeInstaladaNF, formData );
		
		capacidadeMaxNF = new NumberField();
		capacidadeMaxNF.setName( SalaDTO.PROPERTY_CAPACIDADE_MAX );
		capacidadeMaxNF.setValue( salaDTO.getCapacidadeInstalada() );
		capacidadeMaxNF.setFieldLabel( getI18nConstants().capacidadeMax() );
		capacidadeMaxNF.setAllowBlank( false );
		capacidadeMaxNF.setMinValue(1);
		capacidadeMaxNF.setMinLength( 1 );
		capacidadeMaxNF.setMaxLength( 20 );
		capacidadeMaxNF.setEmptyText( getI18nConstants().preenchaA() + getI18nConstants().capacidadeMax() );
		capacidadeMaxNF.setToolTip("É a capacidade máxima do ambiente, em número de alunos, em situações extremas.");
		formPanel.add( capacidadeMaxNF, formData );
		
		custoOperacaoCredNF = new NumberField();
		custoOperacaoCredNF.setName( SalaDTO.PROPERTY_CUSTO_OPERACAO_CRED );
		custoOperacaoCredNF.setValue( salaDTO.getCustoOperacaoCred() );
		custoOperacaoCredNF.setFieldLabel( getI18nConstants().custoOperacaoCred() );
		custoOperacaoCredNF.setAllowBlank( false );
		custoOperacaoCredNF.setAllowDecimals( true );
		custoOperacaoCredNF.setMaxValue( 999999 );
		custoOperacaoCredNF.setEmptyText( getI18nConstants().preenchaO() + getI18nConstants().custoOperacaoCred());
		custoOperacaoCredNF.setToolTip("Custo de uso do ambiente por tempo de aula. Pode considerar custo de aluguel, limpeza, energia elétrica, móveis e equipamentos instalados, etc.");
		formPanel.add( custoOperacaoCredNF, formData );
		
		externaCB = new CheckBox();
		externaCB.setName(SalaDTO.PROPERTY_EXTERNA);
		externaCB.setValue(salaDTO.getExterna());
		externaCB.setFieldLabel("Ambiente Externo?");
		formPanel.add( externaCB, formData );

		FormButtonBinding binding = new FormButtonBinding( formPanel );
		binding.addButton( simpleModal.getSalvarBt() );

		simpleModal.setFocusWidget( codigoTF );
	}

	public boolean isValid()
	{
		return formPanel.isValid();
	}

	@Override
	public Button getSalvarButton()
	{
		return simpleModal.getSalvarBt();
	}

	@Override
	public TextField< String > getCodigoTextField()
	{
		return codigoTF;
	}

	@Override
	public TextField< String > getNumeroTextField()
	{
		return numeroTF;
	}

	@Override
	public TextArea getDescricaoTextArea()
	{
		return descricaoTA;
	}

	@Override
	public TextField< String > getAndarTextField()
	{
		return andarTF;
	}

	@Override
	public NumberField getCapacidadeInstaladaNumberField()
	{
		return capacidadeInstaladaNF;
	}

	@Override
	public NumberField getCapacidadeMaxNumberField()
	{
		return capacidadeMaxNF;
	}
	
	@Override
	public NumberField getCustoOperacaoCredNumberField()
	{
		return custoOperacaoCredNF;
	}
	
	@Override
	public CheckBox getExternaCheckBox()
	{
		return externaCB;
	}
	
	@Override
	public UnidadeComboBox getUnidadeComboBox()
	{
		return unidadeCB;
	}

	@Override
	public TipoSalaComboBox getTipoComboBox()
	{
		return tipoSalaCB;
	}

	@Override
	public SalaDTO getSalaDTO()
	{
		return salaDTO;
	}

	@Override
	public SimpleModal getSimpleModal()
	{
		return simpleModal;
	}
}
