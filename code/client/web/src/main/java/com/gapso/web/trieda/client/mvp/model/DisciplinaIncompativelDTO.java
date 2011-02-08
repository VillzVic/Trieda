package com.gapso.web.trieda.client.mvp.model;

import com.extjs.gxt.ui.client.data.BaseModel;


public class DisciplinaIncompativelDTO extends BaseModel {

	private static final long serialVersionUID = 2804545521094014835L;
	
	public DisciplinaIncompativelDTO() {
		super();
	}

	/* == PROPRIEDADES ==
	 * Long   : disciplina1Id
	 * Long   : disciplina2Id
	 * String : disciplina1String
	 * String : disciplina2String
	 * Boolean: incompativel
	 */
	
	public void setDisciplina1Id(Long value) {
		set("disciplina1Id", value);
	}
	public Long getDisciplina1Id() {
		return get("disciplina1Id");
	}
	
	public void setDisciplina2Id(Long value) {
		set("disciplina2Id", value);
	}
	public Long getDisciplina2Id() {
		return get("disciplina2Id");
	}
	
	public void setDisciplina1String(String value) {
		set("disciplina1String", value);
	}
	public String getDisciplina1String() {
		return get("disciplina1String");
	}
	
	public void setDisciplina2String(String value) {
		set("disciplina2String", value);
	}
	public String getDisciplina2String() {
		return get("disciplina2String");
	}
	
	public Boolean getIncompativel() {
		return get("incompativel");
	}
	public void setIncompativel(Boolean value) {
		set("incompativel", value);
	}

}
