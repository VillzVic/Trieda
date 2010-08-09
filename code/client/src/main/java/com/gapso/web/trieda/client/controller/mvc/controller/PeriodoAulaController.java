package com.gapso.web.trieda.client.controller.mvc.controller;

import com.extjs.gxt.ui.client.event.EventType;
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.controller.mvc.view.PeriodoAulaView;

public class PeriodoAulaController extends Controller {

	private PeriodoAulaView periodoDeAulaView;
	
	public PeriodoAulaController() {
		registerEventTypes(AppEvents.PeriodoAulaList);
	}

	private void onList(AppEvent event) {
		periodoDeAulaView = new PeriodoAulaView(this);
		forwardToView(periodoDeAulaView, event);
	}
	
	@Override
	public void handleEvent(AppEvent event) {
		EventType type = event.getType();
		if (type == AppEvents.PeriodoAulaList) {
			onList(event);
		}
	}

}
