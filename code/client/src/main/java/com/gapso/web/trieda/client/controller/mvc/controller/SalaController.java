package com.gapso.web.trieda.client.controller.mvc.controller;

import com.extjs.gxt.ui.client.event.EventType;
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.controller.mvc.view.SalaView;

public class SalaController extends Controller {

	private SalaView salaView;
	
	public SalaController() {
		registerEventTypes(AppEvents.SalaList);
	}

	private void onList(AppEvent event) {
		salaView = new SalaView(this);
		forwardToView(salaView, event);
	}
	
	@Override
	public void handleEvent(AppEvent event) {
		EventType type = event.getType();
		if (type == AppEvents.SalaList) {
			onList(event);
		}
	}

}
