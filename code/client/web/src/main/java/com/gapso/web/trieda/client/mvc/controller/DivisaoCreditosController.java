package com.gapso.web.trieda.client.mvc.controller;

import com.extjs.gxt.ui.client.event.EventType;
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.mvc.view.DivisaoCreditosView;

public class DivisaoCreditosController extends Controller {

	private DivisaoCreditosView divisaoCreditosView;
	
	public DivisaoCreditosController() {
		registerEventTypes(AppEvents.DivisaoCreditosList);
	}

	private void onList(AppEvent event) {
		divisaoCreditosView = new DivisaoCreditosView(this);
		forwardToView(divisaoCreditosView, event);
	}
	
	@Override
	public void handleEvent(AppEvent event) {
		EventType type = event.getType();
		if (type == AppEvents.DivisaoCreditosList) {
			onList(event);
		}
	}

}
