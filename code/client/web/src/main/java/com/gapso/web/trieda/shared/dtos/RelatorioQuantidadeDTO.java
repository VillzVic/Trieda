package com.gapso.web.trieda.shared.dtos;

public class RelatorioQuantidadeDTO  extends AbstractDTO< String >
	implements Comparable< RelatorioQuantidadeDTO >
{
	
	private static final long serialVersionUID = 8643567081805503959L;

	public static final String PROPERTY_CAMPUS_NOME = "campusNome";
	public static final String PROPERTY_FAIXA_CREDITO = "faixaCredito";
	public static final String PROPERTY_QUANTIDADE = "quantidade";
	public static final String PROPERTY_QUANTIDADE2 = "quantidade2";
	public static final String PROPERTY_QUANTIDADE3 = "quantidade3";
	
	public RelatorioQuantidadeDTO()
	{
		super();
	}
	
	public RelatorioQuantidadeDTO(String text)
	{
		super();
		setFaixaCredito(text);
		setQuantidade(0);
		setQuantidade2(0);
		setQuantidade3(0);
	}
	
	public void setCampusNome(String value) {
		set(PROPERTY_CAMPUS_NOME, value);
	}
	
	public String getCampusNome() {
		return get(PROPERTY_CAMPUS_NOME);
	}
	
	public void setFaixaCredito(String value) {
		set(PROPERTY_FAIXA_CREDITO, value);
	}
	
	public String getFaixaCredito() {
		return get(PROPERTY_FAIXA_CREDITO);
	}
		
	public void setQuantidade(Integer value) {
		set(PROPERTY_QUANTIDADE, value);
	}
	
	public Integer getQuantidade() {
		return get(PROPERTY_QUANTIDADE);
	}
	
	public void setQuantidade2(Integer value) {
		set(PROPERTY_QUANTIDADE2, value);
	}
	
	public Integer getQuantidade2() {
		return get(PROPERTY_QUANTIDADE2);
	}
	
	public void setQuantidade3(Integer value) {
		set(PROPERTY_QUANTIDADE3, value);
	}
	
	public Integer getQuantidade3() {
		return get(PROPERTY_QUANTIDADE3);
	}
	
	@Override
	public int compareTo(RelatorioQuantidadeDTO o) {
		return getFaixaCredito().compareTo(o.getFaixaCredito());
	}
	
	@Override
	public String getNaturalKey() {
		return getFaixaCredito();
	}
}
