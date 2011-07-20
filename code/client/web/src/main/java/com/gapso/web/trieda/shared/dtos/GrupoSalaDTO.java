package com.gapso.web.trieda.shared.dtos;

public class GrupoSalaDTO extends AbstractDTO<String>
	implements Comparable<GrupoSalaDTO>
{
	private static final long serialVersionUID = -5134820110949139907L;

	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_CODIGO = "codigo";
	public static final String PROPERTY_NOME = "nome";
	public static final String PROPERTY_UNIDADE_ID = "unidadeId";
	public static final String PROPERTY_UNIDADE_STRING = "unidadeString";
	public static final String PROPERTY_CAMPUS_ID = "campusId";
	public static final String PROPERTY_CAMPUS_STRING = "campusString";
	public static final String PROPERTY_SALAS_STRING = "salasString";

	public GrupoSalaDTO() {
		super();
	}

	public GrupoSalaDTO(Long id, String codigo, String nome,
		Long unidadeId, String unidadeString, Integer version)
	{
		setId(id);
		setCodigo(codigo);
		setNome(nome);
		setUnidadeId(unidadeId);
		setUnidadeString(unidadeString);
		setVersion(version);
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
	
	public Long getUnidadeId() {
		return get(PROPERTY_UNIDADE_ID);
	}
	public void setUnidadeId(Long value) {
		set(PROPERTY_UNIDADE_ID, value);
	}

	public String getUnidadeString() {
		return get(PROPERTY_UNIDADE_STRING);
	}
	public void setUnidadeString(String value) {
		set(PROPERTY_UNIDADE_STRING, value);
	}
	
	public String getCampusString() {
		return get(PROPERTY_CAMPUS_STRING);
	}
	public void setCampusString(String value) {
		set(PROPERTY_CAMPUS_STRING, value);
	}
	
	public Long getCampusId() {
		return get(PROPERTY_CAMPUS_ID);
	}
	public void setCampusId(Long value) {
		set(PROPERTY_CAMPUS_ID, value);
	}
	
	public String getSalasString() {
		return get(PROPERTY_SALAS_STRING);
	}
	public void setSalasString(String value) {
		set(PROPERTY_SALAS_STRING, value);
	}
	
	@Override
	public String getNaturalKey() {
		return getCampusId() + "-" + getUnidadeId() + "-" + getCodigo();
	}

	@Override
	public int compareTo(GrupoSalaDTO o) {
		int result = getCampusString().compareTo(o.getCampusString());
		if (result == 0) {
			result = getUnidadeString().compareTo(o.getUnidadeString());
			if (result == 0) {
				result = getCodigo().compareTo(o.getCodigo());
			}
		}
		return result;
	}	
}
