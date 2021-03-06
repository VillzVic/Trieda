package com.gapso.web.trieda.shared.dtos;

import com.gapso.web.trieda.shared.util.TriedaCurrency;
import com.gapso.web.trieda.shared.util.TriedaUtil;

public class ResumoCursoDTO extends AbstractTreeDTO<String> implements
		Comparable<ResumoCursoDTO> {
	private static final long serialVersionUID = -5134820110949139907L;

	// Propriedades
	public static final String PROPERTY_OFERTA_ID = "ofertaId";
	public static final String PROPERTY_CAMPUS_ID = "campusId";
	public static final String PROPERTY_CAMPUS_STRING = "campusString";
	public static final String PROPERTY_TURNO_ID = "turnoId";
	public static final String PROPERTY_TURNO_STRING = "turnoString";
	public static final String PROPERTY_CURSO_ID = "cursoId";
	public static final String PROPERTY_CURSO_STRING = "cursoString";
	public static final String PROPERTY_MATRIZCURRICULAR_ID = "matrizCurricularId";
	public static final String PROPERTY_MATRIZCURRICULAR_STRING = "matrizCurricularString";
	public static final String PROPERTY_PERIODO_INT = "periodoInt";
	public static final String PROPERTY_DISCIPLINA_ID = "disciplinaId";
	public static final String PROPERTY_DISCIPLINA_STRING = "disciplinaString";
	public static final String PROPERTY_TURMA_STRING = "turmaString";
	public static final String PROPERTY_SALA_ID = "salaId";
	public static final String PROPERTY_SALA_STRING = "salaString";
	public static final String PROPERTY_DIA_SEMANA = "diaSemana";
	public static final String PROPERTY_TIPO_CREDITO_TEORICO_BOOLEAN = "tipoCreditoString";
	public static final String PROPERTY_CREDITOS_INT = "creditosInt";
	public static final String PROPERTY_QUANTIDADE_ALUNOS_INT = "quantidadeAlunosInt";
	public static final String PROPERTY_RATEIO_DOUBLE = "rateioDouble";
	public static final String PROPERTY_RATEIO_STRING = "rateioString";
	public static final String PROPERTY_PROFESSOR_CPF = "professorCPF";
	public static final String PROPERTY_PROFESSOR_NOME = "professorNome";
	public static final String PROPERTY_CUSTO_DOCENTE_DOUBLE = "custoDocenteDouble";
	public static final String PROPERTY_CUSTO_DOCENTE_STRING = "custoDocenteString";
	public static final String PROPERTY_RECEITA_DOUBLE = "receitaDouble";
	public static final String PROPERTY_RECEITA_STRING = "receitaString";
	public static final String PROPERTY_MARGEM_DOUBLE = "margemDouble";
	public static final String PROPERTY_MARGEM_STRING = "margemString";
	public static final String PROPERTY_MARGEM_PERCENTE_DOUBLE = "margemPercenteDouble";
	public static final String PROPERTY_MARGEM_PERCENTE_STRING = "margemPercenteString";

	private boolean hasChildren = false;

	public ResumoCursoDTO() {
		super();
	}

	public void setOfertaId(Long value) {
		set(PROPERTY_OFERTA_ID, value);
	}

	public Long getOfertaId() {
		return get(PROPERTY_OFERTA_ID);
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

	public void setTurnoId(Long value) {
		set(PROPERTY_TURNO_ID, value);
	}

	public Long getTurnoId() {
		return get(PROPERTY_TURNO_ID);
	}

	public void setTurnoString(String value) {
		set(PROPERTY_TURNO_STRING, value);
	}

	public String getTurnoString() {
		return get(PROPERTY_TURNO_STRING);
	}

	public void setCursoId(Long value) {
		set(PROPERTY_CURSO_ID, value);
	}

	public Long getCursoId() {
		return get(PROPERTY_CURSO_ID);
	}

	public void setCursoString(String value) {
		set(PROPERTY_CURSO_STRING, value);
	}

	public String getCursoString() {
		return get(PROPERTY_CURSO_STRING);
	}

	public void setMatrizCurricularId(Long value) {
		set(PROPERTY_MATRIZCURRICULAR_ID, value);
	}

	public Long getMatrizCurricularId() {
		return get(PROPERTY_MATRIZCURRICULAR_ID);
	}

	public void setMatrizCurricularString(String value) {
		set(PROPERTY_MATRIZCURRICULAR_STRING, value);
	}

	public String getMatrizCurricularString() {
		return get(PROPERTY_MATRIZCURRICULAR_STRING);
	}

	public void setPeriodo(Integer value) {
		set(PROPERTY_PERIODO_INT, value);
	}

	public Integer getPeriodo() {
		return get(PROPERTY_PERIODO_INT);
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

	public void setTurma(String value) {
		set(PROPERTY_TURMA_STRING, value);
	}

	public String getTurma() {
		return get(PROPERTY_TURMA_STRING);
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
	
	public void setDiaSemana(Integer value) {
		set(PROPERTY_DIA_SEMANA, value);
	}

	public Integer getDiaSemana() {
		return get(PROPERTY_DIA_SEMANA);
	}

	public void setTipoCreditoTeorico(Boolean value) {
		set(PROPERTY_TIPO_CREDITO_TEORICO_BOOLEAN, value);
	}

	public Boolean getTipoCreditoTeorico() {
		return get(PROPERTY_TIPO_CREDITO_TEORICO_BOOLEAN);
	}

	public void setCreditos(Integer value) {
		set(PROPERTY_CREDITOS_INT, value);
	}

	public Integer getCreditos() {
		return get(PROPERTY_CREDITOS_INT);
	}

	public void setQuantidadeAlunos(Integer value) {
		set(PROPERTY_QUANTIDADE_ALUNOS_INT, value);
	}

	public Integer getQuantidadeAlunos() {
		return get(PROPERTY_QUANTIDADE_ALUNOS_INT);
	}

	public void setRateio(Double value) {
		set(PROPERTY_RATEIO_DOUBLE, value);
	}

	public Double getRateio() {
		return get(PROPERTY_RATEIO_DOUBLE);
	}
	
	public void setRateioString(String value) {
		set(PROPERTY_RATEIO_STRING, value);
	}

	public String getRateioString() {
		return get(PROPERTY_RATEIO_STRING);
	}

	public void setCustoDocente(TriedaCurrency value) {
		set(PROPERTY_CUSTO_DOCENTE_DOUBLE, value.toString());
	}

	public TriedaCurrency getCustoDocente() {
		return TriedaUtil.parseTriedaCurrency(get(PROPERTY_CUSTO_DOCENTE_DOUBLE));
	}
	
	public void setCustoDocenteString(String value) {
		set(PROPERTY_CUSTO_DOCENTE_STRING, value);
	}

	public String getCustoDocenteString() {
		return get(PROPERTY_CUSTO_DOCENTE_STRING);
	}
	
	public void setProfessorCPF(String value) {
		set(PROPERTY_PROFESSOR_CPF,value);
	}

	public String getProfessorCPF() {
		return get(PROPERTY_PROFESSOR_CPF);
	}
	
	public void setProfessorNome(String value) {
		set(PROPERTY_PROFESSOR_NOME,value);
	}

	public String getProfessorNome() {
		return get(PROPERTY_PROFESSOR_NOME);
	}

	public void setReceita(TriedaCurrency value) {
		set(PROPERTY_RECEITA_DOUBLE, value.toString());
	}

	public TriedaCurrency getReceita() {
		return TriedaUtil.parseTriedaCurrency(get(PROPERTY_RECEITA_DOUBLE));
	}
	
	public void setReceitaString(String value) {
		set(PROPERTY_RECEITA_STRING, value);
	}

	public String getReceitaString() {
		return get(PROPERTY_RECEITA_STRING);
	}

	public void setMargem(TriedaCurrency value) {
		set(PROPERTY_MARGEM_DOUBLE, value.toString());
	}

	public TriedaCurrency getMargem() {
		return TriedaUtil.parseTriedaCurrency(get(PROPERTY_MARGEM_DOUBLE));
	}
	
	public void setMargemString(String value) {
		set(PROPERTY_MARGEM_STRING, value);
	}

	public String getMargemString() {
		return get(PROPERTY_MARGEM_STRING);
	}

	public void setMargemPercente(Double value) {
		set(PROPERTY_MARGEM_PERCENTE_DOUBLE, value);
	}

	public Double getMargemPercente() {
		return get(PROPERTY_MARGEM_PERCENTE_DOUBLE);
	}
	
	public void setMargemPercenteString(String value) {
		set(PROPERTY_MARGEM_PERCENTE_STRING, value);
	}

	public String getMargemPercenteString() {
		return get(PROPERTY_MARGEM_PERCENTE_STRING);
	}

	public boolean hasChildren() {
		return hasChildren;
	}

	public void hasChildren(boolean hasChildren) {
		this.hasChildren = hasChildren;
	}

	@Override
	public String getNaturalKey() {
		return getCampusId() + "-" + getTurnoId() + "-" + getCursoId() + "-"
				+ getMatrizCurricularId() + "-" + getPeriodo() + "-"
				+ getDisciplinaId() + "-" + getTurma() + "-"
				+ getTipoCreditoTeorico();
	}

	@Override
	public int compareTo(ResumoCursoDTO o) {
		return getCampusString().compareTo(o.getCampusString());
	}

	@Override
	public String toString() {
		return "Campus " + getCampusId() + " - Turno " + getTurnoId()
				+ " - Curso" + getCursoId() + " - Matriz Curricular "
				+ getMatrizCurricularId() + " - Periodo " + getPeriodo()
				+ " - Disciplina " + getDisciplinaId() + " - Turma "
				+ getTurma() + " - Credito Teorico " + getTipoCreditoTeorico()
				+ "\n";
	}
}
