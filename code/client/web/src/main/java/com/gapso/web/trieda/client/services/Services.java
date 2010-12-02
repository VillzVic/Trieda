package com.gapso.web.trieda.client.services;

import com.extjs.gxt.ui.client.Registry;
import com.google.gwt.core.client.GWT;

public class Services {

	public static final String OTIMIZAR = "otimizar";
	public static final String CENARIOS = "cenarios";
	public static final String CAMPI = "campi";
	public static final String CAMPIPROFESSORES = "campiProfessores";
	public static final String TURNOS = "turnos";
	public static final String UNIDADES = "unidades";
	public static final String SALAS = "salas";
	public static final String GRUPOSSALAS = "gruposalas";
	public static final String CURSOS = "cursos";
	public static final String DISCIPLINAS = "disciplinas";
	public static final String EQUIVALENCIAS = "equivalencias";
	public static final String OFERTAS = "ofertas";
	public static final String CURRICULOS = "curriculos";
	public static final String PROFESSORES = "professores";
	public static final String PROFESSORESDISCIPLINAS = "professoresDisciplinas";
	public static final String DEMANDAS = "demandas";
	public static final String DIVISOESCREDITOS = "divisoescreditos";
	public static final String HORARIOSAULA = "horariosaula";
	public static final String AREASTITULACAO = "areasTitulacao";
	public static final String SEMANASLETIVA = "semanasLetiva";
	public static final String TIPOSCURSOS = "tiposCursos";
	
	public static Object get(String id) {
		if(id.equals(CAMPI)) return campi();
		if(id.equals(CAMPIPROFESSORES)) return campiProfessores();
		if(id.equals(TURNOS)) return turnos();
		if(id.equals(UNIDADES)) return unidades();
		if(id.equals(SALAS)) return salas();
		if(id.equals(GRUPOSSALAS)) return gruposSalas();
		if(id.equals(CURSOS)) return cursos();
		if(id.equals(DISCIPLINAS)) return disciplinas();
		if(id.equals(EQUIVALENCIAS)) return equivalencias();
		if(id.equals(OFERTAS)) return ofertas();
		if(id.equals(CURRICULOS)) return curriculos();
		if(id.equals(PROFESSORES)) return professores();
		if(id.equals(PROFESSORESDISCIPLINAS)) return professoresDisciplinas();
		if(id.equals(DEMANDAS)) return demandas();
		if(id.equals(DIVISOESCREDITOS)) return divisoesCreditos();
		if(id.equals(HORARIOSAULA)) return horariosAula();
		if(id.equals(AREASTITULACAO)) return areasTitulacao();
		if(id.equals(SEMANASLETIVA)) return semanasLetiva();
		return null;
	}
	
	public static OtimizarServiceAsync otimizar() {
		OtimizarServiceAsync service = (OtimizarServiceAsync) Registry.get(OTIMIZAR);
		if(service == null) {
			service = GWT.create(OtimizarService.class);
			Registry.register(OTIMIZAR, service);
		}
		return service;
	}
	
	public static CenariosServiceAsync cenarios() {
		CenariosServiceAsync service = (CenariosServiceAsync) Registry.get(CENARIOS);
		if(service == null) {
			service = GWT.create(CenariosService.class);
			Registry.register(CENARIOS, service);
		}
		return service;
	}
	
	public static TurnosServiceAsync turnos() {
		TurnosServiceAsync service = (TurnosServiceAsync) Registry.get(TURNOS);
		if(service == null) {
			service = GWT.create(TurnosService.class);
			Registry.register(TURNOS, service);
		}
		return service;
	}
	
	public static CampiServiceAsync campi() {
		CampiServiceAsync service = (CampiServiceAsync) Registry.get(CAMPI);
		if(service == null) {
			service = GWT.create(CampiService.class);
			Registry.register(CAMPI, service);
		}
		return service;
	}
	
	public static CampiProfessoresServiceAsync campiProfessores() {
		CampiProfessoresServiceAsync service = (CampiProfessoresServiceAsync) Registry.get(CAMPIPROFESSORES);
		if(service == null) {
			service = GWT.create(CampiProfessoresService.class);
			Registry.register(CAMPIPROFESSORES, service);
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
	
	public static GruposSalasServiceAsync gruposSalas() {
		GruposSalasServiceAsync service = (GruposSalasServiceAsync) Registry.get(GRUPOSSALAS);
		if(service == null) {
			service = GWT.create(GruposSalasService.class);
			Registry.register(GRUPOSSALAS, service);
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
	
	public static EquivalenciasServiceAsync equivalencias() {
		EquivalenciasServiceAsync service = (EquivalenciasServiceAsync) Registry.get(EQUIVALENCIAS);
		if(service == null) {
			service = GWT.create(EquivalenciasService.class);
			Registry.register(EQUIVALENCIAS, service);
		}
		return service;
	}
	
	public static OfertasServiceAsync ofertas() {
		OfertasServiceAsync service = (OfertasServiceAsync) Registry.get(OFERTAS);
		if(service == null) {
			service = GWT.create(OfertasService.class);
			Registry.register(OFERTAS, service);
		}
		return service;
	}
	
	public static CurriculosServiceAsync curriculos() {
		CurriculosServiceAsync service = (CurriculosServiceAsync) Registry.get(CURRICULOS);
		if(service == null) {
			service = GWT.create(CurriculosService.class);
			Registry.register(CURRICULOS, service);
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
	
	public static ProfessoresDisciplinasServiceAsync professoresDisciplinas() {
		ProfessoresDisciplinasServiceAsync service = (ProfessoresDisciplinasServiceAsync) Registry.get(PROFESSORESDISCIPLINAS);
		if(service == null) {
			service = GWT.create(ProfessoresDisciplinasService.class);
			Registry.register(PROFESSORESDISCIPLINAS, service);
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
	
	public static HorariosAulaServiceAsync horariosAula() {
		HorariosAulaServiceAsync service = (HorariosAulaServiceAsync) Registry.get(HORARIOSAULA);
		if(service == null) {
			service = GWT.create(HorariosAulaService.class);
			Registry.register(HORARIOSAULA, service);
		}
		return service;
	}
	
	public static AreasTitulacaoServiceAsync areasTitulacao() {
		AreasTitulacaoServiceAsync service = (AreasTitulacaoServiceAsync) Registry.get(AREASTITULACAO);
		if(service == null) {
			service = GWT.create(AreasTitulacaoService.class);
			Registry.register(AREASTITULACAO, service);
		}
		return service;
	}
	
	public static SemanasLetivaServiceAsync semanasLetiva() {
		SemanasLetivaServiceAsync service = (SemanasLetivaServiceAsync) Registry.get(SEMANASLETIVA);
		if(service == null) {
			service = GWT.create(SemanasLetivaService.class);
			Registry.register(SEMANASLETIVA, service);
		}
		return service;
	}
	
	public static TiposCursosServiceAsync tiposCursos() {
		TiposCursosServiceAsync service = (TiposCursosServiceAsync) Registry.get(TIPOSCURSOS);
		if(service == null) {
			service = GWT.create(TiposCursosService.class);
			Registry.register(TIPOSCURSOS, service);
		}
		return service;
	}
}
