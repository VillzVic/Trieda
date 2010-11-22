package com.gapso.web.trieda.client.mvp.model;



public class DemandaDTO extends FileModel {

	private static final long serialVersionUID = 5815525344760896272L;
	
	public DemandaDTO() {
	}

	/* == PROPRIEDADES ==
	 * Long   : id
	 * Integer: version
	 * Long   : cenarioId
	 * Long   : ofertaId
	 * Long   : campusId
	 * String : campusString
	 * Long   : cursoId
	 * String : cursoString
	 * Long   : curriculoId
	 * String : curriculoString
	 * Long   : turnoId
	 * String : turnoString
	 * Long   : disciplinaId
	 * String : disciplinaString
	 * Integer: demanda
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
	
	public void setOfertaId(Long value) {
		set("ofertaId", value);
	}
	public Long getOfertaId() {
		return get("ofertaId");
	}
	
	public void setCampusId(Long value) {
		set("campusId", value);
	}
	public Long getCampusId() {
		return get("campusId");
	}
	
	public void setCampusString(String value) {
		set("campusString", value);
	}
	public String getCampusString() {
		return get("campusString");
	}
	
	public void setCursoId(Long value) {
		set("cursoId", value);
	}
	public Long getCursoId() {
		return get("cursoId");
	}
	
	public void setCursoString(String value) {
		set("cursoString", value);
	}
	public String getCursoString() {
		return get("cursoString");
	}
	
	public void setCurriculoId(Long value) {
		set("curriculoId", value);
	}
	public Long getCurriculoId() {
		return get("curriculoId");
	}
	
	public void setCurriculoString(String value) {
		set("curriculoString", value);
	}
	public String getCurriculoString() {
		return get("curriculoString");
	}
	
	public void setTurnoId(Long value) {
		set("turnoId", value);
	}
	public Long getTurnoId() {
		return get("turnoId");
	}
	
	public void setTurnoString(String value) {
		set("turnoString", value);
	}
	public String getTurnoString() {
		return get("turnoString");
	}
	
	public void setDisciplinaId(Long value) {
		set("disciplinaId", value);
	}
	public Long getDisciplinaId() {
		return get("disciplinaId");
	}
	
	public void setDisciplinaString(String value) {
		set("disciplinaString", value);
	}
	public String getDisciplinaString() {
		return get("disciplinaString");
	}
	
	public void setDemanda(Integer value) {
		set("demanda", value);
	}
	public Integer getDemanda() {
		return get("demanda");
	}
	
}
