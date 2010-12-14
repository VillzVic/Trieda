package com.gapso.web.trieda.client.mvp.presenter;

import com.extjs.gxt.ui.client.widget.Component;
import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.gapso.web.trieda.client.mvp.model.CenarioDTO;
import com.gapso.web.trieda.client.mvp.view.ToolBarView;
import com.gapso.web.trieda.client.services.CenariosServiceAsync;
import com.gapso.web.trieda.client.services.Services;
import com.gapso.web.trieda.client.util.view.CenarioPanel;
import com.gapso.web.trieda.client.util.view.GTab;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.RootPanel;
import com.google.gwt.user.client.ui.Widget;

public class AppPresenter implements Presenter {

	public interface Display {
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
		cenariosServer.getMasterData(new AsyncCallback<CenarioDTO>() {
			@Override
			public void onFailure(Throwable caught) {
				caught.printStackTrace();
			}
			@Override
			public void onSuccess(CenarioDTO masterData) {
				RootPanel rp = (RootPanel) widget;
				Presenter presenter = new ToolBarPresenter(masterData, viewport.getCenarioPanel(), new ToolBarView());
				presenter.go(viewport.asWidget());
				rp.add(viewport.asWidget());
			}
		});
		
	}

}
