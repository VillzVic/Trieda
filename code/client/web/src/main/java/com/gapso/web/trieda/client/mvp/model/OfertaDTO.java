package com.gapso.web.trieda.client.mvp.model;

import com.extjs.gxt.ui.client.data.BaseModel;

public class OfertaDTO extends BaseModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public OfertaDTO() {
		super();
	}

	public OfertaDTO(Long id, Long campusId, String campusString, String cursoString, Long matrizCurricularId, String matrizCurricularString, Long turnoId, String turnoString, Integer version) {
		setId(id);
		setCampusId(campusId);
		setCampusString(campusString);
		setCursoString(cursoString);
		setMatrizCurricularId(matrizCurricularId);
		setMatrizCurricularString(matrizCurricularString);
		setTurnoId(turnoId);
		setTurnoString(turnoString);
		setVersion(version);
	}
	
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
	
	public Long getCampusId() {
		return get("campusId");
	}
	public void setCampusId(Long value) {
		set("campusId", value);
	}
	
	public String getCampusString() {
		return get("campusString");
	}
	public void setCampusString(String value) {
		set("campusString", value);
	}
	
	public String getCursoString() {
		return get("cursoString");
	}
	public void setCursoString(String value) {
		set("cursoString", value);
	}
	
	public Long getMatrizCurricularId() {
		return get("matrizCurricularId");
	}
	public void setMatrizCurricularId(Long value) {
		set("matrizCurricularId", value);
	}
	
	public String getMatrizCurricularString() {
		return get("matrizCurricularString");
	}
	public void setMatrizCurricularString(String value) {
		set("matrizCurricularString", value);
	}
	
	public Long getTurnoId() {
		return get("turnoId");
	}
	public void setTurnoId(Long value) {
		set("turnoId", value);
	}
	
	public String getTurnoString() {
		return get("turnoString");
	}
	public void setTurnoString(String value) {
		set("turnoString", value);
	}
	
	
}
