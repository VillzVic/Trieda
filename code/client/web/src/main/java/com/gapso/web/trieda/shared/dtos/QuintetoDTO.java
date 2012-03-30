package com.gapso.web.trieda.shared.dtos;

import com.extjs.gxt.ui.client.data.BaseModel;

public class QuintetoDTO<P, S, T, Q, Qui> extends BaseModel {
	private static final long serialVersionUID = 8409587132857717405L;
	
	private P primeiro;
	private S segundo;
	private T terceiro;
	private Q quarto;
	private Qui quinto;
	
	static public <P,S,T,Q,Qui> QuintetoDTO<P,S,T,Q,Qui> create(P primeiro, S segundo, T terceiro, Q quarto, Qui quinto) {
		return new QuintetoDTO<P,S,T,Q,Qui>(primeiro,segundo,terceiro,quarto,quinto);
	}

	public QuintetoDTO() {
		super();
	}

	private QuintetoDTO(P primeiro, S segundo, T terceiro, Q quarto, Qui quinto) {
		super();
		this.primeiro = primeiro;
		this.segundo = segundo;
		this.terceiro = terceiro;
		this.quarto = quarto;
		this.quinto = quinto;
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
	
	public Q getQuarto() {
		return quarto;
	}

	public void setQuarto(Q quarto) {
		this.quarto = quarto;
	}
	
	public Qui getQuinto() {
		return quinto;
	}

	public void setQuinto(Qui quinto) {
		this.quinto = quinto;
	}
}