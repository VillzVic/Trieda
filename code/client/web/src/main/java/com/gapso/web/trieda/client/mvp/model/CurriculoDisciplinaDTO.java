package com.gapso.web.trieda.client.mvp.model;


public class CurriculoDisciplinaDTO extends FileModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public CurriculoDisciplinaDTO() {
		super();
	}

	public CurriculoDisciplinaDTO(Long id, Long disciplinaId, String disciplinaString, Integer periodo, Integer creditosTeorico, Integer creditosPratico, Integer creditosTotal, Long curriculoId, Integer version) {
		setId(id);
		setDisciplinaId(disciplinaId);
		setDisciplinaString(disciplinaString);
		setPeriodo(periodo);
		setCreditosTeorico(creditosTeorico);
		setCreditosPratico(creditosPratico);
		setCreditosTotal(creditosTotal);
		setCurriculoId(curriculoId);
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
	
	public Long getDisciplinaId() {
		return get("disciplinaId");
	}
	public void setDisciplinaId(Long value) {
		set("disciplinaId", value);
	}
	
	public String getDisciplinaString() {
		return get("disciplinaString");
	}
	public void setDisciplinaString(String value) {
		set("disciplinaString", value);
	}
	
	public void setPeriodo(Integer value) {
		set("periodo", value);
	}
	public Integer getPeriodo() {
		return get("periodo");
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
	
	public void setCreditosTotal(Integer value) {
		set("creditosTotal", value);
	}
	public Integer getCreditosTotal() {
		return get("creditosTotal");
	}
	
	public void setCurriculoId(Long value) {
		set("curriculoId", value);
	}
	public Long getCurriculoId() {
		return get("curriculoId");
	}
	
	public void setDisciplinaCodigoNomeString(String value) {
		set("disciplinaCodigoNomeString", value);
	}
	public String getDisciplinaCodigoNomeString() {
		return get("disciplinaCodigoNomeString");
	}
	
}
