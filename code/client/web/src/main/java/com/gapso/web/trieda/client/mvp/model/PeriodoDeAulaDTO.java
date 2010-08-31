package com.gapso.web.trieda.client.mvp.model;

import java.util.Date;

import com.extjs.gxt.ui.client.data.BaseModel;


public class PeriodoDeAulaDTO extends BaseModel {

	private static final long serialVersionUID = -4670030478798237916L;
	
	public PeriodoDeAulaDTO() {
	}

	public PeriodoDeAulaDTO(Long id, Long calendario, Long turno, Date inicio, Date fim, Integer version) {
		setId(id);
		setCalendario(calendario);
		setTurno(turno);
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

	public void setCalendario(Long value) {
		set("calendario", value);
	}
	public Long getCalendario() {
		return get("calendario");
	}

	public void setTurno(Long value) {
		set("turno", value);
	}
	public Long getTurno() {
		return get("turno");
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
