package com.gapso.web.trieda.client.mvc.model;

import com.extjs.gxt.ui.client.data.BaseModel;

public class CalendarioModel extends BaseModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public CalendarioModel() {
		super();
	}

	public CalendarioModel(String codigo, String descricao, Integer duracaoAula) {
		setCodigo(codigo);
		setDescricao(descricao);
		setDuracaoAula(duracaoAula);
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
	
	public Integer getDuracaoAula() {
		return get("duracao_aula");
	}
	public void setDuracaoAula(Integer value) {
		set("duracao_aula", value);
	}
	
}
