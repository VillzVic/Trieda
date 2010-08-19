package com.gapso.web.trieda.client.mvc.model;

import com.extjs.gxt.ui.client.data.BaseModel;

public class DisciplinaModel extends BaseModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public DisciplinaModel() {
		super();
	}

	public DisciplinaModel(String codigo, String nome, Integer credTeorico, Integer credPratico, Boolean laboratorio, String tipo, String dificuldade, Integer maxAlunTeorico, Integer maxAlunPratico) {
		setCodigo(codigo);
		setNome(nome);
		setCredTeorico(credTeorico);
		setCredPratico(credPratico);
		setLaboratorio(laboratorio);
		setTipo(tipo);
		setDificuldade(dificuldade);
		setMaxAlunTeorico(maxAlunPratico);
		setMaxAlunPratico(maxAlunPratico);
	}
	
	public String getCodigo() {
		return get("codigo");
	}
	public void setCodigo(String value) {
		set("codigo", value);
	}
	
	public String getNome() {
		return get("nome");
	}
	public void setNome(String value) {
		set("nome", value);
	}
	
	public Integer getCredTeorico() {
		return get("cred_teorico");
	}
	public void setCredTeorico(Integer value) {
		set("cred_teorico", value);
	}
	
	public Integer getCredPratico() {
		return get("cred_pratico");
	}
	public void setCredPratico(Integer value) {
		set("cred_pratico", value);
	}
	
	public Boolean getLaboratorio() {
		return get("laboratorio");
	}
	public void setLaboratorio(Boolean value) {
		set("laboratorio", value);
	}
	
	public String getTipo() {
		return get("tipo");
	}
	public void setTipo(String value) {
		set("tipo", value);
	}
	
	public String getDificuldade() {
		return get("dificuldade");
	}
	public void setDificuldade(String value) {
		set("dificuldade", value);
	}
	
	public Integer getMaxAlunTeorico() {
		return get("max_alun_teorico");
	}
	public void setMaxAlunTeorico(Integer value) {
		set("max_alun_teorico", value);
	}

	public Integer getMaxAlunPratico() {
		return get("max_alun_pratico");
	}
	public void setMaxAlunPratico(Integer value) {
		set("max_alun_pratico", value);
	}
	
	
	
	
	
}
