package com.gapso.web.trieda.client.mvc.model;

import com.extjs.gxt.ui.client.data.BaseModel;

public class CursoCampusModel extends BaseModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public CursoCampusModel() {
		super();
	}
	
	public CursoCampusModel(String turno, String campus, String curso, String matrizCurricular) {
		setTurno(turno);
		setCampus(campus);
		setCurso(curso);
		setMatrizCurricular(matrizCurricular);
	}

	public String getTurno() {
		return get("turno");
	}
	public void setTurno(String value) {
		set("turno", value);
	}

	public String getCampus() {
		return get("campus");
	}
	public void setCampus(String value) {
		set("campus", value);
	}

	public String getCurso() {
		return get("curso");
	}
	public void setCurso(String value) {
		set("curso", value);
	}

	public String getMatrizCurricular() {
		return get("matrizcurricular");
	}
	public void setMatrizCurricular(String value) {
		set("matrizcurricular", value);
	}

	
}
