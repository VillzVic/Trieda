package com.gapso.web.trieda.client.mvp.model;

import com.extjs.gxt.ui.client.data.BaseModel;



public class DivisaoCreditoDTO extends BaseModel {

	private static final long serialVersionUID = 7603464268140278420L;

	public DivisaoCreditoDTO() {
	}

	/* == PROPRIEDADES ==
	 * Long   : id
	 * Integer: version
	 * Long   : cenarioId
	 * Long   : disciplinaId
	 * String : disciplinaString
	 * Integer: totalCreditos
	 * Integer: dia1
	 * Integer: dia2
	 * Integer: dia3
	 * Integer: dia4
	 * Integer: dia5
	 * Integer: dia6
	 * Integer: dia7
	 */
	
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
	
	public void setCenarioId(Long value) {
		set("cenarioId", value);
	}
	public Long getCenarioId() {
		return get("cenarioId");
	}
	
	public void setDisciplinaId(Long value) {
		set("disciplinaId", value);
	}
	public Long getDisciplinaId() {
		return get("disciplinaId");
	}
	
	public void setDisciplinaString(String value) {
		set("disciplinaString", value);
	}
	public String getDisciplinaString() {
		return get("disciplinaString");
	}

	public void setTotalCreditos(Integer value){ set("totalCreditos", value); }
	public Integer getTotalCreditos() 		   { return get("totalCreditos"); }
	
	public void setDia1(Integer value) { set("dia1", value); }
	public Integer getDia1() 		   { return get("dia1"); }
	
	public void setDia2(Integer value) { set("dia2", value); }
	public Integer getDia2() 		   { return get("dia2"); }
	
	public void setDia3(Integer value) { set("dia3", value); }
	public Integer getDia3() 		   { return get("dia3"); }
	
	public void setDia4(Integer value) { set("dia4", value); }
	public Integer getDia4() 		   { return get("dia4"); }
	
	public void setDia5(Integer value) { set("dia5", value); }
	public Integer getDia5() 		   { return get("dia5"); }
	
	public void setDia6(Integer value) { set("dia6", value); }
	public Integer getDia6() 		   { return get("dia6"); }
	
	public void setDia7(Integer value) { set("dia7", value); }
	public Integer getDia7() 		   { return get("dia7"); }
	
}
