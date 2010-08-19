package com.gapso.web.trieda.client.mvc.view;

import com.extjs.gxt.ui.client.Registry;
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.extjs.gxt.ui.client.mvc.View;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.crudmodel.CursoCrudModel;
import com.gapso.web.trieda.client.mvc.model.CursoModel;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.simplecrud.SimpleCrud;

public class CursoView extends View {

	private GTabItem tabItem;
	
	public CursoView(Controller controller) {
		super(controller);
	}

	@Override
	public void initialize() {
	}
	
	private void initList() {
		tabItem = new GTabItem("Cursos", Resources.DEFAULTS.curso16());
		tabItem.setIdTabItem("cursos");
		
		tabItem.setContent(new SimpleCrud<CursoModel>(new CursoCrudModel()));
		
		GTab tab = (GTab)Registry.get(AppView.TAB);
		tab.add(tabItem);
	}
	
	
	@Override
	protected void handleEvent(AppEvent event) {
		if (event.getType() == AppEvents.CursoList) {
			initList();
		}
	}

}
