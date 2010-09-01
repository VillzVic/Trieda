package com.gapso.web.trieda.client.mvp.model;

import java.util.Date;

import com.extjs.gxt.ui.client.data.BaseModel;


public class HorarioAulaDTO extends BaseModel {

	private static final long serialVersionUID = -4670030478798237916L;
	
	public HorarioAulaDTO() {
	}

	public HorarioAulaDTO(Long id, Long calendarioId, String calendarioString, String turnoString, Long turnoId, Date inicio, Date fim, Integer version) {
		setId(id);
		setCalendarioId(calendarioId);
		setCalendarioString(calendarioString);
		setTurnoId(turnoId);
		setTurnoString(turnoString);
		setInicio(inicio);
		setFim(fim);
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

	public void setCalendarioId(Long value) {
		set("calendarioId", value);
	}
	public Long getCalendarioId() {
		return get("calendarioId");
	}
	
	public void setCalendarioString(String value) {
		set("calendarioString", value);
	}
	public String getCalendarioString() {
		return get("calendarioString");
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
	
	public void setInicio(Date value) {
		set("inicio", value);
	}
	public Date getInicio() {
		return get("inicio");
	}
	
	public void setFim(Date value) {
		set("fim", value);
	}
	public Date getFim() {
		return get("fim");
	}
	
}
