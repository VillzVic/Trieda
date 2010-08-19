package com.gapso.web.trieda.client.mvc.model;

import com.extjs.gxt.ui.client.data.BaseModel;

public class UnidadeDeslocamentoModel extends BaseModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public UnidadeDeslocamentoModel() {
		super();
	}

	public UnidadeDeslocamentoModel(String name, Double d1, Double d2, Double d3, Double d4, Double d5) {
		set("name", name);
		set("unidade1", d1);
		set("unidade2", d2);
		set("unidade3", d3);
		set("unidade4", d4);
		set("unidade5", d5);
	}
	
	
}
