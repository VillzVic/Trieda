package com.gapso.web.trieda.client.mvp.model;

import com.extjs.gxt.ui.client.data.BaseModel;



public class ProfessorDisciplinaDTO extends BaseModel {

	private static final long serialVersionUID = 5815525344760896272L;
	
	public ProfessorDisciplinaDTO() {
	}

	/* == PROPRIEDADES ==
	 * Long   : id
	 * Integer: version
	 * Long   : professorId
	 * String : professorString
	 * String : professorCpf
	 * Long   : disciplinaId
	 * String : disciplinaString
	 * Integer: preferencia
	 * Integer: notaDesempenho
	 */
	
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
	
	public void setProfessorId(Long value) {
		set("professorId", value);
	}
	public Long getProfessorId() {
		return get("professorId");
	}

	public void setProfessorString(String value) {
		set("professorString", value);
	}
	public String getProfessorString() {
		return get("professorString");
	}
	
	public void setProfessorCpf(String value) {
		set("professorCpf", value);
	}
	public String getProfessorCpf() {
		return get("professorCpf");
	}
	
	public void setDisciplinaId(Long value) {
		set("disciplinaId", value);
	}
	public Long getDisciplinaId() {
		return get("disciplinaId");
	}
	
	public void setDisciplinaString(String value) {
		set("disciplinaString", value);
	}
	public String getDisciplinaString() {
		return get("disciplinaString");
	}

	public void setPreferencia(Integer value) {
		set("preferencia", value);
	}
	public Integer getPreferencia() {
		return get("preferencia");
	}
	
	public void setNotaDesempenho(Integer value) {
		set("notaDesempenho", value);
	}
	public Integer getNotaDesempenho() {
		return get("notaDesempenho");
	}
	
}
