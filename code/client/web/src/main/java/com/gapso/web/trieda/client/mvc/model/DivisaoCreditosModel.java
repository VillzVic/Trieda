package com.gapso.web.trieda.client.mvc.model;

import com.extjs.gxt.ui.client.data.BaseModel;

public class DivisaoCreditosModel extends BaseModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public DivisaoCreditosModel() {
		super();
	}

	public DivisaoCreditosModel(Integer creditos, Integer dia1, Integer dia2, Integer dia3, Integer dia4, Integer dia5, Integer dia6, Integer dia7) {
		setCreditos(creditos);
		setDia1(dia1);
		setDia2(dia2);
		setDia3(dia3);
		setDia4(dia4);
		setDia5(dia5);
		setDia6(dia6);
		setDia7(dia7);
	}
	
	public Integer getCreditos() {
		return get("creditos");
	}
	public void setCreditos(Integer value) {
		set("creditos", value);
	}

	public Integer getDia1() {
		return get("dia1");
	}
	public void setDia1(Integer value) {
		set("dia1", value);
	}
	
	public Integer getDia2() {
		return get("dia2");
	}
	public void setDia2(Integer value) {
		set("dia2", value);
	}
	
	public Integer getDia3() {
		return get("dia3");
	}
	public void setDia3(Integer value) {
		set("dia3", value);
	}
	
	public Integer getDia4() {
		return get("dia4");
	}
	public void setDia4(Integer value) {
		set("dia4", value);
	}
	
	public Integer getDia5() {
		return get("dia5");
	}
	public void setDia5(Integer value) {
		set("dia5", value);
	}
	
	public Integer getDia6() {
		return get("dia6");
	}
	public void setDia6(Integer value) {
		set("dia6", value);
	}
	
	public Integer getDia7() {
		return get("dia7");
	}
	public void setDia7(Integer value) {
		set("dia7", value);
	}
}
