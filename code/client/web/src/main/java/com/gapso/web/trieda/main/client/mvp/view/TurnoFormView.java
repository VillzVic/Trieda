package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.TextField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.main.client.mvp.presenter.TurnoFormPresenter;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.SimpleModal;

public class TurnoFormView
	extends MyComposite
	implements TurnoFormPresenter.Display
{
	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private TextField< String > nomeTF;
	private TurnoDTO turnoDTO;

	public TurnoFormView( TurnoDTO turnoDTO )
	{
		this.turnoDTO = turnoDTO;
		this.initUI();
	}
	
	private void initUI()
	{
		String title = ( ( this.turnoDTO.getId() == null ) ?
			"Inserção de Turno" : "Edição de Turno" );

		this.simpleModal = new SimpleModal(
			title, Resources.DEFAULTS.turno16() );
		this.simpleModal.setHeight( 138 );
		createForm();

		this.simpleModal.setContent( this.formPanel );
	}

	private void createForm()
	{
		FormData formData = new FormData( "-20" );
		this.formPanel = new FormPanel();
		this.formPanel.setHeaderVisible( false );

		this.nomeTF = new TextField< String >();
		this.nomeTF.setName( TurnoDTO.PROPERTY_NOME );
		this.nomeTF.setValue( this.turnoDTO.getNome() );
		this.nomeTF.setFieldLabel( "Nome" );
		this.nomeTF.setAllowBlank( false );
		this.nomeTF.setMinLength( 1 );
		this.nomeTF.setMaxLength( 50 );
		this.nomeTF.setEmptyText( "Preencha o nome" );
		this.formPanel.add( this.nomeTF, formData );

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
	public TurnoDTO getTurnoDTO()
	{
		return this.turnoDTO;
	}
}
