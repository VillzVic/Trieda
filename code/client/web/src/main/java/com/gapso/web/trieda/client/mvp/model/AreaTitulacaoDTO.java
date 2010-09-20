package com.gapso.web.trieda.client.mvp.model;

import com.extjs.gxt.ui.client.data.BaseModel;

public class AreaTitulacaoDTO extends BaseModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public AreaTitulacaoDTO() {
		super();
	}

	public AreaTitulacaoDTO(Long id, String codigo, String descricao, Integer version) {
		setId(id);
		setCodigo(codigo);
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
	
	public String getCodigo() {
		return get("codigo");
	}
	public void setCodigo(String value) {
		set("codigo", value);
	}

	public String getDescricao() {
		return get("descricao");
	}
	public void setDescricao(String value) {
		set("descricao", value);
	}
	
}
