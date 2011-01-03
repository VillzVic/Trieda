package com.gapso.web.trieda.client.services;

import com.extjs.gxt.ui.client.Registry;
import com.google.gwt.core.client.GWT;

public class Services {

	private static final String OTIMIZAR = "otimizar";
	private static final String CENARIOS = "cenarios";
	private static final String CAMPI = "campi";
	private static final String TURNOS = "turnos";
	private static final String UNIDADES = "unidades";
	private static final String SALAS = "salas";
	private static final String GRUPOSSALAS = "gruposalas";
	private static final String CURSOS = "cursos";
	private static final String DISCIPLINAS = "disciplinas";
	private static final String OFERTAS = "ofertas";
	private static final String CURRICULOS = "curriculos";
	private static final String DEMANDAS = "demandas";
	private static final String HORARIOSAULA = "horariosaula";
	private static final String AREASTITULACAO = "areasTitulacao";
	private static final String SEMANASLETIVA = "semanasLetiva";
	private static final String TIPOSCURSOS = "tiposCursos";
	private static final String ATENDIMENTOS = "atendimentos";
	private static final String DIVISAOCREDITOS = "divisaoCreditos";
	private static final String PROFESSORES = "professores";
	private static final String PROFESSORESDISCIPLINA = "professoresDisciplina";
	private static final String EQUIVALENCIAS = "equivalencias";
	private static final String FIXACOES = "fixacoes";
	
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

	public static DemandasServiceAsync demandas() {
		DemandasServiceAsync service = (DemandasServiceAsync) Registry.get(DEMANDAS);
		if(service == null) {
			service = GWT.create(DemandasService.class);
			Registry.register(DEMANDAS, service);
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
	
	public static AtendimentosServiceAsync atendimentos() {
		AtendimentosServiceAsync service = (AtendimentosServiceAsync) Registry.get(ATENDIMENTOS);
		if(service == null) {
			service = GWT.create(AtendimentosService.class);
			Registry.register(ATENDIMENTOS, service);
		}
		return service;
	}
	
	public static DivisoesCreditosServiceAsync divisaoCreditos() {
		DivisoesCreditosServiceAsync service = (DivisoesCreditosServiceAsync) Registry.get(DIVISAOCREDITOS);
		if(service == null) {
			service = GWT.create(DivisoesCreditosService.class);
			Registry.register(DIVISAOCREDITOS, service);
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
	
	public static ProfessoresDisciplinaServiceAsync professoresDisciplina() {
		ProfessoresDisciplinaServiceAsync service = (ProfessoresDisciplinaServiceAsync) Registry.get(PROFESSORESDISCIPLINA);
		if(service == null) {
			service = GWT.create(ProfessoresDisciplinaService.class);
			Registry.register(PROFESSORESDISCIPLINA, service);
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
	
	public static FixacoesServiceAsync fixacoes() {
		FixacoesServiceAsync service = (FixacoesServiceAsync) Registry.get(FIXACOES);
		if(service == null) {
			service = GWT.create(FixacoesService.class);
			Registry.register(FIXACOES, service);
		}
		return service;
	}
	
}
