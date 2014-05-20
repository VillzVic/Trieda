package com.gapso.trieda.domain;

public interface Clonable<T> {
	public Long getId();
	
	public T clone(CenarioClone novoCenario);
	
	public void cloneChilds(CenarioClone novoCenario, T entidadeClone);
}
