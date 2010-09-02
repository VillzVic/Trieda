package com.gapso.web.trieda.client.mvp.model;

import com.extjs.gxt.ui.client.data.BaseModel;

public class SalaDTO extends BaseModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public SalaDTO() {
		super();
	}

	public SalaDTO(Long id, String codigo, Long unidadeId, String unidadeString, String numero, String andar, Long tipoId, String tipoString, Integer capacidade, Integer version) {
		setId(id);
		setCodigo(codigo);
		setUnidadeId(unidadeId);
		setUnidadeString(unidadeString);
		setNumero(numero);
		setAndar(andar);
		setTipoId(tipoId);
		setTipoString(tipoString);
		setCapacidade(capacidade);
		setVersion(version);
	}
	
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
	
	public String getCodigo() {
		return get("codigo");
	}
	public void setCodigo(String value) {
		set("codigo", value);
	}
	
	public Long getUnidadeId() {
		return get("unidadeId");
	}
	public void setUnidadeId(Long value) {
		set("unidadeId", value);
	}

	public String getUnidadeString() {
		return get("unidadeString");
	}
	public void setUnidadeString(String value) {
		set("unidadeString", value);
	}
	
	public String getNumero() {
		return get("numero");
	}
	public void setNumero(String value) {
		set("numero", value);
	}
	
	public String getAndar() {
		return get("andar");
	}
	public void setAndar(String value) {
		set("andar", value);
	}
	
	public Long getTipoId() {
		return get("tipoId");
	}
	public void setTipoId(Long value) {
		set("tipoId", value);
	}
	
	public String getTipoString() {
		return get("tipoString");
	}
	public void setTipoString(String value) {
		set("tipoString", value);
	}

	public Integer getCapacidade() {
		return get("capacidade");
	}
	public void setCapacidade(Integer value) {
		set("capacidade", value);
	}
	
	
	
	
	
}
