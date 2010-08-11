package com.gapso.web.trieda.client.controller.mvc.controller;

import com.extjs.gxt.ui.client.event.EventType;
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.controller.mvc.view.TurnoView;

public class TurnoController extends Controller {

	private TurnoView turnoView;
	
	public TurnoController() {
		registerEventTypes(AppEvents.TurnoList);
	}

	private void onList(AppEvent event) {
		turnoView = new TurnoView(this);
		forwardToView(turnoView, event);
	}
	
	@Override
	public void handleEvent(AppEvent event) {
		EventType type = event.getType();
		if (type == AppEvents.TurnoList) {
			onList(event);
		}
	}

}
