package com.gapso.web.trieda.shared.util.relatorioVisao;

import java.util.HashMap;
import java.util.Map;
import java.util.Set;

public class RelatorioVisaoMap<T, U, V> {
	
	private Map<T, Map<U, Map<V, AtendimentoServiceRelatorioResponse>>> map;
	
	public RelatorioVisaoMap(){
		map = new HashMap<T, Map<U, Map<V, AtendimentoServiceRelatorioResponse>>>();
	}
	
	public Map<U, Map<V, AtendimentoServiceRelatorioResponse>> get(T t){
		return map.get(t);
	}
	
	public void put(T t, Map<U, Map<V, AtendimentoServiceRelatorioResponse>> value){
		map.put(t, value);
	}
	
	public Set<T> keySet(){
		return map.keySet();
	}

}
