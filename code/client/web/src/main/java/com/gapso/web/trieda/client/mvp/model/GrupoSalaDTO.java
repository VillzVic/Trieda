package com.gapso.web.trieda.client.mvp.model;

import com.extjs.gxt.ui.client.data.BaseModel;

public class GrupoSalaDTO extends BaseModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public GrupoSalaDTO() {
		super();
	}

	public GrupoSalaDTO(Long id, String codigo, String nome, Long unidadeId, String unidadeString, Integer version) {
		setId(id);
		setCodigo(codigo);
		setNome(nome);
		setUnidadeId(unidadeId);
		setUnidadeString(unidadeString);
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
	
	public Long getUnidadeId() {
		return get("unidadeId");
	}
	public void setUnidadeId(Long value) {
		set("unidadeId", value);
	}

	public String getUnidadeString() {
		return get("unidadeString");
	}
	public void setUnidadeString(String value) {
		set("unidadeString", value);
	}
	
}
