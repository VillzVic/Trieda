package com.gapso.web.trieda.client.mvc.model;

import com.extjs.gxt.ui.client.data.BaseModel;

public class UnidadeModel extends BaseModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public UnidadeModel() {
		super();
	}

	public UnidadeModel(String codigo, String nome, String campus, Integer capSalas) {
		setCodigo(codigo);
		setNome(nome);
		setCampus(campus);
		setCapSalas(capSalas);
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
	
	public String getCampus() {
		return get("campus");
	}
	public void setCampus(String value) {
		set("campus", value);
	}
	
	public Integer getCapSalas() {
		return get("cap_salas");
	}
	public void setCapSalas(Integer value) {
		set("cap_salas", value);
	}
	
	
	
	
}
