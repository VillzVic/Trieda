package com.gapso.web.trieda.client.mvc.controller;

import com.extjs.gxt.ui.client.event.EventType;
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.mvc.view.SalaDisciplinaView;

public class SalaController extends Controller {
	
	public SalaController() {
		registerEventTypes(AppEvents.SalaDisciplinaList);
	}
	
	private void onSalaDisciplinaList(AppEvent event) {
		SalaDisciplinaView salaDisciplinaView = new SalaDisciplinaView(this);
		forwardToView(salaDisciplinaView, event);
	}
	
	@Override
	public void handleEvent(AppEvent event) {
		EventType type = event.getType();
		if (type == AppEvents.SalaDisciplinaList) onSalaDisciplinaList(event);
	}

}
