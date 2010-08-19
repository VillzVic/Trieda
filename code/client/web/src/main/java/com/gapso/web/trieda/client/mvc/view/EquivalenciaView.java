package com.gapso.web.trieda.client.mvc.view;

import com.extjs.gxt.ui.client.Registry;
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.extjs.gxt.ui.client.mvc.View;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.crudmodel.EquivalenciaCrudModel;
import com.gapso.web.trieda.client.mvc.model.EquivalenciaModel;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.simplecrud.SimpleCrud;

public class EquivalenciaView extends View {

	private GTabItem tabItem;
	
	public EquivalenciaView(Controller controller) {
		super(controller);
	}

	@Override
	public void initialize() {
	}
	
	private void initList() {
		tabItem = new GTabItem("Equivalência entre Disciplinas", Resources.DEFAULTS.disciplina16());
		
		tabItem.setContent(new SimpleCrud<EquivalenciaModel>(new EquivalenciaCrudModel()));
		
		GTab tab = (GTab)Registry.get(AppView.TAB);
		tab.add(tabItem);
	}
	
	@Override
	protected void handleEvent(AppEvent event) {
		if (event.getType() == AppEvents.EquivalenciaList) {
			initList();
		}
	}

}
