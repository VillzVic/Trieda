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
	
	@Override
	public String getNaturalKey() {
		return getUsername();
	}	

	@Override
	public int compareTo(UsuarioDTO o) {
		return getNome().compareTo(o.getNome());
	}
}