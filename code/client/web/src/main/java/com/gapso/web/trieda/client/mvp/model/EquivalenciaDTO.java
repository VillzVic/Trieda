package com.gapso.web.trieda.client.mvp.model;



public class EquivalenciaDTO extends FileModel {

	private static final long serialVersionUID = 5815525344760896272L;
	
	public EquivalenciaDTO() {
	}

	/* == PROPRIEDADES ==
	 * Long   : id
	 * Integer: version
	 * Long   : cursouId
	 * String : cursouString
	 * String : eliminaString
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
	
	public void setCursouId(Long value) {
		set("cursouId", value);
	}
	public Long getCursouId() {
		return get("cursouId");
	}

	public void setCursouString(String value) {
		set("cursouString", value);
	}
	public String getCursouString() {
		return get("cursouString");
	}
	
	public void setEliminaString(String value) {
		set("eliminaString", value);
	}
	public String getEliminaString() {
		return get("eliminaString");
	}

}
