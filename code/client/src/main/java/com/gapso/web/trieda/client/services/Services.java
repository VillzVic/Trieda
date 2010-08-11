package com.gapso.web.trieda.client.services;

import com.extjs.gxt.ui.client.Registry;
import com.google.gwt.core.client.GWT;

public class Services {

	public static final String CAMPI = "campi";
	public static final String UNIDADES = "unidades";
	public static final String SALAS = "salas";
	public static final String CURSOS = "cursos";
	public static final String DISCIPLINAS = "disciplinas";
	public static final String MATRIZESCURRICULARES = "matrizescurriculares";
	public static final String CURSOSCAMPI = "cursoscampi";
	public static final String PROFESSORES = "professores";
	public static final String DEMANDAS = "demandas";
	public static final String DIVISOESCREDITOS = "divisoescreditos";
	public static final String PERIODOSAULA = "periodosaula";
	
	public static Object get(String id) {
		if(id.equals(CAMPI)) return campi();
		if(id.equals(UNIDADES)) return unidades();
		if(id.equals(SALAS)) return salas();
		if(id.equals(CURSOS)) return cursos();
		if(id.equals(DISCIPLINAS)) return disciplinas();
		if(id.equals(MATRIZESCURRICULARES)) return matrizesCurriculares();
		if(id.equals(CURSOSCAMPI)) return cursosCampi();
		if(id.equals(PROFESSORES)) return professores();
		if(id.equals(DEMANDAS)) return demandas();
		if(id.equals(DIVISOESCREDITOS)) return divisoesCreditos();
		if(id.equals(PERIODOSAULA)) return periodosAula();
		return null;
	}
	
	public static CampiServiceAsync campi() {
		CampiServiceAsync service = (CampiServiceAsync) Registry.get(CAMPI);
		if(service == null) {
			service = GWT.create(CampiService.class);
			Registry.register(CAMPI, service);
		}
		return service;
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
	
	public static DemandasServiceAsync demandas() {
		DemandasServiceAsync service = (DemandasServiceAsync) Registry.get(DEMANDAS);
		if(service == null) {
			service = GWT.create(DemandasService.class);
			Registry.register(DEMANDAS, service);
		}
		return service;
	}
	
	public static DivisoesCreditosServiceAsync divisoesCreditos() {
		DivisoesCreditosServiceAsync service = (DivisoesCreditosServiceAsync) Registry.get(DIVISOESCREDITOS);
		if(service == null) {
			service = GWT.create(DivisoesCreditosService.class);
			Registry.register(DIVISOESCREDITOS, service);
		}
		return service;
	}
	
	public static PeriodosDeAulasServiceAsync periodosAula() {
		PeriodosDeAulasServiceAsync service = (PeriodosDeAulasServiceAsync) Registry.get(PERIODOSAULA);
		if(service == null) {
			service = GWT.create(PeriodosDeAulasService.class);
			Registry.register(PERIODOSAULA, service);
		}
		return service;
	}
	
}
