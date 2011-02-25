package com.gapso.web.trieda.client.mvp.model;

import java.util.HashMap;
import java.util.Map;



public class TurnoDTO extends FileModel {

	private static final long serialVersionUID = 5815525344760896272L;
	
	private Map<Integer,Integer> countHorariosAula;
	
	public TurnoDTO() {
		countHorariosAula = new HashMap<Integer, Integer>();
	}

	/* == PROPRIEDADES ==
	 * Long   : id
	 * Integer: version
	 * Long   : cenarioId
	 * String : nome
	 * Integer: tempo
	 * Integer: maxCreditos
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

	public void setNome(String value) {
		set("nome", value);
	}
	public String getNome() {
		return get("nome");
	}

	public void setTempo(Integer value) {
		set("tempo", value);
	}
	public Integer getTempo() {
		return get("tempo");
	}
	
	public void setMaxCreditos(Integer value) {
		set("maxCreditos", value);
	}
	public Integer getMaxCreditos() {
		return get("maxCreditos");
	}

	public Map<Integer, Integer> getCountHorariosAula() {
		return countHorariosAula;
	}

	public void setCountHorariosAula(Map<Integer, Integer> countHorariosAula) {
		this.countHorariosAula = countHorariosAula;
	}
	
	public int getMaxCreditos(int diaSemana) {
		Integer value = countHorariosAula.get(diaSemana);
		return (value != null) ? value : 0;
	}

	@Override
	public String toString() {
		return getNome() + " (" + getTempo() +"min)";
	}
	
}
