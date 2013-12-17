package com.gapso.web.trieda.shared.dtos;


public class TipoDisciplinaDTO extends AbstractDTO<String> implements Comparable<TipoDisciplinaDTO> {

	private static final long serialVersionUID = -5134820110949139907L;
	
	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_NOME = "nome";
	public static final String PROPERTY_CENARIO_ID = "cenarioId";

	public TipoDisciplinaDTO() {
		super();
	}

	public TipoDisciplinaDTO(Long id, String nome, Integer version) {
		setId(id);
		setNome(nome);
		setVersion(version);
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
	
	public String getNome() {
		return get(PROPERTY_NOME);
	}
	public void setNome(String value) {
		set(PROPERTY_NOME, value);
	}
	
	public void setCenarioId(Long value) {
		set(PROPERTY_CENARIO_ID, value);
	}
	public Long getCenarioId() {
		return get(PROPERTY_CENARIO_ID);
	}
	
	@Override
	public String getNaturalKey() {
		return getNome();
	}

	@Override
	public int compareTo(TipoDisciplinaDTO o) {
		return getNome().compareTo(o.getNome());
	}
}
