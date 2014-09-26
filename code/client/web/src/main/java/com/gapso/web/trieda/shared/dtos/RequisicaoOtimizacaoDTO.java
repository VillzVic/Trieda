package com.gapso.web.trieda.shared.dtos;

import java.io.Serializable;
import java.util.Set;

public class RequisicaoOtimizacaoDTO extends AbstractDTO<String> implements Comparable<RequisicaoOtimizacaoDTO> {

	private static final long serialVersionUID = 1L;
	
	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_USER_NAME = "userName";
	public static final String PROPERTY_USER_NAME_CANCEL = "userNameCanacel";
	public static final String PROPERTY_CENARIO_ID = "cenarioId";
	public static final String PROPERTY_CENARIO_NOME = "cenarioNome";
	public static final String PROPERTY_POSICAO_FILA = "posicaoFila";
	public static final String PROPERTY_ROUND = "round";
	public static final String PROPERTY_PARAMETRO_ID = "parametroId";
	public static final String PROPERTY_STATUS_INDEX = "statusIndex";
	public static final String PROPERTY_MODO_OTIMIZACAO = "modoOtimizacao";
	public static final String PROPERTY_OTIMIZAR_POR = "otimizarPor";
	public static final String PROPERTY_FUNCAO_OBJETIVO = "funcaoObjetivo";
	public static final String PROPERTY_CAMPI_SELECIONADOS = "campiSelecionados";
	public static final String PROPERTY_TURNOS_SELECIONADOS = "turnosSelecionados";
	public static final String PROPERTY_TOTAL_CAMPI = "totalCampi";
	public static final String PROPERTY_TOTAL_TURNOS = "totalTurnos";
	public static final String PROPERTY_TOTAL_ALUNOS = "totalAlunos";
	public static final String PROPERTY_TOTAL_ALUNODEMANDA_P1 = "totalAlunosDemandasP1";
	public static final String PROPERTY_TOTAL_ALUNODEMANDA_P2 = "totalAlunosDemandasP2";
	public static final String PROPERTY_TOTAL_AMBIENTES = "totalAmbientes";
	public static final String PROPERTY_TOTAL_PROFESSORES = "totalProfessores";
	public static final String PROPERTY_INSTANTE_INICIO_REQUISICAO = "instanteInicioRequisicao";
	public static final String PROPERTY_INSTANTE_INICIO_OTIMIZACAO = "instanteInicioOtimizacao";
	public static final String PROPERTY_INSTANTE_TERMINO = "instanteTermino";
	public static final String PROPERTY_DURACAO_REQUISICAO = "duracaoRequisicao";
	public static final String PROPERTY_DURACAO_OTIMIZACAO = "duracaoOtimizacao";
	
	static public enum StatusRequisicaoOtimizacao implements Serializable {AGUARDANDO,EXECUTANDO,FINALIZADA_COM_RESULTADO,FINALIZADA_SEM_RESULTADO,CANCELADA,INVALIDO}
	
	transient private Set<Long> professoresRelacionadosIDs;
	public void setProfessoresRelacionadosIDs(Set<Long> professoresRelacionadosIDs) {
		this.professoresRelacionadosIDs = professoresRelacionadosIDs; 
	}
	public Set<Long> getProfessoresRelacionadosIDs() {
		return this.professoresRelacionadosIDs;
	}
	
	public void setId(Long value) {
		set(PROPERTY_ID,value);
	}
	
	public Long getId() {
		return get(PROPERTY_ID);
	}
	
	public void setUserName(String value) {
		set(PROPERTY_USER_NAME,value);
	}
	
	public String getUserName() {
		return get(PROPERTY_USER_NAME);
	}
	
	public void setUserNameCancel(String value) {
		set(PROPERTY_USER_NAME_CANCEL,value);
	}
	
	public String getUserNameCancel() {
		return get(PROPERTY_USER_NAME_CANCEL);
	}
	
	public void setCenarioId(Long value) {
		set(PROPERTY_CENARIO_ID,value);
	}
	
	public Long getCenarioId() {
		return get(PROPERTY_CENARIO_ID);
	}
	
	public void setCenarioNome(String value) {
		set(PROPERTY_CENARIO_NOME,value);
	}
	
	public Long getCenarioNome() {
		return get(PROPERTY_CENARIO_NOME);
	}
	
	public void setPosicaoFila(String value) {
		set(PROPERTY_POSICAO_FILA,value);
	}
	
	public Long getPosicaoFila() {
		return get(PROPERTY_POSICAO_FILA);
	}
	
	public void setRound(Long value) {
		set(PROPERTY_ROUND,value);
	}
	
	public Long getRound() {
		return get(PROPERTY_ROUND);
	}
	
	public void setParametroId(Long value) {
		set(PROPERTY_PARAMETRO_ID,value);
	}
	
	public Long getParametroId() {
		return get(PROPERTY_PARAMETRO_ID);
	}
	
	public void setStatusIndex(Integer value) {
		set(PROPERTY_STATUS_INDEX,value);
	}
	
