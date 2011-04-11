package com.gapso.web.trieda.shared.dtos;


public class CursoDescompartilhaDTO extends AbstractDTO<Long> implements Comparable<CursoDescompartilhaDTO> {

	private static final long serialVersionUID = 5815525344760896272L;
	
	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_PARAMETRO_ID = "parametroId";
	public static final String PROPERTY_CURSO1_ID = "curso1Id";
	public static final String PROPERTY_CURSO1_DISPLAY = "curso1Display";
	public static final String PROPERTY_CURSO2_ID = "curso2Id";
	public static final String PROPERTY_CURSO2_DISPLAY = "curso2Display";
	

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
	
	public void setParametroId(Long value) {
		set(PROPERTY_PARAMETRO_ID, value);
	}
	public Long getParametroId() {
		return get(PROPERTY_PARAMETRO_ID);
	}
	
	public void setCurso1Id(Long value) {
		set(PROPERTY_CURSO1_ID, value);
	}
	public Long getCurso1Id() {
		return get(PROPERTY_CURSO1_ID);
	}

	public void setCurso1Display(String value) {
		set(PROPERTY_CURSO1_DISPLAY, value);
	}
	public String getCurso1Display() {
		return get(PROPERTY_CURSO1_DISPLAY);
	}
	
	public void setCurso2Id(Long value) {
		set(PROPERTY_CURSO2_ID, value);
	}
	public Long getCurso2Id() {
		return get(PROPERTY_CURSO2_ID);
	}
	
	public void setCurso2Display(String value) {
		set(PROPERTY_CURSO2_DISPLAY, value);
	}
	public String getCurso2Display() {
		return get(PROPERTY_CURSO2_DISPLAY);
	}
	
	@Override
	public Long getNaturalKey() {
		return getId();
	}	

	@Override
	public int compareTo(CursoDescompartilhaDTO o) {
		return getCurso1Display().compareTo(o.getCurso1Display());
	}
}