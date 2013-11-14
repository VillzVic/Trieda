package com.gapso.web.trieda.shared.dtos;

public class TipoProfessorDTO extends AbstractDTO<String> implements Comparable<TipoProfessorDTO> {

	/**
	 * 
	 */
	private static final long serialVersionUID = -5795593581854552872L;
	public static final String PROPERTY_NOME = "nome";
	public static final String PROPERTY_INSTITUCIONAL = "institucional";
	public static final String PROPERTY_VIRTUAL = "virtual";
	
	public TipoProfessorDTO() {
		super();
	}
	
	public String getNome() {
		return get(PROPERTY_NOME);
	}
	public void setNome(String value) {
		set(PROPERTY_NOME, value);
	}
	
	public Boolean getInstitucional() {
		return get(PROPERTY_INSTITUCIONAL);
	}
	public void setInstitucional(Boolean value) {
		set(PROPERTY_INSTITUCIONAL, value);
	}
	
	public Boolean getVirtual() {
		return get(PROPERTY_VIRTUAL);
	}
	public void setVirtual(Boolean value) {
		set(PROPERTY_VIRTUAL, value);
	}
	
	@Override
	public String getNaturalKey() {
		return getNome();
	}

	@Override
	public int compareTo(TipoProfessorDTO o) {
		return getNome().compareTo(o.getNome());
	}
}
