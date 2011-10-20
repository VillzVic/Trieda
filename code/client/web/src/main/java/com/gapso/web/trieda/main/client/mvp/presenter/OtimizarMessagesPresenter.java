package com.gapso.web.trieda.main.client.mvp.presenter;

import java.util.List;

import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.ui.Widget;

public class OtimizarMessagesPresenter
	implements Presenter
{
	public interface Display
	{
		ContentPanel getMessagesWarningPanel();
		ContentPanel getMessagesErrorPanel();
		SimpleModal getSimpleModal();
	}

	private List< String > warnings;
	private List< String > errors;
	private Display display;

	public OtimizarMessagesPresenter(
		List< String > warnings,
		List< String > errors, Display display )
	{
		this.display = display;
		this.warnings = warnings;
		this.errors = errors;

		populationMessages();
	}

	private void populationMessages()
	{
		for ( String msg : this.warnings )
		{
			this.display.getMessagesWarningPanel().addText( "• " + msg );
		}

		for ( String msg : this.errors )
		{
			this.display.getMessagesErrorPanel().addText( "• " + msg );
		}

		this.display.getMessagesWarningPanel().setVisible( !this.warnings.isEmpty() );
		this.display.getMessagesErrorPanel().setVisible( !this.errors.isEmpty() );
	}

	@Override
	public void go( Widget widget )
	{
		this.display.getSimpleModal().show();
	}
}
