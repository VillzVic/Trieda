package com.gapso.web.trieda.shared.dtos;

public class FixacaoDTO extends AbstractDTO<String>
	implements Comparable<FixacaoDTO>
{
	private static final long serialVersionUID = 6642146983954267367L;
	
	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_CENARIO_ID = "cenarioId";
	public static final String PROPERTY_DESCRICAO = "descricao";
	public static final String PROPERTY_PROFESSOR_ID = "professorId";
	public static final String PROPERTY_PROFESSOR_STRING = "professorString";
	public static final String PROPERTY_DISCIPLINA_ID = "disciplinaId";
	public static final String PROPERTY_DISCIPLINA_STRING = "disciplinaString";
	public static final String PROPERTY_TURMA_STRING = "turmaString";
	public static final String PROPERTY_CAMPUS_ID = "campusId";
	public static final String PROPERTY_CAMPUS_STRING = "campusString";
	public static final String PROPERTY_UNIDADE_ID = "unidadeId";
	public static final String PROPERTY_UNIDADE_STRING = "unidadeString";
	public static final String PROPERTY_SALA_ID = "salaId";
	public static final String PROPERTY_SALA_STRING = "salaString";
	public static final String PROPERTY_FIXA_DIAS_HORARIOS = "diaEHorarioString";
	public static final String PROPERTY_FIXA_AMBIENTE = "ambienteString";
	
	public void setId(Long value)
	{
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

	
	public void setCenarioId(Long value) {
		set(PROPERTY_CENARIO_ID, value);
	}
	public Long getCenarioId() {
		return get(PROPERTY_CENARIO_ID);
	}
	
	public void setDescricao(String value) {
		set(PROPERTY_DESCRICAO, value);
	}
	public String getDescricao() {
		return get(PROPERTY_DESCRICAO);
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
	
	public void setTurmaString(String value) {
		set(PROPERTY_TURMA_STRING, value);
	}
	public String getTurmaString() {
		return get(PROPERTY_TURMA_STRING);
	}
	
	public void setCampusId(Long value) {
		set(PROPERTY_CAMPUS_ID, value);
	}
	public Long getCampusId() {
		return get(PROPERTY_CAMPUS_ID);
	}
	
	public void setCampusString(String value) {
		set(PROPERTY_CAMPUS_STRING, value);
	}
	public String getCampusString() {
		return get(PROPERTY_CAMPUS_STRING);
	}
	
	public void setUnidadeId(Long value) {
		set(PROPERTY_UNIDADE_ID, value);
	}
	public Long getUnidadeId() {
		return get(PROPERTY_UNIDADE_ID);
	}
	
	public void setUnidadeString(String value) {
		set(PROPERTY_UNIDADE_STRING, value);
	}
	public String getUnidadeString() {
		return get(PROPERTY_UNIDADE_STRING);
	}
	
	public void setSalaId(Long value) {
		set(PROPERTY_SALA_ID, value);
	}
	public Long getSalaId() {
		return get(PROPERTY_SALA_ID);
	}
	
	public void setSalaString(String value) {
		set(PROPERTY_SALA_STRING, value);
	}
	public String getSalaString() {
		return get(PROPERTY_SALA_STRING);
	}
	
	public void setFixaDiaEHorario(Boolean value){
		set(PROPERTY_FIXA_DIAS_HORARIOS, value);
	}
	
	public Boolean getFixaDiaEHorario(){
		return get(PROPERTY_FIXA_DIAS_HORARIOS);
	}
	
	public void setFixaAmbiente(Boolean value){
		set(PROPERTY_FIXA_AMBIENTE, value);
	}
	
	public Boolean getFixaAmbiente(){
		return get(PROPERTY_FIXA_AMBIENTE);
	}
	
	@Override
	public String getNaturalKey() {
		return getId().toString();
	}
	
	@Override
	public int compareTo(FixacaoDTO o) {
		return getId().compareTo(o.getId());
	}
}