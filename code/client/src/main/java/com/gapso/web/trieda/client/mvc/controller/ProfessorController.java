package com.gapso.web.trieda.client.mvc.controller;

import com.extjs.gxt.ui.client.event.EventType;
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.mvc.view.ProfessorView;

public class ProfessorController extends Controller {

	private ProfessorView professorView;
	
	public ProfessorController() {
		registerEventTypes(AppEvents.ProfessorList);
	}

	private void onList(AppEvent event) {
		professorView = new ProfessorView(this);
		forwardToView(professorView, event);
	}
	
	@Override
	public void handleEvent(AppEvent event) {
		EventType type = event.getType();
		if (type == AppEvents.ProfessorList) {
			onList(event);
		}
	}

}
