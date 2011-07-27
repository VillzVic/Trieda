package com.gapso.web.trieda.shared.dtos;

import com.gapso.web.trieda.shared.util.TriedaCurrency;
import com.gapso.web.trieda.shared.util.TriedaUtil;

public class ProfessorDTO extends AbstractDTO<String> implements
		Comparable<ProfessorDTO> {
	private static final long serialVersionUID = 5815525344760896272L;

	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_CENARIO_ID = "cenarioId";
	public static final String PROPERTY_NOME = "nome";
	public static final String PROPERTY_CPF = "cpf";
	public static final String PROPERTY_TIPO_CONTRATO_STRING = "tipoContratoString";
	public static final String PROPERTY_TIPO_CONTRATO_ID = "tipoContratoId";
	public static final String PROPERTY_CARGA_HORARIA_MAX = "cargaHorariaMax";
	public static final String PROPERTY_CARGA_HORARIA_MIN = "cargaHorariaMin";
	public static final String PROPERTY_TITULACAO_STRING = "titulacaoString";
	public static final String PROPERTY_TITULACAO_ID = "titulacaoId";
	public static final String PROPERTY_AREA_TITULACAO_STRING = "areaTitulacaoString";
	public static final String PROPERTY_AREA_TITULACAO_ID = "areaTitulacaoId";
	public static final String PROPERTY_NOTA_DESEMPENHO = "notaDesempenho";
	public static final String PROPERTY_CREDITO_ANTERIOR = "creditoAnterior";
	public static final String PROPERTY_VALOR_CREDITO = "valorCredito";

	public ProfessorDTO() {
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

	public void setNome(String value) {
		set(PROPERTY_NOME, value);
	}

	public String getNome() {
		return get(PROPERTY_NOME);
	}

	public void setCpf(String value) {
		set(PROPERTY_CPF, value);
	}

	public String getCpf() {
		return get(PROPERTY_CPF);
	}

	public void setTipoContratoString(String value) {
		set(PROPERTY_TIPO_CONTRATO_STRING, value);
	}

	public String getTipoContratoString() {
		return get(PROPERTY_TIPO_CONTRATO_STRING);
	}

	public void setTipoContratoId(Long value) {
		set(PROPERTY_TIPO_CONTRATO_ID, value);
	}

	public Long getTipoContratoId() {
		return get(PROPERTY_TIPO_CONTRATO_ID);
	}

	public void setCargaHorariaMax(Integer value) {
		set(PROPERTY_CARGA_HORARIA_MAX, value);
	}

	public Integer getCargaHorariaMax() {
		return get(PROPERTY_CARGA_HORARIA_MAX);
	}

	public void setCargaHorariaMin(Integer value) {
		set(PROPERTY_CARGA_HORARIA_MIN, value);
	}

	public Integer getCargaHorariaMin() {
		return get(PROPERTY_CARGA_HORARIA_MIN);
	}

	public void setTitulacaoString(String value) {
		set(PROPERTY_TITULACAO_STRING, value);
	}

	public String getTitulacaoString() {
		return get(PROPERTY_TITULACAO_STRING);
	}

	public void setTitulacaoId(Long value) {
		set(PROPERTY_TITULACAO_ID, value);
	}

	public Long getTitulacaoId() {
		return get(PROPERTY_TITULACAO_ID);
	}

	public void setAreaTitulacaoString(String value) {
		set(PROPERTY_AREA_TITULACAO_STRING, value);
	}

	public String getAreaTitulacaoString() {
		return get(PROPERTY_AREA_TITULACAO_STRING);
	}

	public void setAreaTitulacaoId(Long value) {
		set(PROPERTY_AREA_TITULACAO_ID, value);
	}

	public Long getAreaTitulacaoId() {
		return get(PROPERTY_AREA_TITULACAO_ID);
	}

	public void setNotaDesempenho(Integer value) {
		set(PROPERTY_NOTA_DESEMPENHO, value);
	}

	public Integer getNotaDesempenho() {
		return get(PROPERTY_NOTA_DESEMPENHO);
	}

	public void setCreditoAnterior(Integer value) {
		set(PROPERTY_CREDITO_ANTERIOR, value);
	}

	public Integer getCreditoAnterior() {
		return get(PROPERTY_CREDITO_ANTERIOR);
	}

	public void setValorCredito(TriedaCurrency value) {
		set(PROPERTY_VALOR_CREDITO, value.toString());
	}

	public TriedaCurrency getValorCredito() {
		return TriedaUtil.parseTriedaCurrency(get(PROPERTY_VALOR_CREDITO));
	}

	@Override
	public String getNaturalKey() {
		return getCpf();
	}

	@Override
	public int compareTo(ProfessorDTO o) {
		int result = getCpf().compareTo(o.getCpf());
		if (result == 0) {
			result = getNome().compareTo(o.getNome());
		}
		return result;
	}
}
