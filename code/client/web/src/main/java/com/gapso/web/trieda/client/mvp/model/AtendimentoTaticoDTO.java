package com.gapso.web.trieda.client.mvp.model;

import java.util.List;

import com.extjs.gxt.ui.client.data.BaseModel;
import com.gapso.web.trieda.shared.util.TriedaUtil;

public class AtendimentoTaticoDTO extends BaseModel {

	private static final long serialVersionUID = -2870302894382757778L;
	
	public AtendimentoTaticoDTO() {
	}

	/* == PROPRIEDADES ==
	 * Long   : id
	 * Integer: version
	 * Long   : cenarioId
	 * String : turma
	 * Long   : campusId
	 * String : campusString
	 * Long   : unidadeId
	 * String : unidadeString
	 * Long   : salaId
	 * String : salaString
	 * Integer: semana
	 * Long   : ofertaId
	 * Long   : disciplinaId
	 * String : disciplinaString
	 * Integer: quantidadeAlunos
	 * String: quantidadeAlunosString
	 * Integer: creditosTeorico
	 * Integer: creditosPratico
	 */
	
	public void setId(Long value) {
		set("id", value);
	}
	public Long getId() {
		return get("id");
	}
	
	public void setVersion(Integer value) {
		set("version", value);
	}
	public Integer getVersion() {
		return get("version");
	}
	
	public void setCenarioId(Long value) {
		set("cenarioId", value);
	}
	public Long getCenarioId() {
		return get("cenarioId");
	}
	
	public String getTurma() {
		return get("turma");
	}
	public void setTurma(String value) {
		set("turma", value);
	}
	
	public void setCampusId(Long value) {
		set("campusId", value);
	}
	public Long getCampusId() {
		return get("campusId");
	}
	
	public String getCampusString() {
		return get("campusString");
	}
	public void setCampusString(String value) {
		set("campusString", value);
	}
	
	public void setUnidadeId(Long value) {
		set("unidadeId", value);
	}
	public Long getUnidadeId() {
		return get("unidadeId");
	}
	
	public String getUnidadeString() {
		return get("unidadeString");
	}
	public void setUnidadeString(String value) {
		set("unidadeString", value);
	}
	
	public void setSalaId(Long value) {
		set("salaId", value);
	}
	public Long getSalaId() {
		return get("salaId");
	}
	
	public String getSalaString() {
		return get("salaString");
	}
	public void setSalaString(String value) {
		set("salaString", value);
	}

	public void setSemana(Integer value) {
		set("semana", value);
	}
	public Integer getSemana() {
		return get("semana");
	}
	
	public void setOfertaId(Long value) {
		set("ofertaId", value);
	}
	public Long getOfertaId() {
		return get("ofertaId");
	}
	
	public void setDisciplinaId(Long value) {
		set("disciplinaId", value);
	}
	public Long getDisciplinaId() {
		return get("disciplinaId");
	}
	
	public String getDisciplinaString() {
		return get("disciplinaString");
	}
	public void setDisciplinaString(String value) {
		set("disciplinaString", value);
	}
	
	public String getDisciplinaNome() {
		return get("disciplinaNome");
	}
	public void setDisciplinaNome(String value) {
		set("disciplinaNome", value);
	}
	
	public void setQuantidadeAlunos(Integer value) {
		set("quantidadeAlunos", value);
	}
	public Integer getQuantidadeAlunos() {
		return get("quantidadeAlunos");
	}
	
	public void setQuantidadeAlunosString(String value) {
		set("quantidadeAlunosString", value);
	}
	public String getQuantidadeAlunosString() {
		return get("quantidadeAlunosString");
	}
	
	public void setCreditosTeorico(Integer value) {
		set("creditosTeorico", value);
	}
	public Integer getCreditosTeorico() {
		return get("creditosTeorico");
	}
	
	public void setCreditosPratico(Integer value) {
		set("creditosPratico", value);
	}
	public Integer getCreditosPratico() {
		return get("creditosPratico");
	}
	
	public void setCursoString(String value) {
		set("cursoString", value);
	}
	public String getCursoString() {
		return get("cursoString");
	}
	
	public void setCursoNome(String value) {
		set("cursoNome", value);
	}
	public String getCursoNome() {
		return get("cursoNome");
	}
	
	public void setCurricularString(String value) {
		set("curriculoString", value);
	}
	public String getCurriculoString() {
		return get("curriculoString");
	}
	
	public void setPeriodo(Integer value) {
		set("periodo", value);
	}
	public Integer getPeriodo() {
		return get("periodo");
	}
	
	public void setPeriodoString(String value) {
		set("periodoString", value);
	}
	public String getPeriodoString() {
		return get("periodoString");
	}
	
	public void setTotalCreditoDisciplina(Integer value) {
		set("totalCreditoDisciplina", value);
	}
	public Integer getTotalCreditoDisciplina() {
		return get("totalCreditoDisciplina");
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
