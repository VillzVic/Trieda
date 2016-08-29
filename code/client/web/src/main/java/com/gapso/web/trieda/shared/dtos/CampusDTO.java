package com.gapso.web.trieda.shared.dtos;

import com.gapso.web.trieda.shared.util.TriedaCurrency;
import com.gapso.web.trieda.shared.util.TriedaUtil;

public class CampusDTO
	extends AbstractDTO< String >
	implements Comparable< CampusDTO >
{
	private static final long serialVersionUID = -5134820110949139907L;

	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_CENARIO_ID = "cenarioId";
	public static final String PROPERTY_CODIGO = "codigo";
	public static final String PROPERTY_NOME = "nome";
	public static final String PROPERTY_ESTADO = "estado";
	public static final String PROPERTY_MUNICIPIO = "municipio";
	public static final String PROPERTY_BAIRRO = "bairro";
	public static final String PROPERTY_VALOR_CREDITO = "valorCredito";
	public static final String PROPERTY_OTIMIZADO_TATICO = "otimizadoTatico";
	public static final String PROPERTY_OTIMIZADO_OPERACIONAL = "otimizadoOperacional";
	public static final String PROPERTY_PUBLICADO = "publicado";
	public static final String PROPERTY_QTD_PROFESSOR_VIRTUAL = "qtdProfessorVirtual";

	public CampusDTO()
	{
		super();
	}

	public void setId(Long value) {
		set(PROPERTY_ID, value);
	}
	public Long getId() {
		return get(PROPERTY_ID);
	}
	
	public void setVersion(Integer value) {
		set(PROPERTY_VERSION, value);
	}
	public Integer getVersion() {
		return get(PROPERTY_VERSION);
	}

	public void setCenarioId(Long value) {
		set(PROPERTY_CENARIO_ID, value);
	}
	public Long getCenarioId() {
		return get(PROPERTY_CENARIO_ID);
	}
	
	public String getCodigo() {
		return get(PROPERTY_CODIGO);
	}
	public void setCodigo(String value) {
		set(PROPERTY_CODIGO, value);
	}

	public String getNome() {
		return get(PROPERTY_NOME);
	}
	public void setNome(String value) {
		set(PROPERTY_NOME, value);
	}
	
	public String getEstado() {
		return get(PROPERTY_ESTADO);
	}
	public void setEstado(String value) {
		set(PROPERTY_ESTADO, value);
	}

	public String getMunicipio() {
		return get(PROPERTY_MUNICIPIO);
	}
	public void setMunicipio(String value) {
		set(PROPERTY_MUNICIPIO, value);
	}

	public String getBairro() {
		return get(PROPERTY_BAIRRO);
	}
	public void setBairro(String value) {
		set(PROPERTY_BAIRRO, value);
	}

	public void setValorCredito(TriedaCurrency value) {
		set(PROPERTY_VALOR_CREDITO, value.toString());
	}

	public TriedaCurrency getValorCredito() {
		return TriedaUtil.parseTriedaCurrency(get(PROPERTY_VALOR_CREDITO));
	}

	public void setOtimizadoTatico(Boolean value) {
		set(PROPERTY_OTIMIZADO_TATICO, value);
	}
	public Boolean getOtimizadoTatico() {
		return get(PROPERTY_OTIMIZADO_TATICO);
	}
	
	public void setOtimizadoOperacional(Boolean value) {
		set(PROPERTY_OTIMIZADO_OPERACIONAL, value);
	}
	public Boolean getOtimizadoOperacional() {
		return get(PROPERTY_OTIMIZADO_OPERACIONAL);
	}
	
	public void setPublicado(Boolean value) {
		set(PROPERTY_PUBLICADO, value);
	}
	public Boolean getPublicado() {
		return get(PROPERTY_PUBLICADO);
	}
	public void setQtdProfessorVirtual(String value){
		set(PROPERTY_QTD_PROFESSOR_VIRTUAL, value);
	}
	public String getQtdProfessorVirtual() {
		return get(PROPERTY_QTD_PROFESSOR_VIRTUAL);
	}
	
	@Override
	public String getNaturalKey() {
		return getCodigo();
	}

	@Override
	public int compareTo(CampusDTO o) {
		return getCodigo().compareTo(o.getCodigo());
	}
}