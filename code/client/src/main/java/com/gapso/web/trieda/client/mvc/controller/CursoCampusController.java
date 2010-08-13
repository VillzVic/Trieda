package com.gapso.web.trieda.client.mvc.controller;

import com.extjs.gxt.ui.client.event.EventType;
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.mvc.view.CursoCampusView;

public class CursoCampusController extends Controller {

	private CursoCampusView cursoCampusView;
	
	public CursoCampusController() {
		registerEventTypes(AppEvents.CursoCampusList);
	}

	private void onList(AppEvent event) {
		cursoCampusView = new CursoCampusView(this);
		forwardToView(cursoCampusView, event);
	}
	
	@Override
	public void handleEvent(AppEvent event) {
		EventType type = event.getType();
		if (type == AppEvents.CursoCampusList) {
			onList(event);
		}
	}

}
