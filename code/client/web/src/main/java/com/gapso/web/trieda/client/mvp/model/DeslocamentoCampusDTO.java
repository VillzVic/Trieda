package com.gapso.web.trieda.client.mvp.model;

import java.util.ArrayList;
import java.util.List;

import com.gapso.web.trieda.shared.dtos.AbstractDTO;

public class DeslocamentoCampusDTO extends AbstractDTO<String> implements Comparable<DeslocamentoCampusDTO> {

	private static final long serialVersionUID = -5134820110949139907L;
	
	// Propriedades
	public static final String PROPERTY_ORIGEM_ID = "origemId";
	public static final String PROPERTY_ORIGEM_STRING = "origemString";
	public static final String PROPERTY_DESTINO_IDS_LIST = "destinosIdList";
	public static final String PROPERTY_DESTINO_ID = "destinoId";
	public static final String PROPERTY_DESTINO_STRING = "destinoString";
	public static final String PROPERTY_DESTINO_TEMPO = "destinoTempo";
	public static final String PROPERTY_DESTINO_CUSTO = "destinoCusto";

	public DeslocamentoCampusDTO() {
		super();
		set(PROPERTY_DESTINO_IDS_LIST, new ArrayList<Long>());
	}

	public void setOrigemId(Long value) {
		set(PROPERTY_ORIGEM_ID, value);
	}
	public Long getOrigemId() {
		return get(PROPERTY_ORIGEM_ID);
	}
	
	public void setOrigemString(String value) {
		set(PROPERTY_ORIGEM_STRING, value);
	}
	public String getOrigemString() {
		return get(PROPERTY_ORIGEM_STRING);
	}
	
	// Adicionando um destino, não existe um set/get de cada destino pq a quantidade é dinamica.
	public void addDestino(Long destinoId, String destinoString, Integer destinoTempo, Double destinoCusto) {
		List<Long> listIdList = get(PROPERTY_DESTINO_IDS_LIST);
		if(!listIdList.contains(destinoId)) listIdList.add(destinoId);
		set(PROPERTY_DESTINO_ID+destinoId, destinoId);
		set(PROPERTY_DESTINO_STRING+destinoId, destinoString);
		set(PROPERTY_DESTINO_TEMPO+destinoId, destinoTempo);
		set(PROPERTY_DESTINO_CUSTO+destinoId, destinoCusto);
	}
	
	public List<Long> getDestinoIdList() {
		List<Long> listIdList = get(PROPERTY_DESTINO_IDS_LIST);
		return listIdList;
	}
	
	public String getDestinoString(Long id) {
		return get(PROPERTY_DESTINO_STRING+id);
	}
	
	public Integer getDestinoTempo(Long id) {
		Number tempo = get(PROPERTY_DESTINO_TEMPO+id);
		if(tempo == null) return 0;
		return tempo.intValue();
	}
	
	public Double getDestinoCusto(Long id) {
		Number custo = get(PROPERTY_DESTINO_CUSTO+id);
		if(custo == null) return 0.0;
		return custo.doubleValue();
	}

	@Override
	public String getNaturalKey() {
		return getOrigemString();
	}
	
	@Override
	public int compareTo(DeslocamentoCampusDTO o) {
		return getOrigemString().compareTo(o.getOrigemString());
	}	
}