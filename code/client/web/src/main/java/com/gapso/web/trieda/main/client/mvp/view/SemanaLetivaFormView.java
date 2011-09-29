package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
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

		initUI();
	}

	private void initUI()
	{
		String title = ( ( semanaLetivaDTO.getId() == null ) ?
			"Inserção de Semana Letiva" : "Edição de Semana Letiva" );

		simpleModal = new SimpleModal( title, Resources.DEFAULTS.semanaLetiva16() );
		simpleModal.setHeight( 158 );
		createForm();
		simpleModal.setContent( formPanel );
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
		this.codigoTF.setFieldLabel( "Codigo" );
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
}
