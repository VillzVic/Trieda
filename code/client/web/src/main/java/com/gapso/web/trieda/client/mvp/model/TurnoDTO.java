package com.gapso.web.trieda.client.mvp.model;



public class TurnoDTO extends FileModel {

	private static final long serialVersionUID = 5815525344760896272L;
	
	public TurnoDTO() {
	}

	public TurnoDTO(Long id, String nome, Integer tempo, Integer version) {
		setId(id);
		setNome(nome);
		setTempo(tempo);
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

	public void setNome(String value) {
		set("nome", value);
	}
	public String getNome() {
		return get("nome");
	}

	public void setTempo(Integer value) {
		set("tempo", value);
	}
	public Integer getTempo() {
		return get("tempo");
	}

	@Override
	public String toString() {
		return getNome() + "(" + getTempo() +"min)";
	}
	
}
