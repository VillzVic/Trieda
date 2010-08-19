package com.gapso.web.trieda.client.mvc.model;

import com.extjs.gxt.ui.client.data.BaseModel;

public class EquivalenciaModel extends BaseModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public EquivalenciaModel() {
		super();
	}

	public EquivalenciaModel(String cursou, String elimina) {
		setCursou(cursou);
		setElimina(elimina);
	}
	
	public String getCursou() {
		return get("cursou");
	}
	public void setCursou(String value) {
		set("cursou", value);
	}
	
	public String getElimina() {
		return get("elimina");
	}
	public void setElimina(String value) {
		set("elimina", value);
	}
	
}
