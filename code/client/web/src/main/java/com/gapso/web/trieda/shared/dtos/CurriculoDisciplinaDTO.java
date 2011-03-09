package com.gapso.web.trieda.shared.dtos;



public class CurriculoDisciplinaDTO extends AbstractDTO<String> implements Comparable<CurriculoDisciplinaDTO> {

	private static final long serialVersionUID = -5134820110949139907L;
	
	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_DISCIPLINA_ID = "disciplinaId";
	public static final String PROPERTY_DISCIPLINA_STRING = "disciplinaString";
	public static final String PROPERTY_PERIODO = "periodo";
	public static final String PROPERTY_CREDITOS_TEORICO = "creditosTeorico";
	public static final String PROPERTY_CREDITOS_PRATICO = "creditosPratico";
	public static final String PROPERTY_CREDITOS_TOTAL = "creditosTotal";
	public static final String PROPERTY_CURRICULO_ID = "curriculoId";

	public CurriculoDisciplinaDTO() {
		super();
	}

	public CurriculoDisciplinaDTO(Long id, Long disciplinaId, String disciplinaString, Integer periodo, Integer creditosTeorico, Integer creditosPratico, Integer creditosTotal, Long curriculoId, Integer version) {
		setId(id);
		setDisciplinaId(disciplinaId);
		setDisciplinaString(disciplinaString);
		setPeriodo(periodo);
		setCreditosTeorico(creditosTeorico);
		setCreditosPratico(creditosPratico);
		setCreditosTotal(creditosTotal);
		setCurriculoId(curriculoId);
		setVersion(version);
	}
	
	public void setId(Long value) {
		set(PROPERTY_ID, value);
	}
	public Long getId() {
		return get(PROPERTY_ID);
	}
	
	public void setVersion(Integer value) {
		set(PROPERTY_VERSION, value);
	}
	public Integer getVersion() {
		return get(PROPERTY_VERSION);
	}
	
	public Long getDisciplinaId() {
		return get(PROPERTY_DISCIPLINA_ID);
	}
	public void setDisciplinaId(Long value) {
		set(PROPERTY_DISCIPLINA_ID, value);
	}
	
	public String getDisciplinaString() {
		return get(PROPERTY_DISCIPLINA_STRING);
	}
	public void setDisciplinaString(String value) {
		set(PROPERTY_DISCIPLINA_STRING, value);
	}
	
	public void setPeriodo(Integer value) {
		set(PROPERTY_PERIODO, value);
	}
	public Integer getPeriodo() {
		return get(PROPERTY_PERIODO);
	}
	
	public void setCreditosTeorico(Integer value) {
		set(PROPERTY_CREDITOS_TEORICO, value);
	}
	public Integer getCreditosTeorico() {
		return get(PROPERTY_CREDITOS_TEORICO);
	}
	
	public void setCreditosPratico(Integer value) {
		set(PROPERTY_CREDITOS_PRATICO, value);
	}
	public Integer getCreditosPratico() {
		return get(PROPERTY_CREDITOS_PRATICO);
	}
	
	public void setCreditosTotal(Integer value) {
		set(PROPERTY_CREDITOS_TOTAL, value);
	}
	public Integer getCreditosTotal() {
		return get(PROPERTY_CREDITOS_TOTAL);
	}
	
	public void setCurriculoId(Long value) {
		set(PROPERTY_CURRICULO_ID, value);
	}
	public Long getCurriculoId() {
		return get(PROPERTY_CURRICULO_ID);
	}
	
	@Override
	public String getNaturalKey() {
		return getCurriculoId().toString() + "-" + getPeriodo().toString() + "-" + getDisciplinaId();
	}
	
	@Override
	public int compareTo(CurriculoDisciplinaDTO o) {
		int result = getPeriodo().compareTo(o.getPeriodo());
		if (result == 0) {
			result = getDisciplinaString().compareTo(o.getDisciplinaString());
		}
		return result;
	}
}
