package com.gapso.web.trieda.client.mvp.model;

import com.gapso.web.trieda.shared.dtos.AbstractDTO;


public class SemanaLetivaDTO extends AbstractDTO<String> implements Comparable<SemanaLetivaDTO> {

	private static final long serialVersionUID = -5419695301010396477L;
	
	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_CODIGO = "codigo";
	public static final String PROPERTY_DESCRICAO = "descricao";
	
	public SemanaLetivaDTO() {
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

	public void setCodigo(String value) {
		set(PROPERTY_CODIGO, value);
	}
	public String getCodigo() {
		return get(PROPERTY_CODIGO);
	}
	
	public void setDescricao(String value) {
		set(PROPERTY_DESCRICAO, value);
	}
	public String getDescricao() {
		return get(PROPERTY_DESCRICAO);
	}
	
	@Override
	public String getNaturalKey() {
		return getCodigo();
	}

	@Override
	public int compareTo(SemanaLetivaDTO o) {
		return getCodigo().compareTo(o.getCodigo());
	}	
}