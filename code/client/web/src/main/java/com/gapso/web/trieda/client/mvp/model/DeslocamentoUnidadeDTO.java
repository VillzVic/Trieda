package com.gapso.web.trieda.client.mvp.model;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.data.BaseModel;

public class DeslocamentoUnidadeDTO extends BaseModel {

	private static final long serialVersionUID = -5134820110949139907L;

	public DeslocamentoUnidadeDTO() {
		super();
		set("destinosIdList", new ArrayList<Long>());
	}

	public void setOrigemId(Long value) {
		set("origemId", value);
	}
	public Long getOrigemId() {
		return get("origemId");
	}
	
	public void setOrigemString(String value) {
		set("origemString", value);
	}
	public String getOrigemString() {
		return get("origemString");
	}
	
	public void addDestino(Long destinoId, String destinoString, Integer destinoTempo, Double destinoCusto) {
		List<Long> listIdList = get("destinosIdList");
		if(!listIdList.contains(destinoId)) listIdList.add(destinoId);
		set("destinoId"+destinoId, destinoId);
		set("destinoString"+destinoId, destinoString);
		set("destinoTempo"+destinoId, destinoTempo);
		set("destinoCusto"+destinoId, destinoCusto);
	}
	
	public List<Long> getDestinoIdList() {
		List<Long> listIdList = get("destinosIdList");
		return listIdList;
	}
	
	public String getDestinoString(Long id) {
		return get("destinoString"+id);
	}
	
	public Integer getDestinoTempo(Long id) {
		Number tempo = get("destinoTempo"+id);
		if(tempo == null) return 0;
		return tempo.intValue();
	}
	
	public Double getDestinoCusto(Long id) {
		Number custo = get("destinoCusto"+id);
		if(custo == null) return 0.0;
		return custo.doubleValue();
	}
	
}
