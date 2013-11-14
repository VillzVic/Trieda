package com.gapso.web.trieda.shared.dtos;

public class RelatorioQuantidadeDTO  extends AbstractDTO< String >
	implements Comparable< RelatorioQuantidadeDTO >
{
	
	private static final long serialVersionUID = 8643567081805503959L;

	public static final String PROPERTY_CAMPUS_NOME = "campusNome";
	public static final String PROPERTY_FAIXA_CREDITO = "faixaCredito";
	public static final String PROPERTY_QUANTIDADE = "quantidade";
	
	public RelatorioQuantidadeDTO()
	{
		super();
	}
	
	public RelatorioQuantidadeDTO(String text)
	{
		super();
		setFaixaCredito(text);
		setQuantidade(0);
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
	
	@Override
	public int compareTo(RelatorioQuantidadeDTO o) {
		return getFaixaCredito().compareTo(o.getFaixaCredito());
	}
	
	@Override
	public String getNaturalKey() {
		return getFaixaCredito();
	}
}
