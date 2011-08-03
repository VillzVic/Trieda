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

	public OtimizarMessagesPresenter( List< String > warnings,
		List< String > errors, Display display )
	{
		this.display = display;
		this.warnings = warnings;
		this.errors = errors;
		populationMessages();
	}

	private void populationMessages()
	{
		for ( String msg : warnings )
		{
			display.getMessagesWarningPanel().addText( "• " + msg );
		}

		for ( String msg : errors )
		{
			display.getMessagesErrorPanel().addText( "• " + msg );
		}

		display.getMessagesWarningPanel().setVisible( !warnings.isEmpty() );
		display.getMessagesErrorPanel().setVisible( !errors.isEmpty() );
	}

	@Override
	public void go( Widget widget )
	{
		display.getSimpleModal().show();
	}
}
