package com.gapso.web.trieda.client.mvc.model;

import com.extjs.gxt.ui.client.data.BaseModel;

public class CampusProfessorModel extends BaseModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public CampusProfessorModel() {
		super();
	}

	public CampusProfessorModel(String campus, String cpf, String nome) {
		setCampus(campus);
		setCpf(cpf);
		setNome(nome);
	}
	
	public String getCampus() {
		return get("campus");
	}
	public void setCampus(String value) {
		set("campus", value);
	}
	
	public String getCpf() {
		return get("cpf");
	}
	public void setCpf(String value) {
		set("cpf", value);
	}

	public String getNome() {
		return get("nome");
	}
	public void setNome(String value) {
		set("nome", value);
	}
	
}
