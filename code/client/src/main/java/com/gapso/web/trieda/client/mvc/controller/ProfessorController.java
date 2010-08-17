package com.gapso.web.trieda.client.mvc.controller;

import com.extjs.gxt.ui.client.event.EventType;
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.mvc.view.ProfessorDisciplinaView;
import com.gapso.web.trieda.client.mvc.view.ProfessorView;

public class ProfessorController extends Controller {

	public ProfessorController() {
		registerEventTypes(AppEvents.ProfessorList);
		registerEventTypes(AppEvents.ProfessorDisciplinaList);
		registerEventTypes(AppEvents.ProfessorDisciplinaView);
	}

	private void onList(AppEvent event) {
		ProfessorView professorView = new ProfessorView(this);
		forwardToView(professorView, event);
	}
	
	private void onProfessoresDisciplinasList(AppEvent event) {
		ProfessorDisciplinaView professorView = new ProfessorDisciplinaView(this);
		forwardToView(professorView, event);
	}
	
	private void onProfessoresDisciplinasView(AppEvent event) {
		ProfessorDisciplinaView professorView = new ProfessorDisciplinaView(this);
		forwardToView(professorView, event);
	}
	
	@Override
	public void handleEvent(AppEvent event) {
		EventType type = event.getType();
		if (type == AppEvents.ProfessorList) onList(event);
		if (type == AppEvents.ProfessorDisciplinaList) onProfessoresDisciplinasList(event);
		if (type == AppEvents.ProfessorDisciplinaView) onProfessoresDisciplinasView(event);
	}

}
