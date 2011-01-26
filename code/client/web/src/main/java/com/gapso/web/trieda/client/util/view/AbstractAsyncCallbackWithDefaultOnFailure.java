package com.gapso.web.trieda.client.util.view;

import com.extjs.gxt.ui.client.widget.MessageBox;
import com.gapso.web.trieda.client.i18n.ITriedaI18nGateway;
import com.google.gwt.user.client.rpc.AsyncCallback;

public abstract class AbstractAsyncCallbackWithDefaultOnFailure<T> implements AsyncCallback<T> {
	
	private ITriedaI18nGateway gateway;
	private String errorMessage;
	
	public AbstractAsyncCallbackWithDefaultOnFailure(ITriedaI18nGateway gateway) {
		this(gateway.getI18nMessages().falhaOperacao(),gateway);
	}
	
	public AbstractAsyncCallbackWithDefaultOnFailure(String errorMessage, ITriedaI18nGateway gateway) {
		this.gateway = gateway;
		this.errorMessage = errorMessage;
	}

	@Override
	public void onFailure(Throwable caught) {
		if (caught != null) {
			TriedaDetailMessageBox.alert(gateway.getI18nConstants().mensagemErro(),errorMessage,getDetailErrorMessage(caught));
		} else {
			MessageBox.alert(gateway.getI18nConstants().mensagemErro(),errorMessage,null);
		}
	}

	private String getDetailErrorMessage(Throwable caught) {
		String detailMsg = "";
		if (caught != null) {
			detailMsg = "Msg: " + caught.getMessage();
			if (caught.getCause() != null) {
				detailMsg += "\nCause: " + caught.getMessage();
			}
		}
		return detailMsg;
	}
}