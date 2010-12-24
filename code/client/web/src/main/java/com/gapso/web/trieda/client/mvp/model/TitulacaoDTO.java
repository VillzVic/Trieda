package com.gapso.web.trieda.client.mvp.model;

import com.extjs.gxt.ui.client.data.BaseModel;

public class TitulacaoDTO extends BaseModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public TitulacaoDTO() {
		super();
	}

	public TitulacaoDTO(Long id, String nome, Integer version) {
		setId(id);
		setVersion(version);
		setNome(nome);
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
	
	public String getNome() {
		return get("nome");
	}
	public void setNome(String value) {
		set("nome", value);
	}

}
