package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.CheckBox;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.extjs.gxt.ui.client.widget.layout.FormLayout;
import com.gapso.web.trieda.main.client.mvp.presenter.CenarioCloneFormPresenter;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.SimpleModal;

public class CenarioCloneFormView
	extends MyComposite
	implements CenarioCloneFormPresenter.Display
{
	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private CheckBox oficialCB;
	private TextField< String > nomeTF;
	private NumberField anoTF;
	private NumberField semestreTF;
	private TextField< String > comentarioTF;
	private CenarioDTO cenarioDTO;

	public CenarioCloneFormView( CenarioDTO cenarioDTO )
	{
		this.cenarioDTO = cenarioDTO;

		initUI();
	}

	private void initUI()
	{
		String title = "Clonar Cenário";
		this.simpleModal = new SimpleModal(
			title, Resources.DEFAULTS.cenarioClonar16() );

		this.simpleModal.setHeight( 220 );
		createForm();
		this.simpleModal.setContent( this.formPanel );
	}

	private void createForm()
	{
		FormData formData = new FormData( "-20" );

		FormLayout formLayout = new FormLayout( LabelAlign.RIGHT );
		formLayout.setLabelWidth( 75 );

		this.formPanel = new FormPanel();
		this.formPanel.setHeaderVisible( false );
		this.formPanel.setLayout( formLayout );

		this.nomeTF = new TextField< String >();
		this.nomeTF.setName( CenarioDTO.PROPERTY_NOME );
		this.nomeTF.setValue( this.cenarioDTO.getNome() );
		this.nomeTF.setFieldLabel("Nome");
		this.nomeTF.setAllowBlank( false );
		this.nomeTF.setMinLength( 1 );
		this.nomeTF.setMaxLength( 50 );
		this.nomeTF.setEmptyText( "Preencha o nome" );
		this.formPanel.add( this.nomeTF, formData );

		this.oficialCB = new CheckBox();
		this.oficialCB.setName( CenarioDTO.PROPERTY_OFICIAL);
		this.oficialCB.setValue( this.cenarioDTO.getOficial() );
		this.oficialCB.setFieldLabel( "Oficial" );
		this.formPanel.add( this.oficialCB, formData );

		this.anoTF = new NumberField();
		this.anoTF.setName( CenarioDTO.PROPERTY_ANO );
		this.anoTF.setValue( this.cenarioDTO.getAno() );
		this.anoTF.setFieldLabel( "Ano" );
		this.anoTF.setAllowDecimals( false );
		this.anoTF.setAllowBlank( false );
		this.anoTF.setMinValue( 1 );
		this.anoTF.setMaxValue( 9999 );
		this.anoTF.setEmptyText( "Preencha o ano letívo" );
		this.formPanel.add( this.anoTF, formData );

		this.semestreTF = new NumberField();
		this.semestreTF.setName( CenarioDTO.PROPERTY_SEMESTRE );
		this.semestreTF.setValue( this.cenarioDTO.getSemestre() );
		this.semestreTF.setFieldLabel( "Semestre" );
		this.semestreTF.setAllowDecimals( false );
		this.semestreTF.setAllowBlank( false );
		this.semestreTF.setMinValue( 1 );
		this.semestreTF.setMaxValue( 12 );
		this.semestreTF.setEmptyText( "Preencha semestre letívo" );
		this.formPanel.add( this.semestreTF, formData );

		this.comentarioTF = new TextField< String >();
		this.comentarioTF.setName( CenarioDTO.PROPERTY_COMENTARIO );
		this.comentarioTF.setValue( this.cenarioDTO.getComentario() );
		this.comentarioTF.setFieldLabel( "Comentário" );
		this.comentarioTF.setMaxLength( 255 );
		this.comentarioTF.setEmptyText( "Preencha um comentário" );
		this.formPanel.add( this.comentarioTF, formData );

		FormButtonBinding binding = new FormButtonBinding( this.formPanel );
		binding.addButton( this.simpleModal.getSalvarBt() );

		this.simpleModal.setFocusWidget( this.nomeTF );
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
	public CheckBox getOficialCheckBox()
	{
		return this.oficialCB;
	}

	@Override
	public TextField< String > getNomeTextField()
	{
		return this.nomeTF;
	}

	@Override
	public NumberField getAnoTextField()
	{
		return this.anoTF;
	}

	@Override
	public NumberField getSemestreTextField()
	{
		return this.semestreTF;
	}

	@Override
	public TextField< String > getComentarioTextField()
	{
		return this.comentarioTF;
	}

	@Override
	public CenarioDTO getCenarioDTO()
	{
		return this.cenarioDTO;
	}
}
