package com.gapso.web.trieda.client.mvp.model;

import com.extjs.gxt.ui.client.data.BaseModel;

public class TurnoModel extends BaseModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public TurnoModel() {
		super();
	}

	public TurnoModel(String nome) {
		setNome(nome);
	}

	public String getNome() {
		return get("nome");
	}
	public void setNome(String value) {
		set("nome", value);
	}
	
}
