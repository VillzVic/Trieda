package com.gapso.web.trieda.client.mvc.view;

import com.extjs.gxt.ui.client.Registry;
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.extjs.gxt.ui.client.mvc.View;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.crudmodel.DivisaoCreditosCrudModel;
import com.gapso.web.trieda.client.mvc.model.DivisaoCreditosModel;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.simplecrud.SimpleCrud;

public class DivisaoCreditosView extends View {

	private GTabItem tabItem;
	
	public DivisaoCreditosView(Controller controller) {
		super(controller);
	}

	@Override
	public void initialize() {
	}
	
	private void initList() {
		tabItem = new GTabItem("Preferência de Divisões de Créditos", Resources.DEFAULTS.divisaoDeCreditos16());
		
		tabItem.setContent(new SimpleCrud<DivisaoCreditosModel>(new DivisaoCreditosCrudModel()));
		
		GTab tab = (GTab)Registry.get(AppView.TAB);
		tab.add(tabItem);
	}
	
	
	@Override
	protected void handleEvent(AppEvent event) {
		if (event.getType() == AppEvents.DivisaoCreditosList) {
			initList();
		}
	}

}
