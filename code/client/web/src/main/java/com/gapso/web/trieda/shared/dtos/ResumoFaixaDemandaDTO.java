package com.gapso.web.trieda.shared.dtos;

public class ResumoFaixaDemandaDTO extends AbstractDTO< String >
	implements Comparable< ResumoFaixaDemandaDTO >
{

	private static final long serialVersionUID = 2913513728511317302L;
	
	public static final String PROPERTY_DEMANDA_DISCIPLINA = "demandaDisc";
	public static final String PROPERTY_DEMANDA_P1 = "demandaP1";
	public static final String PROPERTY_ATENDIMENTO_P1 = "atendimentoP1";
	public static final String PROPERTY_ATENDIMENTO_PERCENT_P1 = "atendimentoPercentP1";
	public static final String PROPERTY_ATENDIMENTO_SOMA = "atendimentoSoma";
	public static final String PROPERTY_ATENDIMENTO_SOMA_PERCENT = "atendimentoSomaPercent";
	public static final String PROPERTY_TURMAS_ABERTAS = "turmasAbertas";
	public static final String PROPERTY_MEDIA_TURMA = "mediaTurma";
	public static final String PROPERTY_DEMANDA_ACUM_P1 = "demandaAcumP1";
	public static final String PROPERTY_CREDITOS_PAGOS = "creditosPagos";
	public static final String PROPERTY_ATENDIMENTO_SOMA_ACUM = "atendimentoSomaAcum";
	public static final String PROPERTY_ATENDIMENTO_ACUM_PERCENT = "atendimentoAcumPercent";
	
	public ResumoFaixaDemandaDTO()
	{
		super();
	}
	
	public void setDemandaDisc(String value) {
		set(PROPERTY_DEMANDA_DISCIPLINA, value);
	}

	public String getDemandaDisc() {
		return get(PROPERTY_DEMANDA_DISCIPLINA);
	}
	
	public void setDemandaP1(int value) {
		set(PROPERTY_DEMANDA_P1, value);
	}

	public int getDemandaP1() {
		return get(PROPERTY_DEMANDA_P1);
	}
	
	public void setAtendimentoP1(int value) {
		set(PROPERTY_ATENDIMENTO_P1, value);
	}

	public int getAtendimentoP1() {
		return get(PROPERTY_ATENDIMENTO_P1);
	}
	
	public void setAtendimentoPercentP1(String value) {
		set(PROPERTY_ATENDIMENTO_PERCENT_P1, value);
	}

	public String getAtendimentoPercentP1() {
		return get(PROPERTY_ATENDIMENTO_PERCENT_P1);
	}

	public void setAtendimentoSoma(int value) {
		set(PROPERTY_ATENDIMENTO_SOMA, value);
	}

	public int getAtendimentoSoma() {
		return get(PROPERTY_ATENDIMENTO_SOMA);
	}
	
	public void setAtendimentoSomaPercent(String value) {
		set(PROPERTY_ATENDIMENTO_SOMA_PERCENT, value);
	}

	public String getAtendimentoSomaPercent() {
		return get(PROPERTY_ATENDIMENTO_SOMA_PERCENT);
	}
	
	public void setTurmasAbertas(int value) {
		set(PROPERTY_TURMAS_ABERTAS, value);
	}

	public int getTurmasAbertas() {
		return get(PROPERTY_TURMAS_ABERTAS);
	}
	
	public void setMediaTurma(Double value) {
		set(PROPERTY_MEDIA_TURMA, value);
	}

	public Double getMediaTurma() {
		return get(PROPERTY_MEDIA_TURMA);
	}
	
	public void setDemandaAcumP1(int value) {
		set(PROPERTY_DEMANDA_ACUM_P1, value);
	}

	public int getDemandaAcumP1() {
		return get(PROPERTY_DEMANDA_ACUM_P1);
	}
	
	public void setCreditosPagos(int value) {
		set(PROPERTY_CREDITOS_PAGOS, value);
	}

	public int getCreditosPagos() {
		return get(PROPERTY_CREDITOS_PAGOS);
	}
	
	public void setAtendimentoSomaAcum(int value) {
		set(PROPERTY_ATENDIMENTO_SOMA_ACUM, value);
	}

	public int getAtendimentoSomaAcum() {
		return get(PROPERTY_ATENDIMENTO_SOMA_ACUM);
	}
	
	public void setAtendimentoSomaAcumPercent(String value) {
		set(PROPERTY_ATENDIMENTO_ACUM_PERCENT, value);
	}

	public String getAtendimentoSomaAcumPercent() {
		return get(PROPERTY_ATENDIMENTO_ACUM_PERCENT);
	}
	
	@Override
	public int compareTo(ResumoFaixaDemandaDTO o) {
		return getDemandaDisc().compareTo(o.getDemandaDisc());
	}

	@Override
	public String getNaturalKey() {
		return getDemandaDisc();
	}

}
