package com.gapso.web.trieda.main.client.mvp.view;

import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.extjs.gxt.ui.client.widget.button.Button;
import com.extjs.gxt.ui.client.widget.layout.RowLayout;
import com.gapso.web.trieda.main.client.mvp.presenter.ErrorsWarningsInputSolverPresenter;
import com.gapso.web.trieda.shared.mvp.view.MyComposite;
import com.gapso.web.trieda.shared.util.resources.Resources;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.ui.AbstractImagePrototype;

public class ErrorsWarningsInputSolverView
	extends MyComposite
	implements ErrorsWarningsInputSolverPresenter.Display
{
	private ContentPanel panel;
	private SimpleModal simpleModal;
	private ContentPanel messagesWarningPanel;
	private ContentPanel messagesErrorPanel;

	public ErrorsWarningsInputSolverView()
	{
		initUI();
	}

	private void initUI()
	{
		String title = "Alertas e Erros";

		this.simpleModal = new SimpleModal( "Gerar Grade",
			"Cancelar", title, Resources.DEFAULTS.serverWarning16() );

		this.simpleModal.setAutoHeight( true );
		this.panel = new ContentPanel( new RowLayout() );
		this.panel.setHeaderVisible( false );

		this.messagesWarningPanel = new ContentPanel();
		this.messagesWarningPanel.setHeading( "Alertas" );
		this.messagesWarningPanel.setIcon( AbstractImagePrototype.create(
			Resources.DEFAULTS.warning16() ) );
		this.messagesWarningPanel.setBodyBorder( false );
		this.messagesWarningPanel.addStyleName( "errorList" );
		this.panel.add( this.messagesWarningPanel );

		this.messagesErrorPanel = new ContentPanel();
		this.messagesErrorPanel.setHeading( "Erros" );
		this.messagesErrorPanel.setIcon( AbstractImagePrototype.create(
			Resources.DEFAULTS.error16() ) );
		this.messagesErrorPanel.setBodyBorder( false );
		this.messagesErrorPanel.addStyleName( "errorList" );
		this.panel.add( this.messagesErrorPanel );

		this.panel.setAutoHeight( true );

		this.simpleModal.setContent( this.panel );
		this.simpleModal.setWidth( 500 );
	}

	@Override
	public SimpleModal getSimpleModal()
	{
		return this.simpleModal;
	}

	@Override
	public ContentPanel getMessagesWarningPanel()
	{
		return this.messagesWarningPanel;
	}

	@Override
	public ContentPanel getMessagesErrorPanel()
	{
		return this.messagesErrorPanel;
	}

	@Override
	public Button getSubmitButton()
	{
		return this.simpleModal.getSalvarBt();
	}

	@Override
	public Button getCancelButton()
	{
		return this.simpleModal.getCancelarBt();
	}
}
