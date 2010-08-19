package com.gapso.web.trieda.client.mvc.controller;

import com.extjs.gxt.ui.client.event.EventType;
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.mvc.view.EquivalenciaView;

public class EquivalenciaController extends Controller {

	public EquivalenciaController() {
		registerEventTypes(AppEvents.EquivalenciaList);
	}

	private void onList(AppEvent event) {
		EquivalenciaView view = new EquivalenciaView(this);
		forwardToView(view, event);
	}
	
	@Override
	public void handleEvent(AppEvent event) {
		EventType type = event.getType();
		if (type == AppEvents.EquivalenciaList) onList(event);
	}

}
