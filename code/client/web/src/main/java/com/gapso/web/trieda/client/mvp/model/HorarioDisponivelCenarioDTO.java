package com.gapso.web.trieda.client.mvp.model;

import java.util.Date;

import com.extjs.gxt.ui.client.data.BaseModel;


public class HorarioDisponivelCenarioDTO extends BaseModel implements Comparable<HorarioDisponivelCenarioDTO> {

	private static final long serialVersionUID = -4670030478798237916L;
	
	public HorarioDisponivelCenarioDTO()
	{ }
	
	public void setHorarioDeAulaId(Long value) {
		set("horarioDeAulaId", value);
	}
	public Long getHorarioDeAulaId() {
		return get("horarioDeAulaId");
	}
	
	public void setHorarioDeAulaVersion(Integer value) {
		set("horarioDeAulaVersion", value);
	}
	public Integer getHorarioDeAulaVersion() {
		return get("horarioDeAulaVersion");
	}

	public void setTurnoString(String value) {
		set("turnoString", value);
	}
	public String getTurnoString() {
		return get("turnoString");
	}
	
	public void setHorario(Date value) {
		set("horarioDate", value);
	}
	public Date getHorario() {
		return get("horarioDate");
	}
	
	public void setHorarioString(String value) {
		set("horarioString", value);
	}
	public String getHorarioString() {
		return get("horarioString");
	}
	
	public void setSegunda(Boolean value) { set("segunda", value); }
	public Boolean getSegunda()           { return get("segunda"); }
	public void setSegundaId(Long value)  { set("segundaId", value); }
	public Long getSegundaId()            { return get("segundaId"); }
	
	public void setTerca(Boolean value)   { set("terca", value); }
	public Boolean getTerca()             { return get("terca"); }
	public void setTercaId(Long value) { set("tercaId", value); }
	public Long getTercaId()           { return get("tercaId"); }
	
	public void setQuarta(Boolean value)  { set("quarta", value); }
	public Boolean getQuarta()            { return get("quarta"); }
	public void setQuartaId(Long value){ set("quartaId", value); }
	public Long getQuartaId()          { return get("quartaId"); }
	
	public void setQuinta(Boolean value)  { set("quinta", value); }
	public Boolean getQuinta()            { return get("quinta"); }
	public void setQuintaId(Long value){ set("quintaId", value); }
	public Long getQuintaId()          { return get("quintaId"); }
	
	public void setSexta(Boolean value)   { set("sexta", value); }
	public Boolean getSexta()             { return get("sexta"); }
	public void setSextaId(Long value) { set("sextaId", value); }
	public Long getSextaId()           { return get("sextaId"); }
	
	public void setSabado(Boolean value)  { set("sabado", value); }
	public Boolean getSabado()            { return get("sabado"); }
	public void setSabadoId(Long value){ set("sabadoId", value); }
	public Long getSabadoId()          { return get("sabadoId"); }
	
	public void setDomingo(Boolean value)   { set("domingo", value); }
	public Boolean getDomingo()             { return get("domingo"); }
	public void setDomingoId(Long value) { set("domingoId", value); }
	public Long getDomingoId()           { return get("domingoId"); }

	@Override
	public int compareTo(HorarioDisponivelCenarioDTO o) {
		return getHorario().compareTo(o.getHorario());
	}
	
	
}
