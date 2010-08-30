package com.gapso.web.trieda.client.mvp.model;

import com.extjs.gxt.ui.client.data.BaseModel;


public class CalendarioDTO extends BaseModel {

	private static final long serialVersionUID = -5419695301010396477L;
	
	public CalendarioDTO() {
	}

	public CalendarioDTO(Long id, String codigo, String descricao, Integer version) {
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

	public void setCodigo(String value) {
		set("codigo", value);
	}
	public String getCodigo() {
		return get("codigo");
	}
	
	public void setDescricao(String value) {
		set("descricao", value);
	}
	public String getDescricao() {
		return get("descricao");
	}
	
	
}
