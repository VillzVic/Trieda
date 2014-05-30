package com.gapso.web.trieda.shared.dtos;

public class DisciplinaDemandaDTO extends AbstractDTO<String> implements Comparable<DisciplinaDemandaDTO> {
	
	private static final long serialVersionUID = 307985406723990729L;
	
	
	public static final String PROPERTY_DISCIPLINA = "disciplina";
	public static final String PROPERTY_DISCIPLINA_ID = "disciplinaId";
	public static final String PROPERTY_DEMANDA_REAL = "demandaReal";
	public static final String PROPERTY_DEMANDA_VIRTUAL = "demandaVirtual";
	public static final String PROPERTY_DEMANDA_TOTAL = "demandaTotal";
	public static final String PROPERTY_NOVA_DEMANDA = "novaDemanda";
	public static final String PROPERTY_EXIGE_EQUIVALENCIA_FORCADA = "exigeEquivalenciaForcada";
	
	public DisciplinaDemandaDTO() {
		super();
	}
	
	public String getDisciplina() {
		return get(PROPERTY_DISCIPLINA);
	}
	public void setDisciplina(String value) {
		set(PROPERTY_DISCIPLINA, value);
	}
	
	public Long getDisciplinaId() {
		return get(PROPERTY_DISCIPLINA_ID);
	}
	public void setDisciplinaId(Long value) {
		set(PROPERTY_DISCIPLINA_ID, value);
	}
	
	public Integer getDemandaReal() {
		return get(PROPERTY_DEMANDA_REAL);
	}
	public void setDemandaReal(Integer value) {
		set(PROPERTY_DEMANDA_REAL, value);
	}
	
	public Integer getDemandaVirtual() {
		return get(PROPERTY_DEMANDA_VIRTUAL);
	}
	public void setDemandaVirtual(Integer value) {
		set(PROPERTY_DEMANDA_VIRTUAL, value);
	}
	
	public Integer getDemandaTotal() {
		return get(PROPERTY_DEMANDA_TOTAL);
	}
	public void setDemandaTotal(Integer value) {
		set(PROPERTY_DEMANDA_TOTAL, value);
	}
	
	public Integer getNovaDemanda() {
		return get(PROPERTY_NOVA_DEMANDA);
	}
	public void setNovaDemanda(Integer value) {
		set(PROPERTY_NOVA_DEMANDA, value);
	}
	
	public Boolean getExigeEquivalenciaForcada() {
		return get(PROPERTY_EXIGE_EQUIVALENCIA_FORCADA);
	}
	public void setExigeEquivalenciaForcada(Boolean value) {
		set(PROPERTY_EXIGE_EQUIVALENCIA_FORCADA, value);
	}

	@Override
	public int compareTo(DisciplinaDemandaDTO o) {
		return this.getDisciplina().compareTo(o.getDisciplina());
	}

	@Override
	public String getNaturalKey() {
		return getDisciplina();
	}
}
