package com.gapso.web.trieda.client.controller.mvc.controller;

import com.extjs.gxt.ui.client.event.EventType;
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.controller.mvc.view.CursoAreaTitulacaoView;

public class CursoAreaTitulacaoController extends Controller {

	private CursoAreaTitulacaoView cursoAreaTitulacaoView;
	
	public CursoAreaTitulacaoController() {
		registerEventTypes(AppEvents.CursoAreaTitulacaoList);
	}

	private void onList(AppEvent event) {
		cursoAreaTitulacaoView = new CursoAreaTitulacaoView(this);
		forwardToView(cursoAreaTitulacaoView, event);
	}
	
	@Override
	public void handleEvent(AppEvent event) {
		EventType type = event.getType();
		if (type == AppEvents.CursoAreaTitulacaoList) {
			onList(event);
		}
	}

}
