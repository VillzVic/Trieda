package com.gapso.web.trieda.client.mvc.model;

import com.extjs.gxt.ui.client.data.BaseModel;

public class ProfessorDisciplinaModel extends BaseModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public ProfessorDisciplinaModel() {
		super();
	}

	public ProfessorDisciplinaModel(String cpf, String nome, String disciplina, Integer preferencia, String nota) {
		setCpf(cpf);
		setNome(nome);
		setDisciplina(disciplina);
		setPreferencia(preferencia);
		setNota(nota);
	}
	
	public String getCpf() {
		return get("cpf");
	}
	public void setCpf(String value) {
		set("cpf", value);
	}

	public String getNome() {
		return get("nome");
	}
	public void setNome(String value) {
		set("nome", value);
	}
	
	public String getDisciplina() {
		return get("disciplina");
	}
	public void setDisciplina(String value) {
		set("disciplina", value);
	}
	
	public Integer getPreferenciaMinima() {
		return get("preferencia");
	}
	public void setPreferencia(Integer value) {
		set("preferencia", value);
	}
	
	public String getNota() {
		return get("nota");
	}
	public void setNota(String value) {
		set("nota", value);
	}
	
}
