package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.form.FormPanel;
import com.extjs.gxt.ui.client.widget.form.FormPanel.LabelAlign;
import com.extjs.gxt.ui.client.widget.form.TextArea;
import com.extjs.gxt.ui.client.widget.layout.FormData;
import com.gapso.web.trieda.main.client.mvp.presenter.MotivosNaoAtendimentoPresenter;
import com.gapso.web.trieda.shared.dtos.AlunoDemandaDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.SimpleModal;

public class MotivosNaoAtendimentoView
	extends MyComposite
	implements MotivosNaoAtendimentoPresenter.Display
{
	private SimpleModal simpleModal;
	private FormPanel formPanel;
	private AlunoDemandaDTO alunoDemandaDTO;
	private TextArea motivos;
	private CenarioDTO cenarioDTO;

	public MotivosNaoAtendimentoView( CenarioDTO cenarioDTO,
		AlunoDemandaDTO alunoDemandaDTO )
	{
		this.cenarioDTO = cenarioDTO;
		this.alunoDemandaDTO = alunoDemandaDTO;
		this.initUI();
	}

	private void initUI()
	{
		String title = "Motivo de Não Atendimento do Aluno: " + alunoDemandaDTO.getAlunoString();
		this.simpleModal = new SimpleModal( title,
			Resources.DEFAULTS.alunoCurriculo16() );

		this.simpleModal.setHeight( 280 );
		this.simpleModal.setWidth( 500 );
		createForm();
		this.simpleModal.setContent( this.formPanel );
	}

	private void createForm()
	{
		FormData formData = new FormData( "-20" );
		this.formPanel = new FormPanel();
		this.formPanel.setHeaderVisible( false );

		// Campus
		motivos = new TextArea();
		motivos.setValue( this.alunoDemandaDTO.getMotivoNaoAtendimento() );
		motivos.setFieldLabel( "Motivos para não atendimento" );
		motivos.setReadOnly( true );
		motivos.setHeight(170);
		formPanel.setLabelAlign(LabelAlign.TOP);
		this.formPanel.add( motivos, formData );

		this.simpleModal.getCancelarBt().setText("Fechar");
		this.simpleModal.getSalvarBt().hide();

		this.simpleModal.setFocusWidget( this.motivos );
	}

	public boolean isValid()
	{
		return this.formPanel.isValid();
	}
	
	@Override
	public SimpleModal getSimpleModal()
	{
		return this.simpleModal;
	}
}
