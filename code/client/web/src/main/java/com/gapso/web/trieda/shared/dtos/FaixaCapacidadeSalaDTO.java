package com.gapso.web.trieda.shared.dtos;

public class FaixaCapacidadeSalaDTO extends AbstractDTO<String> implements Comparable<FaixaCapacidadeSalaDTO> {
	
	private static final long serialVersionUID = -3078709599908727181L;
	public static final String PROPERTY_NOME = "nome";
	public static final String PROPERTY_FAIXA_SUPERIOR = "faixaSuperior";
	public static final String PROPERTY_FAIXA_INFERIOR = "faixaInferior";

	public FaixaCapacidadeSalaDTO() {
		super();
	}
	
	public FaixaCapacidadeSalaDTO(Integer faixaInf, Integer faixaSup) {
		super();
		setFaixaInferior(faixaInf);
		setFaixaSuperior(faixaSup);
		setNome("De " + faixaInf + " até " + faixaSup + " alunos");
	}
	
	public String getNome() {
		return get(PROPERTY_NOME);
	}
	public void setNome(String value) {
		set(PROPERTY_NOME, value);
	}
	
	public Integer getFaixaSuperior() {
		return get(PROPERTY_FAIXA_SUPERIOR);
	}
	public void setFaixaSuperior(Integer value) {
		set(PROPERTY_FAIXA_SUPERIOR, value);
	}
	
	public Integer getFaixaInferior() {
		return get(PROPERTY_FAIXA_INFERIOR);
	}
	public void setFaixaInferior(Integer value) {
		set(PROPERTY_FAIXA_INFERIOR, value);
	}
	
	@Override
	public String getNaturalKey() {
		return getNome();
	}

	@Override
	public int compareTo(FaixaCapacidadeSalaDTO o) {
		return getNome().compareTo(o.getNome());
	}
}
