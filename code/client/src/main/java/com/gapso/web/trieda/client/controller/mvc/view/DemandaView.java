package com.gapso.web.trieda.client.controller.mvc.view;

import com.extjs.gxt.ui.client.Registry;
import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.extjs.gxt.ui.client.mvc.View;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.crudmodel.DemandaCrudModel;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.simplecrud.SimpleCrud;

public class DemandaView extends View {

	private GTabItem tabItem;
	
	public DemandaView(Controller controller) {
		super(controller);
	}

	@Override
	public void initialize() {
	}
	
	private void initList() {
		tabItem = new GTabItem("Previsão de Demandas", Resources.DEFAULTS.demanda16());
		
		tabItem.setContent(new SimpleCrud<ModelData>(new DemandaCrudModel()));
		
		GTab tab = (GTab)Registry.get(AppView.TAB);
		tab.add(tabItem);
	}
	
	@Override
	protected void handleEvent(AppEvent event) {
		if (event.getType() == AppEvents.DemandaList) {
			initList();
		}
	}

}
