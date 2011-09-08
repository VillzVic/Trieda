package com.gapso.web.trieda.shared.dtos;




public class ProfessorVirtualDTO extends AbstractDTO<Long> implements Comparable<ProfessorVirtualDTO> {

	private static final long serialVersionUID = 5815525344760896272L;
	
	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_NOME = "nome";
	public static final String PROPERTY_TIPO_CONTRATO_STRING = "tipoContratoString";
	public static final String PROPERTY_TIPO_CONTRATO_ID = "tipoContratoId";
	public static final String PROPERTY_CARGA_HORARIA_MAX = "cargaHorariaMax";
	public static final String PROPERTY_CARGA_HORARIA_MIN = "cargaHorariaMin";
	public static final String PROPERTY_TITULACAO_STRING = "titulacaoString";
	public static final String PROPERTY_TITULACAO_ID = "titulacaoId";
	public static final String PROPERTY_AREA_TITULACAO_STRING = "areaTitulacaoString";
	public static final String PROPERTY_AREA_TITULACAO_ID = "areaTitulacaoId";
	
	public ProfessorVirtualDTO()
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

	public void setNome(String value) {
		set(PROPERTY_NOME, value);
	}
	public String getNome() {
		return get(PROPERTY_NOME);
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

	@Override
	public Long getNaturalKey() {
		return getId();
	}

	@Override
	public int compareTo(ProfessorVirtualDTO o) {
		return getId().compareTo(o.getId());
	}
}