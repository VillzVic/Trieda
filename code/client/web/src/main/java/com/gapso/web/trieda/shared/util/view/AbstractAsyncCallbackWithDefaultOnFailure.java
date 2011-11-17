package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.widget.MessageBox;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.google.gwt.user.client.rpc.AsyncCallback;

public abstract class AbstractAsyncCallbackWithDefaultOnFailure< T >
	implements AsyncCallback< T >
{
	private ITriedaI18nGateway gateway;
	private String errorMessage;

	public AbstractAsyncCallbackWithDefaultOnFailure( ITriedaI18nGateway gateway )
	{
		this( gateway.getI18nMessages().falhaOperacao(), gateway );
	}

	public AbstractAsyncCallbackWithDefaultOnFailure(
		String errorMessage, ITriedaI18nGateway gateway )
	{
		this.gateway = gateway;
		this.errorMessage = errorMessage;
	}

	@Override
	public void onFailure( Throwable caught )
	{
		if ( caught != null )
		{
			TriedaDetailMessageBox.alert(
				this.gateway.getI18nConstants().mensagemErro(),
				this.errorMessage, getDetailErrorMessage( caught ) );
		}
		else
		{
			MessageBox.alert( this.gateway.getI18nConstants().mensagemErro(),
				this.errorMessage, null );
		}
	}

	private String getDetailErrorMessage( Throwable caught )
	{
		String detailMsg = "";

		if ( caught != null )
		{
			detailMsg = "Msg: " + caught.getMessage();

			if ( caught.getCause() != null )
			{
				detailMsg += "\nCause: " + caught.getMessage();
			}
		}

		return detailMsg;
	}
}