	public Integer getStatusIndex() {
		return get(PROPERTY_STATUS_INDEX);
	}
	
	public void setModoOtimizacao(String value) {
		set(PROPERTY_MODO_OTIMIZACAO,value);
	}
	
	public String getModoOtimizacao() {
		return get(PROPERTY_MODO_OTIMIZACAO);
	}
	
	public void setOtimizarPor(String value) {
		set(PROPERTY_OTIMIZAR_POR,value);
	}
	
	public String getOtimizarPor() {
		return get(PROPERTY_OTIMIZAR_POR);
	}
	
	public void setFuncaoObjetivo(String value) {
		set(PROPERTY_FUNCAO_OBJETIVO,value);
	}
	
	public String getFuncaoObjetivo() {
		return get(PROPERTY_FUNCAO_OBJETIVO);
	}
	
	public void setCampiSelecionados(String value) {
		set(PROPERTY_CAMPI_SELECIONADOS,value);
	}
	
	public String getCampiSelecionados() {
		return get(PROPERTY_CAMPI_SELECIONADOS);
	}
	
	public void setTurnosSelecionados(String value) {
		set(PROPERTY_TURNOS_SELECIONADOS,value);
	}
	
	public String getTurnosSelecionados() {
		return get(PROPERTY_TURNOS_SELECIONADOS);
	}
	
	public void setTotalCampi(Integer value) {
		set(PROPERTY_TOTAL_CAMPI, value);
	}
	public Integer getTotalCampi() {
		return get(PROPERTY_TOTAL_CAMPI);
	}
	
	public void setTotalTurnos(Integer value) {
		set(PROPERTY_TOTAL_TURNOS, value);
	}
	public Integer getTotalTurnos() {
		return get(PROPERTY_TOTAL_TURNOS);
	}
	
	public void setTotalAlunos(Integer value) {
		set(PROPERTY_TOTAL_ALUNOS, value);
	}
	public Integer getTotalAlunos() {
		return get(PROPERTY_TOTAL_ALUNOS);
	}
	
	public void setTotalAlunosDemandasP1(Integer value) {
		set(PROPERTY_TOTAL_ALUNODEMANDA_P1, value);
	}
	public Integer getTotalAlunosDemandasP1() {
		return get(PROPERTY_TOTAL_ALUNODEMANDA_P1);
	}
	
	public void setTotalAlunosDemandasP2(Integer value) {
		set(PROPERTY_TOTAL_ALUNODEMANDA_P2, value);
	}
	public Integer getTotalAlunosDemandasP2() {
		return get(PROPERTY_TOTAL_ALUNODEMANDA_P2);
	}
	
	public void setTotalAmbientes(Integer value) {
		set(PROPERTY_TOTAL_AMBIENTES, value);
	}
	public Integer getTotalAmbientes() {
		return get(PROPERTY_TOTAL_AMBIENTES);
	}
	
	public void setTotalProfessores(Integer value) {
		set(PROPERTY_TOTAL_PROFESSORES, value);
	}
	public Integer getTotalProfessores() {
		return get(PROPERTY_TOTAL_PROFESSORES);
	}
	
	public String getInstanteInicioRequisicao() {
		return get(PROPERTY_INSTANTE_INICIO_REQUISICAO);
	}
	
	public void setInstanteInicioRequisicao(String value) {
		set(PROPERTY_INSTANTE_INICIO_REQUISICAO,value);
	}
	
	public String getInstanteInicioOtimizacao() {
		return get(PROPERTY_INSTANTE_INICIO_OTIMIZACAO);
	}
	
	public void setInstanteInicioOtimizacao(String value) {
		set(PROPERTY_INSTANTE_INICIO_OTIMIZACAO,value);
	}
	
	public String getInstanteTermino() {
		return get(PROPERTY_INSTANTE_TERMINO);
	}
	
	public void setInstanteTermino(String value) {
		set(PROPERTY_INSTANTE_TERMINO,value);
	}
	
	public String getDuracaoRequisicao() {
		return get(PROPERTY_DURACAO_REQUISICAO);
	}
	
	public void setDuracaoRequisicao(String value) {
		set(PROPERTY_DURACAO_REQUISICAO,value);
	}
	
	public String getDuracaoOtimizacao() {
		return get(PROPERTY_DURACAO_OTIMIZACAO);
	}
	
	public void setDuracaoOtimizacao(String value) {
		set(PROPERTY_DURACAO_OTIMIZACAO,value);
	}

	@Override
	public int compareTo(RequisicaoOtimizacaoDTO o) {
		return getId().compareTo(o.getId());
	}

	@Override
	public String getNaturalKey() {
		return getUserName() + "-" + getRound() + "-" + getParametroId() + "-" + getCenarioId();
	}

	@Override
	public String toString() {
		return "Username=" + getUserName() + " / Round=" + getRound() + " / MsgStatus=" + StatusRequisicaoOtimizacao.values()[getStatusIndex()];
	}
}