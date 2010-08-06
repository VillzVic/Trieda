package com.gapso.web.trieda.client.services;

import com.extjs.gxt.ui.client.Registry;
import com.google.gwt.core.client.GWT;

public class Services {

	public static final String UNIDADES = "unidades_service";
	public static final String SALAS = "salas_service";
	public static final String CURSOS = "cursos_service";
	
	public static Object get(String id) {
		if(id.equals(UNIDADES)) return unidades();
		if(id.equals(SALAS)) return salas();
		if(id.equals(CURSOS)) return cursos();
		return null;
	}
	
	public static UnidadesServiceAsync unidades() {
		UnidadesServiceAsync service = (UnidadesServiceAsync) Registry.get(UNIDADES);
		if(service == null) {
			service = GWT.create(UnidadesService.class);
			Registry.register(UNIDADES, service);
		}
		return service;
	}
	
	public static SalasServiceAsync salas() {
		SalasServiceAsync service = (SalasServiceAsync) Registry.get(SALAS);
		if(service == null) {
			service = GWT.create(SalasService.class);
			Registry.register(SALAS, service);
		}
		return service;
	}
	
	public static CursosServiceAsync cursos() {
		CursosServiceAsync service = (CursosServiceAsync) Registry.get(CURSOS);
		if(service == null) {
			service = GWT.create(CursosService.class);
			Registry.register(CURSOS, service);
		}
		return service;
	}
	
}
