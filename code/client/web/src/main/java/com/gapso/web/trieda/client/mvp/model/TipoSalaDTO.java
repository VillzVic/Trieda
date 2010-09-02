package com.gapso.web.trieda.client.mvp.model;

import com.extjs.gxt.ui.client.data.BaseModel;


public class TipoSalaDTO extends BaseModel {

	private static final long serialVersionUID = 5815525344760896272L;
	
	public TipoSalaDTO() {
	}

	public TipoSalaDTO(Long id, String nome, String descricao, Integer version) {
		setId(id);
		setNome(nome);
		setDescricao(descricao);
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

	public void setNome(String value) {
		set("nome", value);
	}
	public String getNome() {
		return get("nome");
	}

	public void setDescricao(String value) {
		set("descricao", value);
	}
	public String getDescricao() {
		return get("descricao");
	}
	
	
}
