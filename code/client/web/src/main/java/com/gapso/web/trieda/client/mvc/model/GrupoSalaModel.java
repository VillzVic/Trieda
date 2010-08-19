package com.gapso.web.trieda.client.mvc.model;

import com.extjs.gxt.ui.client.data.BaseModel;

public class GrupoSalaModel extends BaseModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public GrupoSalaModel() {
		super();
	}

	public GrupoSalaModel(String codigo, String descricao) {
		setCodigo(codigo);
		setDescricao(descricao);
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
