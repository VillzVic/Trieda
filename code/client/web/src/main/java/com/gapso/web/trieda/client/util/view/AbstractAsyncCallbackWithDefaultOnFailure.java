package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.widget.MessageBox;
import com.gapso.web.trieda.client.i18n.ITriedaI18nGateway;
import com.google.gwt.user.client.rpc.AsyncCallback;

public abstract class AbstractAsyncCallbackWithDefaultOnFailure<T> implements AsyncCallback<T>  {
	
	private ITriedaI18nGateway gateway;
	
	public AbstractAsyncCallbackWithDefaultOnFailure(ITriedaI18nGateway gateway) {
		this.gateway = gateway;
	}

	@Override
	public void onFailure(Throwable caught) {
		String errorMsg = "";
		if (caught != null) {
			errorMsg = "Msg: " + caught.getMessage();
			if (caught.getCause() != null) {
				errorMsg += "/n Cause: " + caught.getMessage();
			}
		}
		MessageBox.alert(
			gateway.getI18nConstants().mensagemErro(),
			gateway.getI18nMessages().falhaOperacaoBD(errorMsg),
			null
		);
	}
}