package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.widget.Component;
import com.gapso.web.trieda.main.client.mvp.presenter.AlunosPresenter.Display;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.gapso.web.trieda.shared.util.view.GTab;
import com.gapso.web.trieda.shared.util.view.GTabItem;
import com.google.gwt.user.client.ui.Widget;

public class AlocacaoManualPresenter 
	implements Presenter
{

	private Display display;
	
	public interface Display extends ITriedaI18nGateway {
		Component getComponent();
	}
	
	
	
	public AlocacaoManualPresenter(
		InstituicaoEnsinoDTO instituicaoEnsinoDTO,
		CenarioDTO cenarioDTO, Display display )
	{
		this.display = display;
	}
	
	@Override
	public void go( Widget widget )
	{
		GTab tab = (GTab) widget;
		tab.add( (GTabItem) this.display.getComponent() );
	}
}
