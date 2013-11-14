package com.gapso.web.trieda.shared.dtos;

public class AtendimentoFaixaTurmaDTO extends AbstractDTO< String >
implements Comparable< AtendimentoFaixaTurmaDTO >
{
	
	private static final long serialVersionUID = 8944169462057272071L;

	public static final String PROPERTY_CAMPUS_NOME = "campusNome";
	public static final String PROPERTY_TAMANHO_TURMA = "tamanhoTurma";
	public static final String PROPERTY_ATENDIMENTO_CRED = "atendimentoCred";
	public static final String PROPERTY_ATENDIMENTO_CRED_ACUM = "atendimentoCredAcum";
	public static final String PROPERTY_ATENDIMENTO_ALUNO = "atendimentoAluno";
	public static final String PROPERTY_ATENDIMENTO_ALUNO_ACUM = "atendimentoAlunoAcum";
	public static final String PROPERTY_TURMAS_ABERTAS = "turmasAbertas";
	public static final String PROPERTY_MEDIA_TURMA = "mediaTurma";
	public static final String PROPERTY_CREDITOS_PAGOS = "creditosPagos";
	public static final String PROPERTY_RECEITA_SEMANAL = "receitaSemanal";
	public static final String PROPERTY_CUSTO_DOCENTE_SEMANAL = "custoDocenteSemanal";
	public static final String PROPERTY_CUSTO_DOCENTE_POR_RECEITA_PERCENT = "custoDocentePorReceitaPercent";
	public static final String PROPERTY_RECEITA_ACUMULADA = "receitaAcumulada";
	public static final String PROPERTY_CUSTO_DOCENTE_ACUMULADO = "custoDocenteAcumulado";
	public static final String PROPERTY_CUSTO_DOCENTE_POR_RECEITA_ACUMULADO_PERCENT = "custoDocentePorReceitaAcumuladoPercent";
	
	public AtendimentoFaixaTurmaDTO()
	{
		super();
	}
	
	public AtendimentoFaixaTurmaDTO(String tamanhoTurma)
	{
		super();
		setTamanhoTurma(tamanhoTurma);
	}
	
	public void setCampusNome(String value) {
		set(PROPERTY_CAMPUS_NOME, value);
	}

	public String getCampusNome() {
		return get(PROPERTY_CAMPUS_NOME);
	}
	
	public void setTamanhoTurma(String value) {
		set(PROPERTY_TAMANHO_TURMA, value);
	}

	public String getTamanhoTurma() {
		return get(PROPERTY_TAMANHO_TURMA);
	}
	
	public void setAtendimentoCred(Integer value) {
		set(PROPERTY_ATENDIMENTO_CRED, value);
	}

	public Integer getAtendimentoCred() {
		return get(PROPERTY_ATENDIMENTO_CRED);
	}
	
	public void setAtendimentoCredAcum(Integer value) {
		set(PROPERTY_ATENDIMENTO_CRED_ACUM, value);
	}
	
	public Integer getAtendimentoCredAcum() {
		return get(PROPERTY_ATENDIMENTO_CRED_ACUM);
	}

	public void setAtendimentoAluno(Integer value) {
		set(PROPERTY_ATENDIMENTO_ALUNO, value);
	}

	public Integer getAtendimentoAluno() {
		return get(PROPERTY_ATENDIMENTO_ALUNO);
	}
	
	public void setAtendimentoAlunoAcum(Integer value) {
		set(PROPERTY_ATENDIMENTO_ALUNO_ACUM, value);
	}

	public Integer getAtendimentoAlunoAcum() {
		return get(PROPERTY_ATENDIMENTO_ALUNO_ACUM);
	}
	
	public void setCreditosPagos(Double value) {
		set(PROPERTY_CREDITOS_PAGOS, value);
	}

	public Double getCreditosPagos() {
		return get(PROPERTY_CREDITOS_PAGOS);
	}	
	
	public void setReceitaSemanal(Double value) {
		set(PROPERTY_RECEITA_SEMANAL, value);
	}

	public Double getReceitaSemanal() {
		return get(PROPERTY_RECEITA_SEMANAL);
	}
	
	public void setCustoDocenteSemanal(Double value) {
		set(PROPERTY_CUSTO_DOCENTE_SEMANAL, value);
	}

	public Double getCustoDocenteSemanal() {
		return get(PROPERTY_CUSTO_DOCENTE_SEMANAL);
	}
	
	public void setCustoDocentePorReceitaPercent(String value) {
		set(PROPERTY_CUSTO_DOCENTE_POR_RECEITA_PERCENT, value);
	}

	public String getCustoDocentePorReceitaPercent() {
		return get(PROPERTY_CUSTO_DOCENTE_POR_RECEITA_PERCENT);
	}
	
	public void setReceitaAcumulada(Double value) {
		set(PROPERTY_RECEITA_ACUMULADA, value);
	}

	public Double getReceitaAcumulada() {
		return get(PROPERTY_RECEITA_ACUMULADA);
	}
	
	public void setCustoDocenteAcumulado(Double value) {
		set(PROPERTY_CUSTO_DOCENTE_ACUMULADO, value);
	}

	public Double getCustoDocenteAcumulado() {
		return get(PROPERTY_CUSTO_DOCENTE_ACUMULADO);
	}
	
	public void setCustoDocentePorReceitaAcumuladoPercent(String value) {
		set(PROPERTY_CUSTO_DOCENTE_POR_RECEITA_ACUMULADO_PERCENT, value);
	}

	@Override
	public int compareTo(AtendimentoFaixaTurmaDTO o) {
		return getTamanhoTurma().compareTo(o.getTamanhoTurma());
	}

	@Override
	public String getNaturalKey() {
		return getTamanhoTurma();
	}
	
}
