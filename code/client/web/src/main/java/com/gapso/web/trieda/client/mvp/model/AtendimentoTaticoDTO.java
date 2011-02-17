package com.gapso.web.trieda.client.mvp.model;

import com.extjs.gxt.ui.client.data.BaseModel;

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
	
	public void setQuantidadeAlunos(Integer value) {
		set("quantidadeAlunos", value);
	}
	public Integer getQuantidadeAlunos() {
		return get("quantidadeAlunos");
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
	
}
