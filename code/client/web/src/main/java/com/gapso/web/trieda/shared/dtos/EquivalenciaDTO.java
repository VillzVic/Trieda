package com.gapso.web.trieda.shared.dtos;




public class EquivalenciaDTO extends AbstractDTO<String> implements Comparable<EquivalenciaDTO> {

	private static final long serialVersionUID = 5815525344760896272L;
	
	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_CURSOU_ID = "cursouId";
	public static final String PROPERTY_CURSOU_STRING = "cursouString";
	public static final String PROPERTY_ELIMINA_STRING = "eliminaString";
	
	public EquivalenciaDTO() {
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
	
	public void setCursouId(Long value) {
		set(PROPERTY_CURSOU_ID, value);
	}
	public Long getCursouId() {
		return get(PROPERTY_CURSOU_ID);
	}

	public void setCursouString(String value) {
		set(PROPERTY_CURSOU_STRING, value);
	}
	public String getCursouString() {
		return get(PROPERTY_CURSOU_STRING);
	}
	
	public void setEliminaString(String value) {
		set(PROPERTY_ELIMINA_STRING, value);
	}
	public String getEliminaString() {
		return get(PROPERTY_ELIMINA_STRING);
	}
	
	@Override
	public String getNaturalKey() {
		return getCursouString() + "-" + getEliminaString();
	}

	@Override
	public int compareTo(EquivalenciaDTO o) {
		int result = getCursouString().compareTo(o.getCursouString());
		if (result == 0) {
			result = getEliminaString().compareTo(o.getEliminaString());
		}
		return result;
	}
}
