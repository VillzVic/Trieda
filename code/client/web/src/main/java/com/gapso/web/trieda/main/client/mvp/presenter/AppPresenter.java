package com.gapso.web.trieda.main.client.mvp.presenter;

import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.gapso.web.trieda.main.client.mvp.view.ToolBarView;
import com.gapso.web.trieda.main.client.services.CenariosServiceAsync;
import com.gapso.web.trieda.main.client.services.Services;
import com.gapso.web.trieda.main.client.util.view.AbstractAsyncCallbackWithDefaultOnFailure;
import com.gapso.web.trieda.main.client.util.view.CenarioPanel;
import com.gapso.web.trieda.main.client.util.view.GTab;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.i18n.ITriedaI18nGateway;
import com.gapso.web.trieda.shared.mvp.presenter.Presenter;
import com.google.gwt.user.client.ui.RootPanel;
import com.google.gwt.user.client.ui.Widget;

public class AppPresenter implements Presenter {

	public interface Display extends ITriedaI18nGateway {
		ContentPanel getPanel();
		GTab getGTab();
		Widget asWidget();
		Component getComponent();
		CenarioPanel getCenarioPanel();
	}
	
	private Display viewport;
	
	public AppPresenter(Display viewport) {
		this.viewport = viewport;
	}

	@Override
	public void go(final Widget widget) {
		
		final CenariosServiceAsync cenariosServer = Services.cenarios();
		cenariosServer.getMasterData(new AbstractAsyncCallbackWithDefaultOnFailure<CenarioDTO>(viewport) {
			@Override
			public void onSuccess(CenarioDTO masterData) {
				RootPanel rp = (RootPanel) widget;
				Presenter presenter = new ToolBarPresenter(masterData, viewport.getCenarioPanel(), new ToolBarView());
				presenter.go(viewport.asWidget());
				rp.add(viewport.asWidget());
				RootPanel.get("loading").setVisible(false);
			}
		});
		
	}

}
