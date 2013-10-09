package com.gapso.web.trieda.shared.dtos;

public class AlunoDisciplinaCursadaDTO extends AbstractDTO<String>
	implements Comparable<DisciplinaIncompativelDTO>
{
	private static final long serialVersionUID = 2804545521094014835L;
	
	// Propriedades
	public static final String PROPERTY_CURRICULO_DISCIPLINA_ID = "curriculoDisciplinaId";
	public static final String PROPERTY_CURSO_ID = "cursoId";
	public static final String PROPERTY_CURRICULO_ID = "curriculoId";
	public static final String PROPERTY_PERIODO = "periodo";
	public static final String PROPERTY_DISCIPLINA_ID = "disciplinaId";
	public static final String PROPERTY_ALUNO_ID = "alunoId";
	public static final String PROPERTY_CURSO_STRING = "cursoString";
	public static final String PROPERTY_CURRICULO_STRING = "curriculoString";
	public static final String PROPERTY_DISCIPLINA_STRING = "disciplinaString";
	public static final String PROPERTY_ALUNO_STRING = "alunoString";
	
	public AlunoDisciplinaCursadaDTO() {
		super();
	}
	
	public void setCurriculoDisciplinaId(Long value) {
		set(PROPERTY_CURRICULO_DISCIPLINA_ID, value);
	}
	public Long getCurriculoDisciplinaId() {
		return get(PROPERTY_CURRICULO_DISCIPLINA_ID);
	}
	
	public void setCursoId(Long value) {
		set(PROPERTY_CURSO_ID, value);
	}
	public Long getCursoId() {
		return get(PROPERTY_CURSO_ID);
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
	
	public void setAlunoId(Long value) {
		set(PROPERTY_ALUNO_ID, value);
	}
	public Long getAlunoId() {
		return get(PROPERTY_ALUNO_ID);
	}
	
	public void setPeriodo(Integer value) {
		set(PROPERTY_PERIODO, value);
	}
	public Integer getPeriodo() {
		return get(PROPERTY_PERIODO);
	}
	
	public void setCursoString(String value) {
		set(PROPERTY_CURSO_STRING, value);
	}
	public String getCursoString() {
		return get(PROPERTY_CURSO_STRING);
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
	
	public void setAlunoString(String value) {
		set(PROPERTY_ALUNO_STRING, value);
	}
	public String getAlunoString() {
		return get(PROPERTY_ALUNO_STRING);
	}
	
	@Override
	public String getNaturalKey() {
		return getCurriculoString() + "-" + getPeriodo() + "-" + 
				getDisciplinaString() + "-" + getAlunoString();
	}
	
	@Override
	public int compareTo(DisciplinaIncompativelDTO o) {
		return getNaturalKey().compareTo(o.getNaturalKey());
	}
}
