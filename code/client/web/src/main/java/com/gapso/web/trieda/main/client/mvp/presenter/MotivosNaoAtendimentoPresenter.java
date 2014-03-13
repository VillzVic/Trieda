package com.gapso.web.trieda.main.client.mvp.presenter;

import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.util.view.SimpleModal;
import com.google.gwt.user.client.ui.Widget;

public class MotivosNaoAtendimentoPresenter
	implements Presenter
{
	public interface Display extends ITriedaI18nGateway
	{
		SimpleModal getSimpleModal();
	}
	
	private Display display;
	
	public MotivosNaoAtendimentoPresenter(
			Display display )
	{
		this.display = display;

	}
	
	@Override
	public void go( Widget widget )
	{
		this.display.getSimpleModal().show();
	}
}
