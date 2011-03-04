package com.gapso.web.trieda.shared.dtos;



public class TipoSalaDTO extends AbstractDTO<String> implements Comparable<TipoSalaDTO> {

	private static final long serialVersionUID = 5815525344760896272L;
	
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_NOME = "nome";
	public static final String PROPERTY_DESCRICAO = "descricao";
	
	public TipoSalaDTO() {
	}

	public TipoSalaDTO(Long id, String nome, String descricao, Integer version) {
		setId(id);
		setNome(nome);
		setDescricao(descricao);
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

	public void setNome(String value) {
		set(PROPERTY_NOME, value);
	}
	public String getNome() {
		return get(PROPERTY_NOME);
	}

	public void setDescricao(String value) {
		set(PROPERTY_DESCRICAO, value);
	}
	public String getDescricao() {
		return get(PROPERTY_DESCRICAO);
	}
	
	@Override
	public String getNaturalKey() {
		return getNome();
	}

	@Override
	public int compareTo(TipoSalaDTO o) {
		return getNome().compareTo(o.getNome());
	}	
}