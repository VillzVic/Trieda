package com.gapso.web.trieda.client.mvc.controller;

import com.extjs.gxt.ui.client.event.EventType;
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.mvc.view.AreaTitulacaoView;

public class AreaTitulacaoController extends Controller {

	private AreaTitulacaoView areaTitulacaoView;
	
	public AreaTitulacaoController() {
		registerEventTypes(AppEvents.AreaTitulacaoList);
	}

	private void onList(AppEvent event) {
		areaTitulacaoView = new AreaTitulacaoView(this);
		forwardToView(areaTitulacaoView, event);
	}
	
	@Override
	public void handleEvent(AppEvent event) {
		EventType type = event.getType();
		if (type == AppEvents.AreaTitulacaoList) {
			onList(event);
		}
	}

}
