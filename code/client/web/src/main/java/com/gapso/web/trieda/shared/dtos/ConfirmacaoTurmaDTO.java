package com.gapso.web.trieda.shared.dtos;

public class ConfirmacaoTurmaDTO extends AbstractDTO< String >
	implements Comparable< ConfirmacaoTurmaDTO >
{

	private static final long serialVersionUID = 1742126554266565030L;

	// Propriedades
	public static final String PROPERTY_CONFIRMADA = "confirmada";
	public static final String PROPERTY_TURMA = "turma";
	public static final String PROPERTY_DISCIPLINA_ID = "disciplinaId";
	public static final String PROPERTY_DISCIPLINA_NOME = "disciplinaNome";
	public static final String PROPERTY_DISCIPLINA_CODIGO = "disciplinaCodigo";
	public static final String PROPERTY_CREDITOS_TEORICO = "creditosTeorico";
	public static final String PROPERTY_CREDITOS_PRATICO = "creditosPratico";
	public static final String PROPERTY_QUANTIDADE_ALUNOS = "qtdeAlunos";
	public static final String PROPERTY_SALA_ID = "salaId";
	public static final String PROPERTY_SALA_STRING = "salaString";
	public static final String PROPERTY_PROFESSOR_ID = "professorId";
	public static final String PROPERTY_PROFESSOR_STRING = "professorString";
	public static final String PROPERTY_DIAS_HORARIOS = "diasHorarios";
	
	public ConfirmacaoTurmaDTO()
	{
		super();
	}
	
	public void setConfirmada(Boolean value) {
		set(PROPERTY_CONFIRMADA, value);
	}

	public Boolean getConfirmada() {
		return get(PROPERTY_CONFIRMADA);
	}

	public void setTurma(String value) {
		set(PROPERTY_TURMA, value);
	}

	public String getTurma() {
		return get(PROPERTY_TURMA);
	}
	
	public void setDisciplinaId(Long value) {
		set(PROPERTY_DISCIPLINA_ID, value);
	}

	public Long getDisciplinaId() {
		return get(PROPERTY_DISCIPLINA_ID);
	}
	
	public void setDisciplinaNome(String value) {
		set(PROPERTY_DISCIPLINA_NOME, value);
	}

	public String getDisciplinaNome() {
		return get(PROPERTY_DISCIPLINA_NOME);
	}
	
	public void setDisciplinaCodigo(String value) {
		set(PROPERTY_DISCIPLINA_CODIGO, value);
	}

	public String getDisciplinaCodigo() {
		return get(PROPERTY_DISCIPLINA_CODIGO);
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
	
	public void setQuantidadeAlunos(Integer value) {
		set(PROPERTY_QUANTIDADE_ALUNOS, value);
	}

	public Integer getQuantidadeAlunos() {
		return get(PROPERTY_QUANTIDADE_ALUNOS);
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
	
	public void setDiasHorarios(String value) {
		set(PROPERTY_DIAS_HORARIOS, value);
	}

	public String getDiasHorarios() {
		return get(PROPERTY_DIAS_HORARIOS);
	}

	@Override
	public int compareTo(ConfirmacaoTurmaDTO o) {
		return (getDisciplinaCodigo() + getTurma()).compareTo(o.getDisciplinaCodigo() + o.getTurma());
	}

	@Override
	public String getNaturalKey() {
		return getDisciplinaCodigo() + getTurma();
	}
}
