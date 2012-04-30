package com.gapso.web.trieda.main.client.mvp.view.gateways;

import java.util.List;

import com.extjs.gxt.ui.client.event.Listener;
import com.extjs.gxt.ui.client.event.MessageBoxEvent;
import com.extjs.gxt.ui.client.widget.MessageBox;
import com.gapso.web.trieda.main.client.mvp.presenter.OtimizarMessagesPresenter;
import com.gapso.web.trieda.main.client.mvp.presenter.RequisicoesOtimizacaoPresenter;
import com.gapso.web.trieda.main.client.mvp.view.OtimizarMessagesView;
import com.gapso.web.trieda.main.client.mvp.view.RequisicoesOtimizacaoView;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;

public class GenericViewGatewayImp implements IGenericViewGateway {

	@Override
	public void showMessageBoxAlert(String title, String msg, Listener<MessageBoxEvent> callback) {
		MessageBox.alert(title,msg,callback);
	}

	@Override
	public void showOtimizarMessagesView(List<String> warnings, List<String> errors) {
		Presenter presenter = new OtimizarMessagesPresenter(warnings,errors,new OtimizarMessagesView());
		presenter.go(null);
	}
	
	@Override
	public void showRequisicoesOtimizacaoView(boolean showEmpty) {
		RequisicoesOtimizacaoView view = new RequisicoesOtimizacaoView(showEmpty,new RequisicoesOtimizacaoPresenter());
		view.show();
	}
}