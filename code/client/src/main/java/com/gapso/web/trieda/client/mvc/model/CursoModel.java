package com.gapso.web.trieda.client.mvc.model;

import com.extjs.gxt.ui.client.data.BaseModel;

public class CursoModel extends BaseModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public CursoModel() {
		super();
	}

	public CursoModel(String codigo, String nome, String tipo, Integer minDoutores, Integer minMestres, Integer maxDisProfessor, Boolean multiDisProfessor) {
		setCodigo(codigo);
		setNome(nome);
		setTipo(tipo);
		setMinDoutores(minDoutores);
		setMinMestres(minMestres);
		setMaxDisProfessor(maxDisProfessor);
		setMultiDisProfessor(multiDisProfessor);
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
	
	public String getTipo() {
		return get("tipo");
	}
	public void setTipo(String value) {
		set("tipo", value);
	}
	
	public Integer getMinDoutores() {
		return get("min_doutores");
	}
	public void setMinDoutores(Integer value) {
		set("min_doutores", value);
	}
	
	public Integer getMinMestres() {
		return get("min_mestres");
	}
	public void setMinMestres(Integer value) {
		set("min_mestres", value);
	}
	
	public Integer getMaxDisProfessor() {
		return get("max_dis_professor");
	}
	public void setMaxDisProfessor(Integer value) {
		set("max_dis_professor", value);
	}
	
	public Boolean getMultiDisProfessor() {
		return get("multi_dis_professor");
	}
	public void setMultiDisProfessor(Boolean value) {
		set("multi_dis_professor", value);
	}
	
}
