package com.gapso.web.trieda.client.mvc.controller;

import com.extjs.gxt.ui.client.event.EventType;
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.mvc.view.MatrizCurricularView;

public class MatrizCurricularController extends Controller {

	private MatrizCurricularView matrizCurricularView;
	
	public MatrizCurricularController() {
		registerEventTypes(AppEvents.MatrizCurricularList);
	}

	private void onList(AppEvent event) {
		matrizCurricularView = new MatrizCurricularView(this);
		forwardToView(matrizCurricularView, event);
	}
	
	@Override
	public void handleEvent(AppEvent event) {
		EventType type = event.getType();
		if (type == AppEvents.MatrizCurricularList) {
			onList(event);
		}
	}

}
