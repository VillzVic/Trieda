package com.gapso.web.trieda.client.mvc.controller;

import com.extjs.gxt.ui.client.event.EventType;
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.mvc.view.CampusProfessorView;
import com.gapso.web.trieda.client.mvc.view.CampusView;

public class CampusController extends Controller {

	public CampusController() {
		registerEventTypes(AppEvents.CampusList);
		registerEventTypes(AppEvents.CampusProfessorList);
		registerEventTypes(AppEvents.CampusProfessorView);
	}

	private void onList(AppEvent event) {
		CampusView campusView = new CampusView(this);
		forwardToView(campusView, event);
	}
	
	private void onProfessoresList(AppEvent event) {
		CampusProfessorView view = new CampusProfessorView(this);
		forwardToView(view, event);
	}
	
	private void onProfessoresView(AppEvent event) {
		CampusProfessorView view = new CampusProfessorView(this);
		forwardToView(view, event);
	}
	
	@Override
	public void handleEvent(AppEvent event) {
		EventType type = event.getType();
		if (type == AppEvents.CampusList) onList(event);
		if (type == AppEvents.CampusProfessorList) onProfessoresList(event);
		if (type == AppEvents.CampusProfessorView) onProfessoresView(event);
	}

}
