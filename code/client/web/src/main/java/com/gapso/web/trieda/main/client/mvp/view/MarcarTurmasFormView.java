package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.form.FormButtonBinding;
import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.NumberField;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.main.client.mvp.presenter.MarcarTurmasFormPresenter;
import com.gapso.web.trieda.shared.dtos.ConfirmacaoTurmaDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.SimpleModal;

public class MarcarTurmasFormView
	extends MyComposite
	implements MarcarTurmasFormPresenter.Display
{
	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private NumberField qtdeAlunosNF;

	public MarcarTurmasFormView()
	{
		this.initUI();
	}
	
	private void initUI()
	{
		String title = "Marcar Turmas com Mais de X Alunos";

		this.simpleModal = new SimpleModal(
			title, Resources.DEFAULTS.confirmarQtdeAlunos16() );
		this.simpleModal.setHeight( 120 );
		createForm();

		this.simpleModal.setContent( this.formPanel );
	}

	private void createForm()
	{
		FormData formData = new FormData( "-20" );
		this.formPanel = new FormPanel();
		this.formPanel.setHeaderVisible( false );

		this.qtdeAlunosNF = new NumberField();
		this.qtdeAlunosNF.setName( ConfirmacaoTurmaDTO.PROPERTY_QUANTIDADE_ALUNOS );
		this.qtdeAlunosNF.setFieldLabel( "Qtde. de Alunos" );
		this.qtdeAlunosNF.setAllowBlank( false );
		this.qtdeAlunosNF.setMinValue(1);
		this.qtdeAlunosNF.setAllowDecimals(false);
		this.qtdeAlunosNF.setEmptyText( "Quantidade de alunos na turma" );
		this.formPanel.add( this.qtdeAlunosNF, formData );

		FormButtonBinding binding = new FormButtonBinding( this.formPanel );
		binding.addButton( this.simpleModal.getSalvarBt() );

		this.simpleModal.setFocusWidget( this.qtdeAlunosNF );
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
	public NumberField getQtdeAlunosNumberField()
	{
		return this.qtdeAlunosNF;
	}

	@Override
	public SimpleModal getSimpleModal()
	{
		return this.simpleModal;
	}
}
