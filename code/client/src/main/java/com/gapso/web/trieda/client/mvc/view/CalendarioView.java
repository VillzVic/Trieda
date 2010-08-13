package com.gapso.web.trieda.client.mvc.view;

import com.extjs.gxt.ui.client.Registry;
import com.extjs.gxt.ui.client.event.EventType;
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.extjs.gxt.ui.client.mvc.View;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.crudmodel.CalendarioCrudModel;
import com.gapso.web.trieda.client.mvc.model.CalendarioModel;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.CalendarioModal;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.simplecrud.SimpleCrud;

public class CalendarioView extends View {

	private GTabItem tabItem;
	
	public CalendarioView(Controller controller) {
		super(controller);
	}

	@Override
	public void initialize() {
	}
	
	private void initList() {
		tabItem = new GTabItem("Calendários", Resources.DEFAULTS.calendario16());
		tabItem.setContent(new SimpleCrud<CalendarioModel>(new CalendarioCrudModel()));
		GTab tab = (GTab)Registry.get(AppView.TAB);
		tab.add(tabItem);
	}
	
	private void initView() {
		CalendarioModal calendarioModal = new CalendarioModal("Dias de Aula do Calendário");
		calendarioModal.show();
	}
	
	@Override
	protected void handleEvent(AppEvent event) {
		EventType type = event.getType();
		if(type == AppEvents.CalendarioList) initList();  
		if(type == AppEvents.CalendarioView) initView();  
	}

}
