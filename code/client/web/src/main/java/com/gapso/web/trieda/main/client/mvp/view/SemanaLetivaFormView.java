package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.main.client.mvp.presenter.SemanaLetivaFormPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.gapso.web.trieda.shared.util.view.UniqueDomain;
import com.gapso.web.trieda.shared.util.view.UniqueTextField;

public class SemanaLetivaFormView
	extends MyComposite
	implements SemanaLetivaFormPresenter.Display
{
	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private TextField< String > codigoTF;
	private TextField< String > descricaoTF;
	private NumberField tempoNF;
	private CheckBox permiteIntervaloCB;
	private SemanaLetivaDTO semanaLetivaDTO;
	private CenarioDTO cenarioDTO;

	public SemanaLetivaFormView( CenarioDTO cenarioDTO )
	{
		this( new SemanaLetivaDTO(), cenarioDTO );
	}

	public SemanaLetivaFormView(
		SemanaLetivaDTO semanaLetivaDTO, CenarioDTO cenarioDTO )
	{
		this.semanaLetivaDTO = semanaLetivaDTO;
		this.cenarioDTO = cenarioDTO;

		this.initUI();
	}

	private void initUI()
	{
		String title = ( ( this.semanaLetivaDTO.getId() == null ) ?
			"Inserção de Semana Letiva" : "Edição de Semana Letiva" );

		this.simpleModal = new SimpleModal(
			title, Resources.DEFAULTS.semanaLetiva16() );
		this.simpleModal.setHeight( 230 );
		this.simpleModal.setWidth( 300 );
		this.createForm();

		this.simpleModal.setContent( this.formPanel );
	}

	private void createForm()
	{
		FormData formData = new FormData( "-20" );
		this.formPanel = new FormPanel();
		this.formPanel.setHeaderVisible( false );

		this.codigoTF = new UniqueTextField(
			this.cenarioDTO, UniqueDomain.SEMANA_LETIVA );
		this.codigoTF.setName( SemanaLetivaDTO.PROPERTY_CODIGO );
		this.codigoTF.setValue( this.semanaLetivaDTO.getCodigo() );
		this.codigoTF.setFieldLabel( "Código" );
		this.codigoTF.setAllowBlank( false );
		this.codigoTF.setMinLength( 1 );
		this.codigoTF.setMaxLength( 20 );
		this.codigoTF.setEmptyText( "Preencha o código" );
		this.formPanel.add( this.codigoTF, formData );

		this.descricaoTF = new TextField< String >();
		this.descricaoTF.setName( SemanaLetivaDTO.PROPERTY_DESCRICAO );
		this.descricaoTF.setValue( this.semanaLetivaDTO.getDescricao() );
		this.descricaoTF.setFieldLabel( "Descrição" );
		this.descricaoTF.setAllowBlank( false );
		this.descricaoTF.setMaxLength( 50 );
		this.descricaoTF.setEmptyText( "Preencha uma descrição" );
		this.formPanel.add( this.descricaoTF, formData );

		this.tempoNF = new NumberField();
		this.tempoNF.setName( SemanaLetivaDTO.PROPERTY_TEMPO );
		this.tempoNF.setValue( this.semanaLetivaDTO.getTempo() );
		this.tempoNF.setFieldLabel( "Duração das Aulas (min)" );
		this.tempoNF.setAllowBlank( false );
		this.tempoNF.setAllowDecimals( false );
		this.tempoNF.setAllowNegative( false );
		this.tempoNF.setMinValue(1);
		this.tempoNF.setMaxValue( 1000 );
		this.tempoNF.setEmptyText( "Preencha a duração de aula da semana letiva" );
		this.formPanel.add( this.tempoNF, formData );
		
		this.permiteIntervaloCB = new CheckBox();
		this.permiteIntervaloCB.setName( SemanaLetivaDTO.PROPERTY_PERMITE_INTERVALO_AULA );
		this.permiteIntervaloCB.setValue( this.semanaLetivaDTO.getPermiteIntervaloAula() );
		this.permiteIntervaloCB.setFieldLabel("Permite Intervalo entre Aulas");
		this.permiteIntervaloCB.setToolTip("Quando o Trieda cria uma aula de dois tempos consecutivos (ou dois créditos), " +
				"por padrão, não é permitido que haja um intervalo posicionado entre estes dois tempos de aula. No entanto, caso o parâmetro \"Permite Intervalo entre Aulas\"" +
				" esteja marcado para a semana letiva em questão, então, o Trieda poderá criar aulas de dois tempos com um intervalo entre os tempos da aula.");
		formPanel.add( this.permiteIntervaloCB, formData );

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
	public TextField< String > getCodigoTextField()
	{
		return this.codigoTF;
	}

	@Override
	public SimpleModal getSimpleModal()
	{
		return this.simpleModal;
	}

	@Override
	public TextField< String > getDescricaoTextField()
	{
		return this.descricaoTF;
	}

	@Override
	public SemanaLetivaDTO getSemanaLetivaDTO()
	{
		return this.semanaLetivaDTO;
	}

	@Override
	public NumberField getTempoAulaNumberField()
	{
		return this.tempoNF;
	}
	
	@Override
	public CheckBox getPermiteIntervaloCheckBox()
	{
		return this.permiteIntervaloCB;
	}
}
