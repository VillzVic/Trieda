package com.gapso.web.trieda.shared.dtos;

public class AtendimentoFaixaDemandaDTO extends AbstractDTO< String >
	implements Comparable< AtendimentoFaixaDemandaDTO >
{

	private static final long serialVersionUID = 2913513728511317302L;
	
	public static final String PROPERTY_CAMPUS_NOME = "campusNome";
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
	
	public static final String PROPERTY_RECEITA_SEMANAL = "receitaSemanal";
	public static final String PROPERTY_CUSTO_DOCENTE_SEMANAL = "custoDocenteSemanal";
	public static final String PROPERTY_CUSTO_DOCENTE_POR_RECEITA_PERCENT = "custoDocentePorReceitaPercent";
	public static final String PROPERTY_RECEITA_ACUMULADA = "receitaAcumulada";
	public static final String PROPERTY_CUSTO_DOCENTE_ACUMULADO = "custoDocenteAcumulado";
	public static final String PROPERTY_CUSTO_DOCENTE_POR_RECEITA_ACUMULADO_PERCENT = "custoDocentePorReceitaAcumuladoPercent";
	
	public AtendimentoFaixaDemandaDTO()
	{
		super();
	}
	
	public void setDemandaDisc(String value) {
		set(PROPERTY_DEMANDA_DISCIPLINA, value);
	}

	public String getDemandaDisc() {
		return get(PROPERTY_DEMANDA_DISCIPLINA);
	}
	
	public void setDemandaP1(Integer value) {
		set(PROPERTY_DEMANDA_P1, value);
	}

	public Integer getDemandaP1() {
		return get(PROPERTY_DEMANDA_P1);
	}
	
	public void setAtendimentoP1(Integer value) {
		set(PROPERTY_ATENDIMENTO_P1, value);
	}

	public Integer getAtendimentoP1() {
		return get(PROPERTY_ATENDIMENTO_P1);
	}
	
	public void setAtendimentoPercentP1(String value) {
		set(PROPERTY_ATENDIMENTO_PERCENT_P1, value);
	}

	public String getAtendimentoPercentP1() {
		return get(PROPERTY_ATENDIMENTO_PERCENT_P1);
	}

	public void setAtendimentoSoma(Integer value) {
		set(PROPERTY_ATENDIMENTO_SOMA, value);
	}

	public Integer getAtendimentoSoma() {
		return get(PROPERTY_ATENDIMENTO_SOMA);
	}
	
	public void setAtendimentoSomaPercent(String value) {
		set(PROPERTY_ATENDIMENTO_SOMA_PERCENT, value);
	}

	public String getAtendimentoSomaPercent() {
		return get(PROPERTY_ATENDIMENTO_SOMA_PERCENT);
	}
	
	public void setTurmasAbertas(Integer value) {
		set(PROPERTY_TURMAS_ABERTAS, value);
	}

	public Integer getTurmasAbertas() {
		return get(PROPERTY_TURMAS_ABERTAS);
	}
	
	public void setMediaTurma(Integer value) {
		set(PROPERTY_MEDIA_TURMA, value);
	}

	public Integer getMediaTurma() {
		return get(PROPERTY_MEDIA_TURMA);
	}
	
	public void setDemandaAcumP1(Integer value) {
		set(PROPERTY_DEMANDA_ACUM_P1, value);
	}

	public Integer getDemandaAcumP1() {
		return get(PROPERTY_DEMANDA_ACUM_P1);
	}
	
	public void setCreditosPagos(Integer value) {
		set(PROPERTY_CREDITOS_PAGOS, value);
	}

	public Integer getCreditosPagos() {
		return get(PROPERTY_CREDITOS_PAGOS);
	}
	
	public void setAtendimentoSomaAcum(Integer value) {
		set(PROPERTY_ATENDIMENTO_SOMA_ACUM, value);
	}

	public Integer getAtendimentoSomaAcum() {
		return get(PROPERTY_ATENDIMENTO_SOMA_ACUM);
	}
	
	public void setAtendimentoSomaAcumPercent(String value) {
		set(PROPERTY_ATENDIMENTO_ACUM_PERCENT, value);
	}

	public String getAtendimentoSomaAcumPercent() {
		return get(PROPERTY_ATENDIMENTO_ACUM_PERCENT);
	}
	
	public void setReceitaSemanal(Integer value) {
		set(PROPERTY_RECEITA_SEMANAL, value);
	}

	public Integer getReceitaSemanal() {
		return get(PROPERTY_RECEITA_SEMANAL);
	}
	
	public void setCustoDocenteSemanal(Integer value) {
		set(PROPERTY_CUSTO_DOCENTE_SEMANAL, value);
	}

	public Integer getCustoDocenteSemanal() {
		return get(PROPERTY_CUSTO_DOCENTE_SEMANAL);
	}
	
	public void setCustoDocentePorReceitaPercent(String value) {
		set(PROPERTY_CUSTO_DOCENTE_POR_RECEITA_PERCENT, value);
	}

	public String getCustoDocentePorReceitaPercent() {
		return get(PROPERTY_CUSTO_DOCENTE_POR_RECEITA_PERCENT);
	}
	
	public void setReceitaAcumulada(Integer value) {
		set(PROPERTY_RECEITA_ACUMULADA, value);
	}

	public Integer getReceitaAcumulada() {
		return get(PROPERTY_RECEITA_ACUMULADA);
	}
	
	public void setCustoDocenteAcumulado(Integer value) {
		set(PROPERTY_CUSTO_DOCENTE_ACUMULADO, value);
	}

	public Integer getCustoDocenteAcumulado() {
		return get(PROPERTY_CUSTO_DOCENTE_ACUMULADO);
	}
	
	public void setCustoDocentePorReceitaAcumuladoPercent(String value) {
		set(PROPERTY_CUSTO_DOCENTE_POR_RECEITA_ACUMULADO_PERCENT, value);
	}

	public String getCustoDocentePorReceitaAcumuladoPercent() {
		return get(PROPERTY_CUSTO_DOCENTE_POR_RECEITA_ACUMULADO_PERCENT);
	}
	
	public void setCampusNome(String value) {
		set(PROPERTY_CAMPUS_NOME, value);
	}

	public String getCampusNome() {
		return get(PROPERTY_CAMPUS_NOME);
	}
	
	@Override
	public int compareTo(AtendimentoFaixaDemandaDTO o) {
		return getDemandaDisc().compareTo(o.getDemandaDisc());
	}

	@Override
	public String getNaturalKey() {
		return getDemandaDisc();
	}

}
