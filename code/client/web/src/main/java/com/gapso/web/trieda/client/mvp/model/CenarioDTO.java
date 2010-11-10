package com.gapso.web.trieda.client.mvp.model;

import java.util.Date;

import com.extjs.gxt.ui.client.data.BaseModel;

public class CenarioDTO extends BaseModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public CenarioDTO() {
		super();
	}

	/* == PROPRIDADES ==
	 * Long: id
	 * Integer: version
	 * Boolean: masterData
	 * Boolean: oficial
	 * Integer: ano
	 * Integer: semestre
	 * Long   : semanaLetivaId
	 * String : semanaLetivaString
	 * Long   : criadoUsuarioId
	 * Long   : atualizadoUsuarioId
	 * String : criadoUsuarioString
	 * String : atualizadoUsuarioString
	 * Date   : criadoUsuarioDate
	 * Date   : atualizadoUsuarioDate
	 * String : comentario
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
	
	public void setNome(String value) {
		set("nome", value);
	}
	public String getNome() {
		return get("nome");
	}
	
	public void setMasterData(Boolean value) {
		set("masterData", value);
	}
	public Boolean getMasterData() {
		return get("masterData");
	}
	
	public void setOficial(Boolean value) {
		set("oficial", value);
	}
	public Boolean getOficial() {
		return get("oficial");
	}
	
	public void setAno(Integer value) {
		set("ano", value);
	}
	public Integer getAno() {
		return get("ano");
	}
	
	public void setSemestre(Integer value) {
		set("semestre", value);
	}
	public Integer getSemestre() {
		return get("semestre");
	}
	
	public void setSemanaLetivaId(Long value) {
		set("semanaLetivaId", value);
	}
	public Long getSemanaLetivaId() {
		return get("semanaLetivaId");
	}
	
	public void setSemanaLetivaString(String value) {
		set("semanaLetivaString", value);
	}
	public String getsemanaLetivaString() {
		return get("semanaLetivaString");
	}
	
	public void setCriadoUsuarioId(Long value) {
		set("criadoUsuarioId", value);
	}
	public Long getCriadoUsuarioId() {
		return get("criadoUsuarioId");
	}
	
	public void setAtualizadoUsuarioId(Long value) {
		set("atualizadoUsuarioId", value);
	}
	public Long getAtualizadoUsuarioId() {
		return get("atualizadoUsuarioId");
	}
	
	public void setCriadoUsuarioString(String value) {
		set("criadoUsuarioString", value);
	}
	public String getCriadoUsuarioString() {
		return get("criadoUsuarioString");
	}
	
	public void setAtualizadoUsuarioString(String value) {
		set("atualizadoUsuarioString", value);
	}
	public String getAtualizadoUsuarioString() {
		return get("atualizadoUsuarioString");
	}
	
	public void setCriadoUsuarioDate(Date value) {
		set("criadoUsuarioDate", value);
	}
	public Date getCriadoUsuarioDate() {
		return get("criadoUsuarioDate");
	}
	
	public void setAtualizadoUsuarioDate(Date value) {
		set("atualizadoUsuarioDate", value);
	}
	public Date getAtualizadoUsuarioDate() {
		return get("atualizadoUsuarioDate");
	}
	
	public void setComentario(String value) {
		set("comentario", value);
	}
	public String getComentario() {
		return get("comentario");
	}
	
}
