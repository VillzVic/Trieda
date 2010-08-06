package com.gapso.web.trieda.client.controller.mvc.model;

import com.extjs.gxt.ui.client.data.BaseModel;

public class SalaModel extends BaseModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public SalaModel() {
		super();
	}

	public SalaModel(String codigo, String unidade, String numSala, String andar, String tipo, Integer capacidade) {
		setCodigo(codigo);
		setUnidade(unidade);
		setNumSala(numSala);
		setAndar(andar);
		setTipo(tipo);
		setCapacidade(capacidade);
	}
	
	public String getCodigo() {
		return get("codigo");
	}
	public void setCodigo(String value) {
		set("codigo", value);
	}

	public String getUnidade() {
		return get("unidade");
	}
	public void setUnidade(String value) {
		set("unidade", value);
	}
	
	public String getNumSala() {
		return get("num_sala");
	}
	public void setNumSala(String value) {
		set("num_sala", value);
	}
	
	public String getAndar() {
		return get("andar");
	}
	public void setAndar(String value) {
		set("andar", value);
	}
	
	public String getTipo() {
		return get("tipo");
	}
	public void setTipo(String value) {
		set("tipo", value);
	}

	public Integer getCapacidade() {
		return get("capacidade");
	}
	public void setCapacidade(Integer value) {
		set("capacidade", value);
	}
	
	
	
	
	
}
