package com.gapso.web.trieda.shared.dtos;

import java.io.Serializable;

public class RequisicaoOtimizacaoDTO extends AbstractDTO<String> implements Comparable<RequisicaoOtimizacaoDTO> {

	private static final long serialVersionUID = 1L;
	
	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_USER_NAME = "userName";
	public static final String PROPERTY_CENARIO_ID = "cenarioId";
	public static final String PROPERTY_ROUND = "round";
	public static final String PROPERTY_PARAMETRO_ID = "parametroId";
	public static final String PROPERTY_STATUS_INDEX = "statusIndex";
	public static final String PROPERTY_MODO_OTIMIZACAO = "modoOtimizacao";
	public static final String PROPERTY_OTIMIZAR_POR = "otimizarPor";
	public static final String PROPERTY_FUNCAO_OBJETIVO = "funcaoObjetivo";
	public static final String PROPERTY_CAMPI_SELECIONADOS = "campiSelecionados";
	public static final String PROPERTY_TURNO = "turno";
	
	static public enum StatusRequisicaoOtimizacao implements Serializable {EM_ANDAMENTO,FINALIZADA_COM_OUTPUT,FINALIZADA_SEM_OUTPUT}
	
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
	
	public void setCenarioId(Long value) {
		set(PROPERTY_CENARIO_ID,value);
	}
	
	public Long getCenarioId() {
		return get(PROPERTY_CENARIO_ID);
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
	
	public void setTurno(String value) {
		set(PROPERTY_TURNO,value);
	}
	
	public String getTurno() {
		return get(PROPERTY_TURNO);
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