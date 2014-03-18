package com.gapso.web.trieda.shared.dtos;

public class DicaEliminacaoProfessorVirtualDTO extends AbstractDTO<Long> implements Comparable<DicaEliminacaoProfessorVirtualDTO> {

	private static final long serialVersionUID = -7312915848377134649L;

	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_DICA = "dica";
	public static final String PROPERTY_PROFESSOR_STRING = "professorString";
	public static final String PROPERTY_PROFESSOR_CPF = "professorCpf";
	
	public DicaEliminacaoProfessorVirtualDTO()
	{
		super();
	}

	public void setId(Long value) {
		set(PROPERTY_ID, value);
	}
	public Long getId() {
		return get(PROPERTY_ID);
	}
	
	public void setVersion(Integer value) {
		set(PROPERTY_VERSION, value);
	}
	public Integer getVersion() {
		return get(PROPERTY_VERSION);
	}

	public void setDica(String value) {
		set(PROPERTY_DICA, value);
	}
	public String getDica() {
		return get(PROPERTY_DICA);
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

	@Override
	public Long getNaturalKey() {
		return getId();
	}

	@Override
	public int compareTo(DicaEliminacaoProfessorVirtualDTO o) {
		return getId().compareTo(o.getId());
	}

}
