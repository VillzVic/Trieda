package com.gapso.web.trieda.shared.dtos;

public class EquivalenciaDTO extends AbstractDTO<String>
	implements Comparable<EquivalenciaDTO>
{
	private static final long serialVersionUID = 5815525344760896272L;
	
	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_CURSOU_ID = "cursouId";
	public static final String PROPERTY_CURSOU_CODIGO = "cursouCod";
	public static final String PROPERTY_CURSOU_STRING = "cursouString";
	public static final String PROPERTY_ELIMINA_ID = "eliminaId";
	public static final String PROPERTY_ELIMINA_CODIGO= "eliminaCod";
	public static final String PROPERTY_ELIMINA_STRING = "eliminaString";
	public static final String PROPERTY_EQV_GERAL = "equivalenciaGeral";
	public static final String PROPERTY_CURSO_ID = "cursoId";
	public static final String PROPERTY_CURSO_STRING = "cursoString";
	
	public EquivalenciaDTO() {
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
	
	public void setCursouId(Long value) {
		set(PROPERTY_CURSOU_ID, value);
	}
	public Long getCursouId() {
		return get(PROPERTY_CURSOU_ID);
	}
	
	public void setCursouCodigo(String value) {
		set(PROPERTY_CURSOU_CODIGO, value);
	}
	public String getCursouCodigo() {
		return get(PROPERTY_CURSOU_CODIGO);
	}

	public void setCursouNome(String value) {
		set(PROPERTY_CURSOU_STRING, value);
	}
	public String getCursouNome() {
		return get(PROPERTY_CURSOU_STRING);
	}
	
	public void setEliminaId(Long value) {
		set(PROPERTY_ELIMINA_ID, value);
	}
	public Long getEliminaId() {
		return get(PROPERTY_ELIMINA_ID);
	}
	
	public void setEliminaCodigo(String value) {
		set(PROPERTY_ELIMINA_CODIGO, value);
	}
	public String getEliminaCodigo() {
		return get(PROPERTY_ELIMINA_CODIGO);
	}
	
	public void setEliminaNome(String value) {
		set(PROPERTY_ELIMINA_STRING, value);
	}
	public String getEliminaNome() {
		return get(PROPERTY_ELIMINA_STRING);
	}
	
	public void setEquivalenciaGeral(Boolean value) {
		set(PROPERTY_EQV_GERAL, value);
	}
	public Boolean getEquivalenciaGeral() {
		return get(PROPERTY_EQV_GERAL);
	}
	
	public Long getCursoId() {
		return get(PROPERTY_CURSO_ID);
	}
	
	public void setCursoId(Long value) {
		set(PROPERTY_CURSO_ID, value);
	}
	
	public String getCursoString() {
		return get(PROPERTY_CURSO_STRING);
	}
	
	public void setCursoString(String value) {
		set(PROPERTY_CURSO_STRING, value);
	}
	
	@Override
	public String getNaturalKey() {
		return getCursouNome() + "-" + getEliminaNome();
	}

	@Override
	public int compareTo(EquivalenciaDTO o) {
		int result = getCursouNome().compareTo(o.getCursouNome());
		if (result == 0) {
			result = getEliminaNome().compareTo(o.getEliminaNome());
		}
		return result;
	}
}
