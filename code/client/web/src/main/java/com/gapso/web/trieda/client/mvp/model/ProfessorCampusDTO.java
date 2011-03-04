package com.gapso.web.trieda.client.mvp.model;

import com.gapso.web.trieda.shared.dtos.AbstractDTO;



public class ProfessorCampusDTO extends AbstractDTO<String> implements Comparable<ProfessorCampusDTO> {

	private static final long serialVersionUID = 5815525344760896272L;
	
	public ProfessorCampusDTO() {
	}

	// Propriedades
	public static final String PROPERTY_PROFESSOR_ID = "professorId";
	public static final String PROPERTY_PROFESSOR_STRING = "professorString";
	public static final String PROPERTY_PROFESSOR_CPF = "professorCpf";
	public static final String PROPERTY_CAMPUS_ID = "campusId";
	public static final String PROPERTY_CAMPUS_STRING = "campusString";
	
	public void setProfessorId(Long value) {
		set(PROPERTY_PROFESSOR_ID, value);
	}
	public Long getProfessorId() {
		return get(PROPERTY_PROFESSOR_ID);
	}

	public void setProfessorString(String value) {
		set(PROPERTY_PROFESSOR_STRING, value);
	}
	public String getProfessorString() {
		return get(PROPERTY_PROFESSOR_STRING);
	}
	
	public void setProfessorCpf(String value) {
		set(PROPERTY_PROFESSOR_CPF, value);
	}
	public String getProfessorCpf() {
		return get(PROPERTY_PROFESSOR_CPF);
	}
	
	public void setCampusId(Long value) {
		set(PROPERTY_CAMPUS_ID, value);
	}
	public Long getCampusId() {
		return get(PROPERTY_CAMPUS_ID);
	}
	
	public void setCampusString(String value) {
		set(PROPERTY_CAMPUS_STRING, value);
	}
	public String getCampusString() {
		return get(PROPERTY_CAMPUS_STRING);
	}
	
	@Override
	public String getNaturalKey() {
		return getCampusString() + "-" + getProfessorCpf();
	}

	@Override
	public int compareTo(ProfessorCampusDTO o) {
		int result = getCampusString().compareTo(o.getCampusString());
		if (result == 0) {
			result = getProfessorString().compareTo(o.getProfessorString());
		}
		return result;
	}
}