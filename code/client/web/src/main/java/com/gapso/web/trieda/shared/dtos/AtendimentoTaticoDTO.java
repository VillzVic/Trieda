package com.gapso.web.trieda.shared.dtos;

import java.util.List;

import com.gapso.web.trieda.shared.util.TriedaUtil;

public class AtendimentoTaticoDTO extends AbstractDTO<String> implements Comparable<AtendimentoTaticoDTO> {

	private static final long serialVersionUID = -2870302894382757778L;
	
	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_CENARIO_ID = "cenarioId";
	public static final String PROPERTY_CAMPUS_ID = "campusId";
	public static final String PROPERTY_CAMPUS_STRING = "campusString";
	public static final String PROPERTY_UNIDADE_ID = "unidadeId";
	public static final String PROPERTY_UNIDADE_STRING = "unidadeString";
	public static final String PROPERTY_SALA_ID = "salaId";
	public static final String PROPERTY_SALA_STRING = "salaString";
	public static final String PROPERTY_DIA_SEMANA = "semana";
	public static final String PROPERTY_CURSO_STRING = "cursoString";
	public static final String PROPERTY_CURSO_NOME = "cursoNome";
	public static final String PROPERTY_CURRICULO_STRING = "curriculoString";
	public static final String PROPERTY_PERIODO = "periodo";
	public static final String PROPERTY_PERIODO_STRING = "periodoString";
	public static final String PROPERTY_DISCIPLINA_ID = "disciplinaId";
	public static final String PROPERTY_DISCIPLINA_STRING = "disciplinaString";
	public static final String PROPERTY_DISCIPLINA_NOME = "disciplinaNome";
	public static final String PROPERTY_TOTAL_CRETIDOS_DISCIPLINA = "totalCreditoDisciplina";
	public static final String PROPERTY_OFERTA_ID = "ofertaId";
	public static final String PROPERTY_TURMA = "turma";
	public static final String PROPERTY_QUANTIDADE_ALUNOS = "quantidadeAlunos";
	public static final String PROPERTY_QUANTIDADE_ALUNOS_STRING = "quantidadeAlunosString";
	public static final String PROPERTY_CREDITOS_TEORICOS = "creditosTeorico";
	public static final String PROPERTY_CREDITOS_PRATICOS = "creditosPratico";
	
