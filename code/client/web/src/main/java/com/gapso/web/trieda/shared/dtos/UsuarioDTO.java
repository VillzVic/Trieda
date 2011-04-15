package com.gapso.web.trieda.shared.dtos;






public class UsuarioDTO extends AbstractDTO<String> implements Comparable<UsuarioDTO> {

	private static final long serialVersionUID = 5815525344760896272L;
	
	// Propriedades
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_NOME = "nime";
	public static final String PROPERTY_EMAIL = "email";
	public static final String PROPERTY_USERNAME = "username";
	public static final String PROPERTY_PASSWORD = "maxpassword";
	public static final String PROPERTY_ENABLED = "enabled";
	public static final String PROPERTY_PROFESSOR_ID = "professorId";
	public static final String PROPERTY_PROFESSOR_DISPLAYTEXT = "professorDisplayText";
	
	public UsuarioDTO() {
	}
	
	public void setVersion(Integer value) {
		set(PROPERTY_VERSION, value);
	}
	public Integer getVersion() {
		return get(PROPERTY_VERSION);
	}
	
	public void setNome(String value) {
		set(PROPERTY_NOME, value);
	}
	public String getNome() {
		return get(PROPERTY_NOME);
	}
	
	public void setEmail(String value) {
		set(PROPERTY_EMAIL, value);
	}
	public String getEmail() {
		return get(PROPERTY_EMAIL);
	}
	
	public void setUsername(String value) {
		set(PROPERTY_USERNAME, value);
	}
	public String getUsername() {
		return get(PROPERTY_USERNAME);
	}
	
	public void setPassword(String value) {
		set(PROPERTY_PASSWORD, value);
	}
	public String getPassword() {
		return get(PROPERTY_PASSWORD);
	}

	public void setEnabled(Boolean value) {
		set(PROPERTY_ENABLED, value);
	}
	public Boolean getEnabled() {
		return get(PROPERTY_ENABLED);
	}
	
	public void setProfessorId(Long value) {
		set(PROPERTY_PROFESSOR_ID, value);
	}
	public Long getProfessorId() {
		return get(PROPERTY_PROFESSOR_ID);
	}
	
	public void setProfessorDisplayText(String value) {
		set(PROPERTY_PROFESSOR_DISPLAYTEXT, value);
	}
	public String getProfessorDisplayText() {
		return get(PROPERTY_PROFESSOR_DISPLAYTEXT);
	}
	
	public boolean isProfessor() {
		return getProfessorId() != null && getProfessorId() > 0;
	}
	
	@Override
	public String getNaturalKey() {
		return getUsername();
	}	

	@Override
	public int compareTo(UsuarioDTO o) {
		return getNome().compareTo(o.getNome());
	}
}