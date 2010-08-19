package com.gapso.web.trieda.client.mvc.model;

import com.extjs.gxt.ui.client.data.BaseModel;

public class CampusDeslocamentoModel extends BaseModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public CampusDeslocamentoModel() {
		super();
	}

	public CampusDeslocamentoModel(String name, Double c1, Double c2, Double c3, Double c4, Double c5) {
		set("name", name);
		set("campus1", c1);
		set("campus2", c2);
		set("campus3", c3);
		set("campus4", c4);
		set("campus5", c5);
	}
	
	
}
