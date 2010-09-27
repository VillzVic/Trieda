package com.gapso.web.trieda.client.mvp.model;

import com.extjs.gxt.ui.client.data.BaseModel;

public class DisciplinaDTO extends BaseModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public DisciplinaDTO() {
		super();
	}

	public DisciplinaDTO(Long id, String codigo, String nome, Integer creditosTeorico, Integer creditosPratico, Boolean laboratorio, Long tipoId, String tipoString, String dificuldade, Integer maxAlunosTeorico, Integer maxAlunosPratico, Integer version) {
		setId(id);
		setCodigo(codigo);
		setNome(nome);
		setCreditosTeorico(creditosTeorico);
		setCreditosPratico(creditosPratico);
		setLaboratorio(laboratorio);
		setTipoId(tipoId);
		setTipoString(tipoString);
		setDificuldade(dificuldade);
		setMaxAlunosTeorico(maxAlunosTeorico);
		setMaxAlunosPratico(maxAlunosPratico);
		setVersion(version);
	}
	
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
	
	public void setCreditosTeorico(Integer value) {
		set("creditosTeorico", value);
	}
	public Integer getCreditosTeorico() {
		return get("creditosTeorico");
	}
	
	public void setCreditosPratico(Integer value) {
		set("creditosPratico", value);
	}
	public Integer getCreditosPratico() {
		return get("creditosPratico");
	}
	
	public void setLaboratorio(Boolean value) {
		set("laboratorio", value);
	}
	public Boolean getLaboratorio() {
		return get("laboratorio");
	}
	
	public Long getTipoId() {
		return get("tipoId");
	}
	public void setTipoId(Long value) {
		set("tipoId", value);
	}
	
	public String getTipoString() {
		return get("tipoString");
	}
	public void setTipoString(String value) {
		set("tipoString", value);
	}
	
	public String getDificuldade() {
		return get("dificuldade");
	}
	public void setDificuldade(String value) {
		set("dificuldade", value);
	}
	
	public Integer getMaxAlunosTeorico() {
		return get("maxAlunosTeorico");
	}
	public void setMaxAlunosTeorico(Integer value) {
		set("maxAlunosTeorico", value);
	}
	
	public Integer getMaxAlunosPratico() {
		return get("maxAlunosPratico");
	}
	public void setMaxAlunosPratico(Integer value) {
		set("maxAlunosPratico", value);
	}

}
