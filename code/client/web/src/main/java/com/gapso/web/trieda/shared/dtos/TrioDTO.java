package com.gapso.web.trieda.shared.dtos;

import com.extjs.gxt.ui.client.data.BaseModel;

public class TrioDTO<P, S, T> extends BaseModel {
	private static final long serialVersionUID = 8409587132857717405L;
	
	private P primeiro;
	private S segundo;
	private T terceiro;
	
	static public <P,S,T> TrioDTO<P,S,T> create(P primeiro, S segundo, T terceiro) {
		return new TrioDTO<P,S,T>(primeiro,segundo,terceiro);
	}

	private TrioDTO() {
		super();
	}

	private TrioDTO(P primeiro, S segundo, T terceiro) {
		super();
		this.primeiro = primeiro;
		this.segundo = segundo;
		this.terceiro = terceiro;
	}

	public P getPrimeiro() {
		return primeiro;
	}

	public void setPrimeiro(P primeiro) {
		this.primeiro = primeiro;
	}

	public S getSegundo() {
		return segundo;
	}

	public void setSegundo(S segundo) {
		this.segundo = segundo;
	}
	
	public T getTerceiro() {
		return terceiro;
	}

	public void setTerceiro(T terceiro) {
		this.terceiro = terceiro;
	}
}