	public AtendimentoTaticoDTO() {
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
	
	public void setCenarioId(Long value) {
		set(PROPERTY_CENARIO_ID, value);
	}
	public Long getCenarioId() {
		return get(PROPERTY_CENARIO_ID);
	}
	
	public void setCampusId(Long value) {
		set(PROPERTY_CAMPUS_ID, value);
	}
	public Long getCampusId() {
		return get(PROPERTY_CAMPUS_ID);
	}
	
	public String getCampusString() {
		return get(PROPERTY_CAMPUS_STRING);
	}
	public void setCampusString(String value) {
		set(PROPERTY_CAMPUS_STRING, value);
	}
	
	public void setUnidadeId(Long value) {
		set(PROPERTY_UNIDADE_ID, value);
	}
	public Long getUnidadeId() {
		return get(PROPERTY_UNIDADE_ID);
	}
	
	public String getUnidadeString() {
		return get(PROPERTY_UNIDADE_STRING);
	}
	public void setUnidadeString(String value) {
		set(PROPERTY_UNIDADE_STRING, value);
	}
	
	public void setSalaId(Long value) {
		set(PROPERTY_SALA_ID, value);
	}
	public Long getSalaId() {
		return get(PROPERTY_SALA_ID);
	}
	
	public String getSalaString() {
		return get(PROPERTY_SALA_STRING);
	}
	public void setSalaString(String value) {
		set(PROPERTY_SALA_STRING, value);
	}

	public void setSemana(Integer value) {
		set(PROPERTY_DIA_SEMANA, value);
	}
	public Integer getSemana() {
		return get(PROPERTY_DIA_SEMANA);
	}
	
	public void setCursoString(String value) {
		set(PROPERTY_CURSO_STRING, value);
	}
	public String getCursoString() {
		return get(PROPERTY_CURSO_STRING);
	}
	
	public void setCursoNome(String value) {
		set(PROPERTY_CURSO_NOME, value);
	}
	public String getCursoNome() {
		return get(PROPERTY_CURSO_NOME);
	}
	
	public void setCurricularString(String value) {
		set(PROPERTY_CURRICULO_STRING, value);
	}
	public String getCurriculoString() {
		return get(PROPERTY_CURRICULO_STRING);
	}
	
	public void setPeriodo(Integer value) {
		set(PROPERTY_PERIODO, value);
	}
	public Integer getPeriodo() {
		return get(PROPERTY_PERIODO);
	}
	
	public void setPeriodoString(String value) {
		set(PROPERTY_PERIODO_STRING, value);
	}
	public String getPeriodoString() {
		return get(PROPERTY_PERIODO_STRING);
	}
	
	public void setDisciplinaId(Long value) {
		set(PROPERTY_DISCIPLINA_ID, value);
	}
	public Long getDisciplinaId() {
		return get(PROPERTY_DISCIPLINA_ID);
	}
	
	public String getDisciplinaString() {
		return get(PROPERTY_DISCIPLINA_STRING);
	}
	public void setDisciplinaString(String value) {
		set(PROPERTY_DISCIPLINA_STRING, value);
	}
	
	public String getDisciplinaNome() {
		return get(PROPERTY_DISCIPLINA_NOME);
	}
	public void setDisciplinaNome(String value) {
		set(PROPERTY_DISCIPLINA_NOME, value);
	}
	
	public void setTotalCreditoDisciplina(Integer value) {
		set(PROPERTY_TOTAL_CRETIDOS_DISCIPLINA, value);
	}
	public Integer getTotalCreditoDisciplina() {
		return get(PROPERTY_TOTAL_CRETIDOS_DISCIPLINA);
	}
	
	public void setOfertaId(Long value) {
		set(PROPERTY_OFERTA_ID, value);
	}
	public Long getOfertaId() {
		return get(PROPERTY_OFERTA_ID);
	}
	
	public String getTurma() {
		return get(PROPERTY_TURMA);
	}
	public void setTurma(String value) {
		set(PROPERTY_TURMA, value);
	}
	
	public void setQuantidadeAlunos(Integer value) {
		set(PROPERTY_QUANTIDADE_ALUNOS, value);
	}
	public Integer getQuantidadeAlunos() {
		return get(PROPERTY_QUANTIDADE_ALUNOS);
	}
	
	public void setQuantidadeAlunosString(String value) {
		set(PROPERTY_QUANTIDADE_ALUNOS_STRING, value);
	}
	public String getQuantidadeAlunosString() {
		return get(PROPERTY_QUANTIDADE_ALUNOS_STRING);
	}
	
	public void setCreditosTeorico(Integer value) {
		set(PROPERTY_CREDITOS_TEORICOS, value);
	}
	public Integer getCreditosTeorico() {
		return get(PROPERTY_CREDITOS_TEORICOS);
	}
	
	public void setCreditosPratico(Integer value) {
		set(PROPERTY_CREDITOS_PRATICOS, value);
	}
	public Integer getCreditosPratico() {
		return get(PROPERTY_CREDITOS_PRATICOS);
	}
	
	public boolean isTeorico() {
		return getCreditosTeorico() > 0;
	}
	
	public Integer getTotalCreditos() {
		return getCreditosTeorico() + getCreditosPratico();
	}
	
	public void concatenateVisaoSala(AtendimentoTaticoDTO other) {
		setCursoNome(getCursoNome() + " / " + other.getCursoNome());
		setCurricularString(getCurriculoString() + " / " + other.getCurriculoString());
		setPeriodoString(getPeriodoString() + " / " + other.getPeriodoString());
		setQuantidadeAlunosString(getQuantidadeAlunosString() + " / " + other.getQuantidadeAlunosString());
		setQuantidadeAlunos(getQuantidadeAlunos() + other.getQuantidadeAlunos());
	}
	
	public void concatenateVisaoCurso(AtendimentoTaticoDTO other) {
		setDisciplinaString(getDisciplinaString() + "/" + other.getDisciplinaString());
		setTurma(getTurma() + "/" + other.getTurma());
		setCampusString(getCampusString() + "/" + other.getCampusString());
		setUnidadeString(getUnidadeString() + "/" + other.getUnidadeString());
		setSalaString(getSalaString() + "/" + other.getSalaString());
		setQuantidadeAlunosString(getQuantidadeAlunosString() + "/" + other.getQuantidadeAlunosString());
	}
	
	public String getContentVisaoSala() {
		return getDisciplinaString() + "<br />"
		+ TriedaUtil.truncate(getDisciplinaNome(),12) + "<br />"
		+ "Turma " + getTurma() + "<br />"
		//+ TriedaUtil.truncate(getCursoNome(),12) + "<br />"
		+ getQuantidadeAlunosString() + " aluno(s)";
	}
	
	public String getContentToolTipVisaoSala() {
		return "<b>Turma:</b> "+ getTurma() + "<br />"
		+ "<b>Crédito(s) " + ((isTeorico())? "Teórico(s)" : "Prático(s)") + ":</b> " + getTotalCreditos()+" de "+getTotalCreditoDisciplina() + "<br />"
		+ "<b>Curso:</b> " + getCursoNome() +"<br />"
		+ "<b>Matriz Curricular:</b> " + getCurriculoString() + "<br />"
		+ "<b>Período:</b> "+ getPeriodoString() +"<br />" 
		+ "<b>Quantidade:</b> "+ getQuantidadeAlunosString() +"<br />";
	}
	
	public String getExcelContentVisaoSala() {
		return getDisciplinaString() + " / " + getTurma();
	}
	
	public String getExcelCommentVisaoSala() {
		return getDisciplinaNome() + "\n"
		+ "Turma: "+ getTurma() + "\n"
		+ "Crédito(s) " + ((isTeorico())? "Teórico(s)" : "Prático(s)") + ": " + getTotalCreditos()+" de "+getTotalCreditoDisciplina() + "\n"
		+ "Curso: " + getCursoNome() + "\n"
		+ "Matriz Curricular: " + getCurriculoString() + "\n"
		+ "Período: "+ getPeriodoString() + "\n" 
		+ "Quantidade: "+ getQuantidadeAlunosString();
	}
	
	@Override
	public String getNaturalKey() {
		return getCampusString() + "-" + getUnidadeString() + "-" + getSalaString() + "-" +
		       getSemana() + "-" +
		       getCursoString() + "-" + getCurriculoString() + "-" + getPeriodo() + "-" +
		       getDisciplinaString() + "-" + getTurma();
	}

	@Override
	public int compareTo(AtendimentoTaticoDTO o) {
		return 0;
	}

	@Override
	public String toString() {
		return getDisciplinaString()+"@"+getTurma()+"@"+getSalaString()+"@"+getSemana();
	}

	static public boolean compatibleByApproach1(AtendimentoTaticoDTO dto1, AtendimentoTaticoDTO dto2) {
		return dto1.getDisciplinaId().equals(dto2.getDisciplinaId()) && 
			   !dto1.getSalaId().equals(dto2.getSalaId()) &&
			   !dto1.getTurma().equals(dto2.getTurma()) &&
			   dto1.getTotalCreditos().equals(dto2.getTotalCreditos()) &&
			   dto1.getSemana().equals(dto2.getSemana());
	}
	
	static public boolean compatibleByApproach2(AtendimentoTaticoDTO dto1, AtendimentoTaticoDTO dto2) {
		return !dto1.getDisciplinaId().equals(dto2.getDisciplinaId()) && 
			   !dto1.getSalaId().equals(dto2.getSalaId()) &&
			   !dto1.getTurma().equals(dto2.getTurma()) &&
			   dto1.getTotalCreditos().equals(dto2.getTotalCreditos()) &&
			   dto1.getSemana().equals(dto2.getSemana());
	}
	
	static public int countListDTOsCreditos(List<AtendimentoTaticoDTO> listDTOs) {
		int count = 0;
		for (AtendimentoTaticoDTO dto : listDTOs) {
			count += dto.getTotalCreditos();
		}
		return count;
	}
	
	static public int countListListDTOsCreditos(List<List<AtendimentoTaticoDTO>> listListDTOs) {
		int count = 0;
		for (List<AtendimentoTaticoDTO> listDTOs : listListDTOs) {
			count += listDTOs.get(0).getTotalCreditos();
		}
		return count;
	}
}
