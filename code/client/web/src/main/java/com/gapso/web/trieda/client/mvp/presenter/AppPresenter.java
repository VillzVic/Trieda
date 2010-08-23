package com.gapso.web.trieda.client.mvp.presenter;

import com.extjs.gxt.ui.client.widget.ContentPanel;
import com.gapso.web.trieda.client.mvp.view.ToolBarView;
import com.google.gwt.user.client.ui.RootPanel;
import com.google.gwt.user.client.ui.Widget;

public class AppPresenter implements Presenter {

	public interface Display {
		ContentPanel getPanel();
		Widget asWidget();
	}
	
	private Display viewport;
	
	public AppPresenter(Display viewport) {
		this.viewport = viewport;
	}

	@Override
	public void go(Widget widget) {
		RootPanel rp = (RootPanel) widget;
		Presenter presenter = new ToolBarPresenter(new ToolBarView());
		presenter.go(viewport.getPanel());
		rp.add(viewport.asWidget());
	}

}
