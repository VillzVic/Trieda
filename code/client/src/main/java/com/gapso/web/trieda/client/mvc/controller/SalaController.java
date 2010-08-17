package com.gapso.web.trieda.client.mvc.controller;

import com.extjs.gxt.ui.client.event.EventType;
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.mvc.view.GrupoSalaView;
import com.gapso.web.trieda.client.mvc.view.SalaView;

public class SalaController extends Controller {
	
	public SalaController() {
		registerEventTypes(AppEvents.SalaList);
		registerEventTypes(AppEvents.GrupoSalaList);
	}

	private void onList(AppEvent event) {
		SalaView salaView = new SalaView(this);
		forwardToView(salaView, event);
	}
	
	private void onGrupoList(AppEvent event) {
		GrupoSalaView grupoSalaView = new GrupoSalaView(this);
		forwardToView(grupoSalaView, event);
	}
	
	
	
	@Override
	public void handleEvent(AppEvent event) {
		EventType type = event.getType();
		if (type == AppEvents.SalaList) onList(event);
		if (type == AppEvents.GrupoSalaList) onGrupoList(event);
		
	}

}
