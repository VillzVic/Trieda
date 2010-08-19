package com.gapso.web.trieda.client.mvc.controller;

import com.extjs.gxt.ui.client.event.EventType;
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.mvc.view.UnidadeDeslocamentoView;
import com.gapso.web.trieda.client.mvc.view.UnidadeView;

public class UnidadeController extends Controller {

	private UnidadeView unidadeView;
	
	public UnidadeController() {
		registerEventTypes(AppEvents.UnidadeList);
		registerEventTypes(AppEvents.UnidadeDeslocamento);
	}

	private void onList(AppEvent event) {
		unidadeView = new UnidadeView(this);
		forwardToView(unidadeView, event);
	}
	
	private void onDeslocamentos(AppEvent event) {
		UnidadeDeslocamentoView view = new UnidadeDeslocamentoView(this);
		forwardToView(view, event);
	}
	
	@Override
	public void handleEvent(AppEvent event) {
		EventType type = event.getType();
		if (type == AppEvents.UnidadeList) onList(event);
		if (type == AppEvents.UnidadeDeslocamento) onDeslocamentos(event);
	}

}
