package com.gapso.web.trieda.client.mvp.model;


public class CampusDTO extends FileModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public CampusDTO() {
	}

	/* == PROPRIEDADES ==
	 * Long   : id
	 * Integer: version
	 * Long   : cenarioId
	 * String : codigo
	 * String : nome
	 * String : estado
	 * String : municipio
	 * String : Bairro
	 * Double : valorCredito
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

	public String getNome() {
		return get("nome");
	}
	public void setNome(String value) {
		set("nome", value);
	}
	
	public String getEstado() {
		return get("estado");
	}
	public void setEstado(String value) {
		set("estado", value);
	}
	
	public String getMunicipio() {
		return get("municipio");
	}
	public void setMunicipio(String value) {
		set("municipio", value);
	}
	
	public String getBairro() {
		return get("bairro");
	}
	public void setBairro(String value) {
		set("bairro", value);
	}
	
	public void setValorCredito(Double value) {
		set("valorCredito", value);
	}
	public Double getValorCredito() {
		return get("valorCredito");
	}

	@Override
	public String getName() {
		return getId().toString();
	}
}
