package com.gapso.web.trieda.client.mvp.model;

import java.io.Serializable;


public class TurnoDTO implements Serializable {

	private String nome;
	private Integer tempo;

	public TurnoDTO() {
	}

	public TurnoDTO(String nome, Integer tempo) {
		this.nome = nome;
		this.tempo = tempo;
	}

	public String getNome() {
		return nome;
	}
	public void setNome(String nome) {
		this.nome = nome;
	}

	public Integer getTempo() {
		return tempo;
	}
	public void setTempo(Integer tempo) {
		this.tempo = tempo;
	}
}
