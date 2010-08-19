package com.gapso.web.trieda.client.mvc.controller;

import com.extjs.gxt.ui.client.event.EventType;
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.mvc.view.AppView;

public class AppController extends Controller {

	private AppView appView;
	
	public AppController() {
		registerEventTypes(AppEvents.Init);
	}

	public void initialize() {
		appView = new AppView(this);
	}
	
	private void onInit(AppEvent event) {
		forwardToView(appView, event);
	}
	
	@Override
	public void handleEvent(AppEvent event) {
		EventType type = event.getType();
		if (type == AppEvents.Init) {
			onInit(event);
		}
	}

}
