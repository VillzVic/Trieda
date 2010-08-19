package com.gapso.web.trieda.client.mvc.model;

import com.extjs.gxt.ui.client.data.BaseModel;

public class CampusModel extends BaseModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public CampusModel() {
		super();
	}

	public CampusModel(String codigo, String nome) {
		setCodigo(codigo);
		setNome(nome);
	}
	
	public String getCodigo() {
		return get("codigo");
	}
	public void setCodigo(String value) {
		set("codigo", value);
	}

	public String getNome() {
		return get("nome");
	}
	public void setNome(String value) {
		set("nome", value);
	}
	
}
