package com.gapso.web.trieda.client.mvp.model;

import com.extjs.gxt.ui.client.data.BaseModel;

public class UnidadeDTO extends BaseModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public UnidadeDTO() {
		super();
	}

	public UnidadeDTO(Long id, String codigo, String nome, Long campusId, String campusString, Integer capSalas, Integer version) {
		setId(id);
		setCodigo(codigo);
		setNome(nome);
		setCampusId(campusId);
		setCampusString(campusString);
		setCapSalas(capSalas);
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
	
	public Long getCampusId() {
		return get("campusId");
	}
	public void setCampusId(Long value) {
		set("campusId", value);
	}
	
	public String getCampusString() {
		return get("campusString");
	}
	public void setCampusString(String value) {
		set("campusString", value);
	}
	
	public Integer getCapSalas() {
		return get("capSalas");
	}
	public void setCapSalas(Integer value) {
		set("capSalas", value);
	}

	@Override
	public String toString() {
		return getCodigo();
	}
}