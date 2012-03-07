package com.gapso.web.trieda.shared.dtos;

import com.extjs.gxt.ui.client.data.BaseModel;

public class ParDTO<P, S> extends BaseModel
{
	private static final long serialVersionUID = 3153799238112011798L;

	private P primeiro;
	private S segundo;
	
	static public <P,S> ParDTO<P,S> create(P primeiro, S segundo) {
		return new ParDTO<P,S>(primeiro,segundo);
	}

	private ParDTO() {
		super();
	}

	private ParDTO(P primeiro, S segundo) {
		super();
		this.primeiro = primeiro;
		this.segundo = segundo;
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
}
