package com.gapso.web.trieda.client;

import com.extjs.gxt.ui.client.mvc.Dispatcher;
import com.gapso.web.trieda.client.controller.mvc.controller.AppController;
import com.gapso.web.trieda.client.controller.mvc.controller.CursoController;
import com.gapso.web.trieda.client.controller.mvc.controller.SalaController;
import com.gapso.web.trieda.client.controller.mvc.controller.UnidadeController;
import com.google.gwt.core.client.EntryPoint;

/**
 * Entry point classes define <code>onModuleLoad()</code>.
 */
public class Trieda implements EntryPoint {
	
	/**
	 * This is the entry point method.
	 */
	public void onModuleLoad() {
		Dispatcher dispatcher = Dispatcher.get(); 
		dispatcher.addController(new AppController());
		dispatcher.addController(new UnidadeController());
		dispatcher.addController(new SalaController());
		dispatcher.addController(new CursoController());
		
		dispatcher.dispatch(AppEvents.Init);
	}
}
