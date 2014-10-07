package com.gapso.web.trieda.shared.util.view;

import com.extjs.gxt.ui.client.widget.MessageBox;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.google.gwt.user.client.rpc.AsyncCallback;

public abstract class AbstractAsyncCallbackWithDefaultOnFailure< T >
	implements AsyncCallback< T >
{
	private ITriedaI18nGateway gateway;
	private String errorMessage;
	private boolean useDetailedMessageFromCaught;

	public AbstractAsyncCallbackWithDefaultOnFailure( ITriedaI18nGateway gateway ) {
		this( gateway.getI18nMessages().falhaOperacao(), gateway );
	}
	
	public AbstractAsyncCallbackWithDefaultOnFailure(boolean useDetailedMessageFromCaught, ITriedaI18nGateway gateway ) {
		this( "", gateway );
		this.useDetailedMessageFromCaught = useDetailedMessageFromCaught;
	}

	public AbstractAsyncCallbackWithDefaultOnFailure(String errorMessage, ITriedaI18nGateway gateway ) {
		this.gateway = gateway;
		this.errorMessage = errorMessage;
		this.useDetailedMessageFromCaught = false;
	}

	@Override
	public void onFailure( Throwable caught )
	{
		if ( caught != null )
		{
			if (this.useDetailedMessageFromCaught) {
				this.errorMessage = caught.getMessage();
			}
			
			TriedaDetailMessageBox.alert(
				this.gateway.getI18nConstants().mensagemErro(),
				this.errorMessage, caught );
		}
		else
		{
			MessageBox.alert( this.gateway.getI18nConstants().mensagemErro(),
				this.errorMessage, null );
		}
	}
}
