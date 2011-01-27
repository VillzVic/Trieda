package com.gapso.web.trieda.client.mvp.model;

import com.extjs.gxt.ui.client.data.BaseModel;



public class ProfessorCampusDTO extends BaseModel {

	private static final long serialVersionUID = 5815525344760896272L;
	
	public ProfessorCampusDTO() {
	}

	/* == PROPRIEDADES ==
	 * Long   : professorId
	 * String : professorString
	 * String : professorCpf
	 * Long   : campusId
	 * String : campusString
	 */
	
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
	
	public void setCampusId(Long value) {
		set("campusId", value);
	}
	public Long getCampusId() {
		return get("campusId");
	}
	
	public void setCampusString(String value) {
		set("campusString", value);
	}
	public String getCampusString() {
		return get("campusString");
	}
	
}
