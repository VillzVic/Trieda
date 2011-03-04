package com.gapso.web.trieda.shared.dtos;




public class ProfessorDisciplinaDTO extends AbstractDTO<String> implements Comparable<ProfessorDisciplinaDTO> {

	private static final long serialVersionUID = 5815525344760896272L;
	
	public ProfessorDisciplinaDTO() {
	}

	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_PROFESSOR_ID = "professorId";
	public static final String PROPERTY_PROFESSOR_STRING = "professorString";
	public static final String PROPERTY_PROFESSOR_CPF = "professorCpf";
	public static final String PROPERTY_DISCIPLINA_ID = "disciplinaId";
	public static final String PROPERTY_DISCIPLINA_STRING = "disciplinaString";
	public static final String PROPERTY_PREFERENCIA = "preferencia";
	public static final String PROPERTY_NOTA_DESEMPENHO = "notaDesempenho";
	
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
	
	public void setProfessorId(Long value) {
		set(PROPERTY_PROFESSOR_ID, value);
	}
	public Long getProfessorId() {
		return get(PROPERTY_PROFESSOR_ID);
	}

	public void setProfessorString(String value) {
		set(PROPERTY_PROFESSOR_STRING, value);
	}
	public String getProfessorString() {
		return get(PROPERTY_PROFESSOR_STRING);
	}
	
	public void setProfessorCpf(String value) {
		set(PROPERTY_PROFESSOR_CPF, value);
	}
	public String getProfessorCpf() {
		return get(PROPERTY_PROFESSOR_CPF);
	}
	
	public void setDisciplinaId(Long value) {
		set(PROPERTY_DISCIPLINA_ID, value);
	}
	public Long getDisciplinaId() {
		return get(PROPERTY_DISCIPLINA_ID);
	}
	
	public void setDisciplinaString(String value) {
		set(PROPERTY_DISCIPLINA_STRING, value);
	}
	public String getDisciplinaString() {
		return get(PROPERTY_DISCIPLINA_STRING);
	}

	public void setPreferencia(Integer value) {
		set(PROPERTY_PREFERENCIA, value);
	}
	public Integer getPreferencia() {
		return get(PROPERTY_PREFERENCIA);
	}
	
	public void setNotaDesempenho(Integer value) {
		set(PROPERTY_NOTA_DESEMPENHO, value);
	}
	public Integer getNotaDesempenho() {
		return get(PROPERTY_NOTA_DESEMPENHO);
	}

	@Override
	public String getNaturalKey() {
		return getDisciplinaString() + "-" + getProfessorCpf();
	}
	
	@Override
	public int compareTo(ProfessorDisciplinaDTO o) {
		int result = getDisciplinaString().compareTo(o.getDisciplinaString());
		if (result == 0) {
			result = getProfessorString().compareTo(o.getProfessorString());
		}
		return result;
	}
}