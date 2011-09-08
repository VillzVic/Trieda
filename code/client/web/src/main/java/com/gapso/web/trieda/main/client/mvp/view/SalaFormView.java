package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
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
	private TextField< String > andarTF;
	private NumberField capacidadeNF;
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
		simpleModal.setHeight( 265 );

		createForm();
		simpleModal.setContent( formPanel );
	}

	private void createForm()
	{
		FormData formData = new FormData( "-20" );
		formPanel = new FormPanel();
		formPanel.setHeaderVisible( false );

		codigoTF = new UniqueTextField( cenarioDTO, UniqueDomain.SALA );
		codigoTF.setName( SalaDTO.PROPERTY_CODIGO );
		codigoTF.setValue( salaDTO.getCodigo() );
		codigoTF.setFieldLabel( getI18nConstants().codigo() );
		codigoTF.setAllowBlank( false );
		codigoTF.setMinLength( 1 );
		codigoTF.setMaxLength( 20 );
		codigoTF.setEmptyText( getI18nConstants().preenchaO() + getI18nConstants().codigo() );
		formPanel.add( codigoTF, formData );

		CampusComboBox campusCB = new CampusComboBox();
		campusCB.setValue( campusDTO );
		formPanel.add( campusCB, formData );

		unidadeCB = new UnidadeComboBox( campusCB );
		unidadeCB.setName( "unidade" );
		unidadeCB.setFieldLabel( getI18nConstants().unidade() );
		unidadeCB.setAllowBlank( false );
		unidadeCB.setValue( unidadeDTO );
		unidadeCB.setEmptyText( getI18nConstants().selecioneA() + getI18nConstants().unidade() );
		formPanel.add( unidadeCB, formData );

		tipoSalaCB = new TipoSalaComboBox();
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

		andarTF = new TextField< String >();
		andarTF.setName( SalaDTO.PROPERTY_ANDAR );
		andarTF.setValue( salaDTO.getAndar() );
		andarTF.setFieldLabel( getI18nConstants().andar() );
		andarTF.setAllowBlank( false );
		andarTF.setMinLength( 1 );
		andarTF.setMaxLength( 20 );
		andarTF.setEmptyText( getI18nConstants().preenchaO() + getI18nConstants().andar() );
		formPanel.add( andarTF, formData );

		capacidadeNF = new NumberField();
		capacidadeNF.setName( SalaDTO.PROPERTY_CAPACIDADE );
		capacidadeNF.setValue( salaDTO.getCapacidade() );
		capacidadeNF.setFieldLabel( getI18nConstants().capacidade() );
		capacidadeNF.setAllowBlank( false );
		capacidadeNF.setMinLength( 1 );
		capacidadeNF.setMaxLength( 20 );
		capacidadeNF.setEmptyText( getI18nConstants().preenchaA() + getI18nConstants().capacidade() );
		formPanel.add( capacidadeNF, formData );

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
	public TextField< String > getAndarTextField()
	{
		return andarTF;
	}

	@Override
	public NumberField getCapacidadeNumberField()
	{
		return capacidadeNF;
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
