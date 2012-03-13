package com.gapso.web.trieda.shared.dtos;

import com.extjs.gxt.ui.client.data.BaseModel;

public class QuartetoDTO<P, S, T, Q> extends BaseModel {
	private static final long serialVersionUID = 8409587132857717405L;
	
	private P primeiro;
	private S segundo;
	private T terceiro;
	private Q quarto;
	
	static public <P,S,T,Q> QuartetoDTO<P,S,T,Q> create(P primeiro, S segundo, T terceiro, Q quarto) {
		return new QuartetoDTO<P,S,T,Q>(primeiro,segundo,terceiro,quarto);
	}

	private QuartetoDTO() {
		super();
	}

	private QuartetoDTO(P primeiro, S segundo, T terceiro, Q quarto) {
		super();
		this.primeiro = primeiro;
		this.segundo = segundo;
		this.terceiro = terceiro;
		this.quarto = quarto;
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
}