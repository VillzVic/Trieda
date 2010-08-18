package com.gapso.web.trieda.client.mvc.view;

import com.extjs.gxt.ui.client.Registry;
import com.extjs.gxt.ui.client.mvc.AppEvent;
import com.extjs.gxt.ui.client.mvc.Controller;
import com.extjs.gxt.ui.client.mvc.View;
import com.gapso.web.trieda.client.AppEvents;
import com.gapso.web.trieda.client.crudmodel.MatrizCurricularCrudModel;
import com.gapso.web.trieda.client.crudmodel.MatrizCurricularDisciplinaCrudModel;
import com.gapso.web.trieda.client.mvc.model.MatrizCurricularDisciplinaModel;
import com.gapso.web.trieda.client.mvc.model.MatrizCurricularModel;
import com.gapso.web.trieda.client.util.resources.Resources;
import com.gapso.web.trieda.client.util.view.GTab;
import com.gapso.web.trieda.client.util.view.GTabItem;
import com.gapso.web.trieda.client.util.view.simplecrud.SimpleCrud;

public class MatrizCurricularView extends View {

	private GTabItem tabItem;
	
	public MatrizCurricularView(Controller controller) {
		super(controller);
	}

	@Override
	public void initialize() {
	}
	
	private void initList() {
		tabItem = new GTabItem("Matrizes Curriculares", Resources.DEFAULTS.matrizCurricular16());
		tabItem.setContent(new SimpleCrud<MatrizCurricularModel>(new MatrizCurricularCrudModel()));
		GTab tab = (GTab)Registry.get(AppView.TAB);
		tab.add(tabItem);
	}
	
	private void initMatrizCurricularDisciplinasList() {
		tabItem = new GTabItem("Matriz Curricular/Disciplinas", Resources.DEFAULTS.matrizCurricular16());
		tabItem.setContent(new SimpleCrud<MatrizCurricularDisciplinaModel>(new MatrizCurricularDisciplinaCrudModel()));
		GTab tab = (GTab)Registry.get(AppView.TAB);
		tab.add(tabItem);
	}
	
	
	@Override
	protected void handleEvent(AppEvent event) {
		if (event.getType() == AppEvents.MatrizCurricularList) initList();
		if (event.getType() == AppEvents.MatrizCurricularDisciplinaList) initMatrizCurricularDisciplinasList();
	}

}
