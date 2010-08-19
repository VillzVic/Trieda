package com.gapso.web.trieda.client.mvc.controller;

import com.extjs.gxt.ui.client.event.EventType;
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.mvc.view.DisciplinaView;

public class DisciplinaController extends Controller {

	private DisciplinaView disciplinaView;
	
	public DisciplinaController() {
		registerEventTypes(AppEvents.DisciplinaList);
	}

	private void onList(AppEvent event) {
		disciplinaView = new DisciplinaView(this);
		forwardToView(disciplinaView, event);
	}
	
	@Override
	public void handleEvent(AppEvent event) {
		EventType type = event.getType();
		if (type == AppEvents.DisciplinaList) {
			onList(event);
		}
	}

}
