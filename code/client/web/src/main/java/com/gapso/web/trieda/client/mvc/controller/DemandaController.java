package com.gapso.web.trieda.client.mvc.controller;

import com.extjs.gxt.ui.client.event.EventType;
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.mvc.view.DemandaView;

public class DemandaController extends Controller {

	private DemandaView demandaView;
	
	public DemandaController() {
		registerEventTypes(AppEvents.DemandaList);
	}

	private void onList(AppEvent event) {
		demandaView = new DemandaView(this);
		forwardToView(demandaView, event);
	}
	
	@Override
	public void handleEvent(AppEvent event) {
		EventType type = event.getType();
		if (type == AppEvents.DemandaList) {
			onList(event);
		}
	}

}
