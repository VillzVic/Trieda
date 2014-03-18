package com.gapso.web.trieda.shared.dtos;

public class MotivoUsoProfessorVirtualDTO extends AbstractDTO<Long> implements Comparable<MotivoUsoProfessorVirtualDTO> {

	private static final long serialVersionUID = 6333517672121702790L;

	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_MOTIVO = "motivo";
	public static final String PROPERTY_PROFESSOR_STRING = "professorString";
	public static final String PROPERTY_PROFESSOR_CPF = "professorCpf";
	
	public MotivoUsoProfessorVirtualDTO()
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

	public void setMotivo(String value) {
		set(PROPERTY_MOTIVO, value);
	}
	public String getMotivo() {
		return get(PROPERTY_MOTIVO);
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
	public int compareTo(MotivoUsoProfessorVirtualDTO o) {
		return this.getId().compareTo(o.getId());
	}

	@Override
	public Long getNaturalKey() {
		return getId();
	}

}
