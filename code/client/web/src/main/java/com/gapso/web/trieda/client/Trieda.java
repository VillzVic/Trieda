package com.gapso.web.trieda.client;

import com.extjs.gxt.ui.client.mvc.Dispatcher;
import com.gapso.web.trieda.client.mvc.controller.AreaTitulacaoController;
import com.gapso.web.trieda.client.mvc.controller.CalendarioController;
import com.gapso.web.trieda.client.mvc.controller.CampusController;
import com.gapso.web.trieda.client.mvc.controller.CursoAreaTitulacaoController;
import com.gapso.web.trieda.client.mvc.controller.CursoCampusController;
import com.gapso.web.trieda.client.mvc.controller.CursoController;
import com.gapso.web.trieda.client.mvc.controller.DemandaController;
import com.gapso.web.trieda.client.mvc.controller.DisciplinaController;
import com.gapso.web.trieda.client.mvc.controller.DivisaoCreditosController;
import com.gapso.web.trieda.client.mvc.controller.EquivalenciaController;
import com.gapso.web.trieda.client.mvc.controller.MatrizCurricularController;
import com.gapso.web.trieda.client.mvc.controller.ProfessorController;
import com.gapso.web.trieda.client.mvc.controller.SalaController;
import com.gapso.web.trieda.client.mvc.controller.UnidadeController;
import com.gapso.web.trieda.client.mvp.presenter.AppPresenter;
import com.gapso.web.trieda.client.mvp.presenter.Presenter;
import com.gapso.web.trieda.client.mvp.view.AppView;
import com.google.gwt.core.client.EntryPoint;
import com.google.gwt.user.client.ui.RootPanel;

/**
 * Entry point classes define <code>onModuleLoad()</code>.
 */
public class Trieda implements EntryPoint {
	
	/**
	 * This is the entry point method.
	 */
	public void onModuleLoad() {
		Dispatcher dispatcher = Dispatcher.get(); 
		dispatcher.addController(new CampusController());
		dispatcher.addController(new UnidadeController());
		dispatcher.addController(new SalaController());
		dispatcher.addController(new CursoController());
		dispatcher.addController(new DisciplinaController());
		dispatcher.addController(new MatrizCurricularController());
		dispatcher.addController(new CursoCampusController());
		dispatcher.addController(new ProfessorController());
		dispatcher.addController(new DemandaController());
		dispatcher.addController(new DivisaoCreditosController());
		dispatcher.addController(new AreaTitulacaoController());
		dispatcher.addController(new CursoAreaTitulacaoController());
		dispatcher.addController(new CalendarioController());
		dispatcher.addController(new EquivalenciaController());
		
//		dispatcher.dispatch(AppEvents.Init);
		
	    Presenter appPresenter = new AppPresenter(new AppView());
	    appPresenter.go(RootPanel.get());
		
		RootPanel.get("loading").setVisible(false);
	}
}
