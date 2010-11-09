package com.gapso.web.trieda.client.mvp.model;


public class CurriculoDTO extends FileModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public CurriculoDTO() {
		super();
	}

	public CurriculoDTO(Long id, String codigo, String descricao, Long cursoId, String cursoString, Integer qtdPeriodos, Integer version) {
		setId(id);
		setCodigo(codigo);
		setDescricao(descricao);
		setCursoId(cursoId);
		setCursoString(cursoString);
		setQtdPeriodos(qtdPeriodos);
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
	
	public String getDescricao() {
		return get("descricao");
	}
	public void setDescricao(String value) {
		set("descricao", value);
	}
	
	public Long getCursoId() {
		return get("cursoId");
	}
	public void setCursoId(Long value) {
		set("cursoId", value);
	}
	
	public String getCursoString() {
		return get("cursoString");
	}
	public void setCursoString(String value) {
		set("cursoString", value);
	}
	
	public void setQtdPeriodos(Integer value) {
		set("qtdPeriodos", value);
	}
	public Integer getQtdPeriodos() {
		return get("qtdPeriodos");
	}

}
