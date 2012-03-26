package com.gapso.web.trieda.shared.dtos;

import com.extjs.gxt.ui.client.data.BaseModel;

public class SextetoDTO<P, S, T, Q, Qui, Sex> extends BaseModel {
	private static final long serialVersionUID = 8409587132857717405L;
	
	private P primeiro;
	private S segundo;
	private T terceiro;
	private Q quarto;
	private Qui quinto;
	private Sex sexto;
	
	static public <P,S,T,Q,Qui,Sex> SextetoDTO<P,S,T,Q,Qui,Sex> create(P primeiro, S segundo, T terceiro, Q quarto, Qui quinto, Sex sexto) {
		return new SextetoDTO<P,S,T,Q,Qui,Sex>(primeiro,segundo,terceiro,quarto,quinto,sexto);
	}

	private SextetoDTO() {
		super();
	}

	private SextetoDTO(P primeiro, S segundo, T terceiro, Q quarto, Qui quinto, Sex sexto) {
		super();
		this.primeiro = primeiro;
		this.segundo = segundo;
		this.terceiro = terceiro;
		this.quarto = quarto;
		this.quinto = quinto;
		this.sexto = sexto;
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
	
	public Sex getSexto() {
		return sexto;
	}

	public void setSexto(Sex sexto) {
		this.sexto = sexto;
	}
}
