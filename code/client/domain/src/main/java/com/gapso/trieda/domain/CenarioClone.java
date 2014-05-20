package com.gapso.trieda.domain;

import java.util.HashMap;
import java.util.Map;

@SuppressWarnings("rawtypes")
public class CenarioClone {

	private Cenario cenarioClone;
	
	public Map<Class<? extends Clonable>, Map<Object, Object>> entidadeMapIdNovoMapIdAntigo;
	
	public CenarioClone(Cenario cenario)
	{
		cenarioClone = cenario;
		
		entidadeMapIdNovoMapIdAntigo = new HashMap<Class<? extends Clonable>, Map<Object, Object>>();
	}
	
	public <A extends Clonable<A>> A clone(A entidadeASerClonada)
	{
		A entidadeClone = entidadeASerClonada.clone(this);
		
		mapIds(entidadeASerClonada, entidadeClone, entidadeASerClonada.getClass());
		entidadeASerClonada.cloneChilds(this, entidadeClone);

		return entidadeClone;
	}
	
	private <A extends Clonable<A>> void mapIds(A entidadeOriginal, A entidadeASerClonada, Class<? extends Clonable> tipoEntidade)
	{
		if (tipoEntidade.equals(HorarioDisponivelCenario.class))
		{
			//System.out.println("Adicionando Horario Aula: " + ((HorarioAula)entidadeOriginal).getId());
		}
		if (entidadeMapIdNovoMapIdAntigo.get(tipoEntidade) == null)
		{
			Map<Object, Object> IdNovoMapIdAntigo = new HashMap<Object, Object>();
			IdNovoMapIdAntigo.put(entidadeOriginal, entidadeASerClonada);
			entidadeMapIdNovoMapIdAntigo.put(tipoEntidade, IdNovoMapIdAntigo);
		}
		else
		{
			entidadeMapIdNovoMapIdAntigo.get(tipoEntidade).put(entidadeOriginal, entidadeASerClonada);
		}
	}
	
	@SuppressWarnings("unchecked")
	public <A extends Clonable<A>> A getEntidadeClonada(A entidadeOriginal)
	{
		return (A) entidadeMapIdNovoMapIdAntigo.get(entidadeOriginal.getClass()).get(entidadeOriginal);
	}
	
	public Cenario getCenario()
	{
		return cenarioClone;
	}
}
