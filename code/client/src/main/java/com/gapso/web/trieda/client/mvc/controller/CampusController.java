package com.gapso.web.trieda.client.mvc.controller;

import com.extjs.gxt.ui.client.event.EventType;
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.mvc.view.CampusView;

public class CampusController extends Controller {

	private CampusView campusView;
	
	public CampusController() {
		registerEventTypes(AppEvents.CampusList);
	}

	private void onList(AppEvent event) {
		campusView = new CampusView(this);
		forwardToView(campusView, event);
	}
	
	@Override
	public void handleEvent(AppEvent event) {
		EventType type = event.getType();
		if (type == AppEvents.CampusList) {
			onList(event);
		}
	}

}
