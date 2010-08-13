package com.gapso.web.trieda.client.mvc.controller;

import com.extjs.gxt.ui.client.event.EventType;
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.mvc.view.CalendarioView;

public class CalendarioController extends Controller {

	private CalendarioView calendarioView;
	
	public CalendarioController() {
		registerEventTypes(AppEvents.CalendarioList);
	}

	private void onList(AppEvent event) {
		calendarioView = new CalendarioView(this);
		forwardToView(calendarioView, event);
	}
	
	@Override
	public void handleEvent(AppEvent event) {
		EventType type = event.getType();
		if (type == AppEvents.CalendarioList) {
			onList(event);
		}
	}

}
