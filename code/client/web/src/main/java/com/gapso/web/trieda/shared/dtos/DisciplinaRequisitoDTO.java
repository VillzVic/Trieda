package com.gapso.web.trieda.shared.dtos;

public class DisciplinaRequisitoDTO extends AbstractDTO<String>
	implements Comparable<DisciplinaIncompativelDTO>
{
	private static final long serialVersionUID = 2804545521094014835L;
	
	// Propriedades
	public static final String PROPERTY_CURRICULO_DISCIPLINA_ID = "curriculoDisciplinaId";
	public static final String PROPERTY_CURRICULO_ID = "curriculoId";
	public static final String PROPERTY_PERIODO = "periodo";
	public static final String PROPERTY_DISCIPLINA_ID = "disciplinaId";
	public static final String PROPERTY_DISCIPLINA_REQUISITO_ID = "disciplinaRequisitoId";
	public static final String PROPERTY_CURRICULO_STRING = "curriculoString";
	public static final String PROPERTY_DISCIPLINA_STRING = "disciplinaString";
	public static final String PROPERTY_DISCIPLINA_REQUISITO_STRING = "disciplinaRequisitoString";
	
	public DisciplinaRequisitoDTO() {
		super();
	}
	
	public void setCurriculoDisciplinaId(Long value) {
		set(PROPERTY_CURRICULO_DISCIPLINA_ID, value);
	}
	public Long getCurriculoDisciplinaId() {
		return get(PROPERTY_CURRICULO_DISCIPLINA_ID);
	}
	
	public void setCurriculoId(Long value) {
		set(PROPERTY_CURRICULO_ID, value);
	}
	public Long getCurriculoId() {
		return get(PROPERTY_CURRICULO_ID);
	}
	
	public void setDisciplinaId(Long value) {
		set(PROPERTY_DISCIPLINA_ID, value);
	}
	public Long getDisciplinaId() {
		return get(PROPERTY_DISCIPLINA_ID);
	}
	
	public void setDisciplinaRequisitoId(Long value) {
		set(PROPERTY_DISCIPLINA_REQUISITO_ID, value);
	}
	public Long getDisciplinaRequisitoId() {
		return get(PROPERTY_DISCIPLINA_REQUISITO_ID);
	}
	
	public void setPeriodo(Integer value) {
		set(PROPERTY_PERIODO, value);
	}
	public Integer getPeriodo() {
		return get(PROPERTY_PERIODO);
	}
	
	public void setCurriculoString(String value) {
		set(PROPERTY_CURRICULO_STRING, value);
	}
	public String getCurriculoString() {
		return get(PROPERTY_CURRICULO_STRING);
	}
	
	public void setDisciplinaString(String value) {
		set(PROPERTY_DISCIPLINA_STRING, value);
	}
	public String getDisciplinaString() {
		return get(PROPERTY_DISCIPLINA_STRING);
	}
	
	public void setDisciplinaRequisitoString(String value) {
		set(PROPERTY_DISCIPLINA_REQUISITO_STRING, value);
	}
	public String getDisciplinaRequisitoString() {
		return get(PROPERTY_DISCIPLINA_REQUISITO_STRING);
	}
	
	@Override
	public String getNaturalKey() {
		return getCurriculoString() + "-" + getPeriodo() + "-" + 
				getDisciplinaString() + "-" + getDisciplinaRequisitoString();
	}
	
	@Override
	public int compareTo(DisciplinaIncompativelDTO o) {
		return getNaturalKey().compareTo(o.getNaturalKey());
	}
}
