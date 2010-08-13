package com.gapso.web.trieda.client.mvc.model;

import com.extjs.gxt.ui.client.data.BaseModel;
import com.extjs.gxt.ui.client.widget.form.Time;

public class PeriodoAulaModel extends BaseModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public PeriodoAulaModel() {
		super();
	}

	public PeriodoAulaModel(String calendario, String turno, Time inicio, Time fim) {
		setCalendario(calendario);
		setTurno(turno);
		setInicio(inicio);
		setFim(fim);
	}
	
	public String getCalendario() {
		return get("calendario");
	}
	public void setCalendario(String value) {
		set("calendario", value);
	}

	public String getTurno() {
		return get("turno");
	}
	public void setTurno(String value) {
		set("turno", value);
	}
	
	public Time getInicio() {
		return get("inicio");
	}
	public void setInicio(Time value) {
		set("inicio", value);
	}
	
	public Time getFim() {
		return get("fim");
	}
	public void setFim(Time value) {
		set("fim", value);
	}
	
	
}
