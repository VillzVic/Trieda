package com.gapso.web.trieda.client.controller.mvc.view;

import com.extjs.gxt.ui.client.Registry;
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.extjs.gxt.ui.client.mvc.View;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.controller.mvc.model.AreaTitulacaoModel;
import com.gapso.web.trieda.client.crudmodel.AreaTitulacaoCrudModel;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.simplecrud.SimpleCrud;

public class AreaTitulacaoView extends View {

	private GTabItem tabItem;
	
	public AreaTitulacaoView(Controller controller) {
		super(controller);
	}

	@Override
	public void initialize() {
	}
	
	private void initList() {
		tabItem = new GTabItem("Áreas de Titulação", Resources.DEFAULTS.areasDeTitulacao16());
		
		tabItem.setContent(new SimpleCrud<AreaTitulacaoModel>(new AreaTitulacaoCrudModel()));
		
		GTab tab = (GTab)Registry.get(AppView.TAB);
		tab.add(tabItem);
	}
	
	
	@Override
	protected void handleEvent(AppEvent event) {
		if (event.getType() == AppEvents.AreaTitulacaoList) {
			initList();
		}
	}

}
