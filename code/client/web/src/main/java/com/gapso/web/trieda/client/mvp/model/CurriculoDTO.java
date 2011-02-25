package com.gapso.web.trieda.client.mvp.model;


public class CurriculoDTO extends FileModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public CurriculoDTO() {
		super();
	}

	/* == PROPRIEDADES ==
	 * Long   : id
	 * Integer: version
	 * Long   : cenarioId
	 * String : codigo
	 * String : descricao
	 * Long   : cursoId
	 * String : cursoString
	 * String : periodos
	 * String : display
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
	
	public void setCenarioId(Long value) {
		set("cenarioId", value);
	}
	public Long getCenarioId() {
		return get("cenarioId");
	}
	
	public String getCodigo() {
		return get("codigo");
	}
	public void setCodigo(String value) {
		set("codigo", value);
	}
	
	public String getDescricao() {
		return get("descricao");
	}
	public void setDescricao(String value) {
		set("descricao", value);
	}
	
	public Long getCursoId() {
		return get("cursoId");
	}
	public void setCursoId(Long value) {
		set("cursoId", value);
	}
	
	public String getCursoString() {
		return get("cursoString");
	}
	public void setCursoString(String value) {
		set("cursoString", value);
	}
	
	public void setPeriodos(String value) {
		set("periodos", value);
	}
	public String getPeriodos() {
		return get("periodos");
	}
	
	public String getDisplay() {
		return get("display");
	}
	public void setDisplay(String value) {
		set("display", value);
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((getCodigo() == null) ? 0 : getCodigo().hashCode());
		return result;
	}
	
	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		CurriculoDTO other = (CurriculoDTO) obj;
		if (getCodigo() == null) {
			if (other.getCodigo() != null)
				return false;
		} else if (!getCodigo().equals(other.getCodigo()))
			return false;
		return true;
	}
}
