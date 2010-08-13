package com.gapso.web.trieda.client.mvc.model;

import com.extjs.gxt.ui.client.data.BaseModel;

public class MatrizCurricularModel extends BaseModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public MatrizCurricularModel() {
		super();
	}
	
	public MatrizCurricularModel(String codigo, String descricao, String curso, Integer numPeriodos) {
		setCodigo(codigo);
		setDescricao(descricao);
		setCurso(curso);
		setNumPeriodos(numPeriodos);
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
	
	public String getCurso() {
		return get("curso");
	}
	public void setCurso(String value) {
		set("curso", value);
	}
	
	public Integer getNumPeriodos() {
		return get("num_periodos");
	}
	public void setNumPeriodos(Integer value) {
		set("num_periodos", value);
	}

	
	
	
	
}
