package com.gapso.web.trieda.client.services;

import com.extjs.gxt.ui.client.Registry;
import com.google.gwt.core.client.GWT;

public class Services {

	public static final String UNIDADES = "unidades_service";
	public static final String SALAS = "salas_service";
	public static final String CURSOS = "cursos_service";
	public static final String DISCIPLINAS = "disciplinas_service";
	public static final String MATRIZESCURRICULARES = "matrizescurriculares_service";
	public static final String CURSOSCAMPI = "cursoscampi_service";
	public static final String PROFESSORES = "professores_service";
	
	public static Object get(String id) {
		if(id.equals(UNIDADES)) return unidades();
		if(id.equals(SALAS)) return salas();
		if(id.equals(CURSOS)) return cursos();
		if(id.equals(DISCIPLINAS)) return disciplinas();
		if(id.equals(MATRIZESCURRICULARES)) return matrizesCurriculares();
		if(id.equals(CURSOSCAMPI)) return cursosCampi();
		if(id.equals(PROFESSORES)) return professores();
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
	
	public static DisciplinasServiceAsync disciplinas() {
		DisciplinasServiceAsync service = (DisciplinasServiceAsync) Registry.get(DISCIPLINAS);
		if(service == null) {
			service = GWT.create(DisciplinasService.class);
			Registry.register(DISCIPLINAS, service);
		}
		return service;
	}
	
	public static MatrizesCurricularesServiceAsync matrizesCurriculares() {
		MatrizesCurricularesServiceAsync service = (MatrizesCurricularesServiceAsync) Registry.get(MATRIZESCURRICULARES);
		if(service == null) {
			service = GWT.create(MatrizesCurricularesService.class);
			Registry.register(MATRIZESCURRICULARES, service);
		}
		return service;
	}
	
	public static CursosCampiServiceAsync cursosCampi() {
		CursosCampiServiceAsync service = (CursosCampiServiceAsync) Registry.get(CURSOSCAMPI);
		if(service == null) {
			service = GWT.create(CursosCampiService.class);
			Registry.register(CURSOSCAMPI, service);
		}
		return service;
	}
	
	public static ProfessoresServiceAsync professores() {
		ProfessoresServiceAsync service = (ProfessoresServiceAsync) Registry.get(PROFESSORES);
		if(service == null) {
			service = GWT.create(ProfessoresService.class);
			Registry.register(PROFESSORES, service);
		}
		return service;
	}
	
}
