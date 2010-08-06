package com.gapso.web.trieda.client.controller.mvc.controller;

import com.extjs.gxt.ui.client.event.EventType;
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.controller.mvc.view.CursoView;

public class CursoController extends Controller {

	private CursoView cursoView;
	
	public CursoController() {
		registerEventTypes(AppEvents.CursoList);
	}

	private void onList(AppEvent event) {
		cursoView = new CursoView(this);
		forwardToView(cursoView, event);
	}
	
	@Override
	public void handleEvent(AppEvent event) {
		EventType type = event.getType();
		if (type == AppEvents.CursoList) {
			onList(event);
		}
	}

}
