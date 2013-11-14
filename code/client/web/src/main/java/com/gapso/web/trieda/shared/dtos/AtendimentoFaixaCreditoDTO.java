package com.gapso.web.trieda.shared.dtos;

public class AtendimentoFaixaCreditoDTO extends AbstractDTO< String >
	implements Comparable< AtendimentoFaixaCreditoDTO >
{
	
	private static final long serialVersionUID = -560261474640869779L;
	
	public static final String PROPERTY_CAMPUS_NOME = "campusNome";
	public static final String PROPERTY_FAIXA_CREDITO = "faixaCredito";
	public static final String PROPERTY_CREDITOS_DEMANDADOS_P1 = "creditosDemandadosP1";
	public static final String PROPERTY_CREDITOS_ATENDIDOS_P1 = "creditosAtendidosP1";
	public static final String PROPERTY_QUANTIDADE_ALUNOS = "quantidadeAlunos";
	public static final String PROPERTY_CREDITOS_DEMANDADOS_P2 = "creditosDemandadosP2";

	public AtendimentoFaixaCreditoDTO()
	{
		super();
	}
	
	public AtendimentoFaixaCreditoDTO(String text)
	{
		super();
		setFaixaCredito(text);
		setCreditosDemandadosP1(0);
		setCreditosAtendidosP1(0);
		setQuantidadeAlunos(0);
		setCreditosDemandadosP2(0);
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
	
	public void setCreditosDemandadosP1(Integer value) {
		set(PROPERTY_CREDITOS_DEMANDADOS_P1, value);
	}

	public Integer getCreditosDemandadosP1() {
		return get(PROPERTY_CREDITOS_DEMANDADOS_P1);
	}
	
	public void setCreditosAtendidosP1(Integer value) {
		set(PROPERTY_CREDITOS_ATENDIDOS_P1, value);
	}

	public Integer getCreditosAtendidosP1() {
		return get(PROPERTY_CREDITOS_ATENDIDOS_P1);
	}
	
	public void setQuantidadeAlunos(Integer value) {
		set(PROPERTY_QUANTIDADE_ALUNOS, value);
	}

	public Integer getQuantidadeAlunos() {
		return get(PROPERTY_QUANTIDADE_ALUNOS);
	}
	
	public void setCreditosDemandadosP2(Integer value) {
		set(PROPERTY_CREDITOS_DEMANDADOS_P2, value);
	}

	public Integer getCreditosDemandadosP2() {
		return get(PROPERTY_CREDITOS_DEMANDADOS_P2);
	}
	
	@Override
	public int compareTo(AtendimentoFaixaCreditoDTO o) {
		return getFaixaCredito().compareTo(o.getFaixaCredito());
	}

	@Override
	public String getNaturalKey() {
		return getFaixaCredito();
	}
}
