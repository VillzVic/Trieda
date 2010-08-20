package com.gapso.web.trieda.client.mvp.presenter;

import com.extjs.gxt.ui.client.widget.button.Button;
import com.google.gwt.user.client.ui.HasWidgets;
import com.google.gwt.user.client.ui.Widget;

public class AppPresenter implements Presenter {

	public interface Display {
		Button getCampusListButton();
		Widget asWidget();
	}
	
	
	private Widget viewPort;
	
	public AppPresenter(Widget viewPort) {
		this.viewPort = viewPort;
	}

	@Override
	public void go(HasWidgets container) {
		container.add(viewPort);
	}

}
