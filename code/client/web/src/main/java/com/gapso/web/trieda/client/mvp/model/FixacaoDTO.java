package com.gapso.web.trieda.client.mvp.model;

import com.extjs.gxt.ui.client.data.BaseModel;



public class FixacaoDTO extends BaseModel {

	private static final long serialVersionUID = 6642146983954267367L;
	
	/* == PROPRIEDADES ==
	 * Long   : id
	 * Integer: version
	 * String : codigo
	 * String : descricao
	 * Long   : discilinaId
	 * String : discilinaString
	 * Long   : campusId
	 * String : campusString
	 * Long   : unidadeId
	 * String : unidadeString
	 * Long   : salaId
	 * String : salaString
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
	
	public void setCodigo(String value) {
		set("codigo", value);
	}
	public String getCodigo() {
		return get("codigo");
	}
	
	public void setDescricao(String value) {
		set("descricao", value);
	}
	public String getDescricao() {
		return get("descricao");
	}
	
	public void setDisciplinaId(Long value) {
		set("discilinaId", value);
	}
	public Long getDisciplinaId() {
		return get("discilinaId");
	}
	
	public void setDisciplinaString(String value) {
		set("discilinaString", value);
	}
	public String getDisciplinaString() {
		return get("discilinaString");
	}
	
	public void setCampusId(Long value) {
		set("campusId", value);
	}
	public Long getCampusId() {
		return get("campusId");
	}
	
	public void setCampusString(String value) {
		set("campusString", value);
	}
	public String getCampusString() {
		return get("campusString");
	}
	
	public void setUnidadeId(Long value) {
		set("unidadeId", value);
	}
	public Long getUnidadeId() {
		return get("unidadeId");
	}
	
	public void setUnidadeString(String value) {
		set("unidadeString", value);
	}
	public String getUnidadeString() {
		return get("unidadeString");
	}
	
	public void setSalaId(Long value) {
		set("salaId", value);
	}
	public Long getSalaId() {
		return get("salaId");
	}
	
	public void setSalaString(String value) {
		set("salaString", value);
	}
	public String getSalaString() {
		return get("salaString");
	}
	
}
