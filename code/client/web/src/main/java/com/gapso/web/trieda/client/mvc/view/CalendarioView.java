package com.gapso.web.trieda.client.mvc.view;

import com.extjs.gxt.ui.client.event.EventType;
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.extjs.gxt.ui.client.mvc.View;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.util.view.CalendarioModal;

public class CalendarioView extends View {

	public CalendarioView(Controller controller) {
		super(controller);
	}

	@Override
	public void initialize() {
	}
	
	private void initView() {
		CalendarioModal calendarioModal = new CalendarioModal("Dias e Horários de Aula");
		calendarioModal.show();
	}
	
	@Override
	protected void handleEvent(AppEvent event) {
		EventType type = event.getType();
		if(type == AppEvents.SemanaLetivaView) initView();  
	}

}
