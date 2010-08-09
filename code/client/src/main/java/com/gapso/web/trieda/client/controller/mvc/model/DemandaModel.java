package com.gapso.web.trieda.client.controller.mvc.model;

import com.extjs.gxt.ui.client.data.BaseModel;

public class DemandaModel extends BaseModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public DemandaModel() {
		super();
	}
	
	public DemandaModel(String campus, String turno, String curso, String disciplina, Integer demanda) {
		setCampus(campus);
		setTurno(turno);
		setCurso(curso);
		setDisciplina(disciplina);
		setDemanda(demanda);
	}
	
	public String getCampus() {
		return get("campus");
	}
	public void setCampus(String value) {
		set("campus", value);
	}

	public String getTurno() {
		return get("turno");
	}
	public void setTurno(String value) {
		set("turno", value);
	}
	
	public String getCurso() {
		return get("curso");
	}
	public void setCurso(String value) {
		set("curso", value);
	}
	
	public String getDisciplina() {
		return get("disciplina");
	}
	public void setDisciplina(String value) {
		set("disciplina", value);
	}
	
	public Integer getDemanda() {
		return get("demanda");
	}
	public void setDemanda(Integer value) {
		set("demanda", value);
	}
		
}
