package com.gapso.web.trieda.shared.dtos;

import com.gapso.web.trieda.shared.util.TriedaCurrency;
import com.gapso.web.trieda.shared.util.TriedaUtil;

public class OfertaDTO extends AbstractDTO<String>
	implements Comparable<OfertaDTO>
{
	private static final long serialVersionUID = -5134820110949139907L;

	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_CAMPUS_ID = "campusId";
	public static final String PROPERTY_CAMPUS_STRING = "campusString";
	public static final String PROPERTY_CURSO_STRING = "cursoString";
	public static final String PROPERTY_RECEITA = "receita";
	public static final String PROPERTY_MATRIZ_CURRICULAR_ID = "matrizCurricularId";
	public static final String PROPERTY_MATRIZ_CURRICULAR_STRING = "curriculoString";
	public static final String PROPERTY_TURNO_ID = "turnoId";
	public static final String PROPERTY_TURNO_STRING = "turnoString";
	public static final String PROPERTY_INSTITUICAO_ENSINO_ID = "instituicaoEnsinoId";
	public static final String PROPERTY_INSTITUICAO_ENSINO_STRING = "instituicaoEnsinoString";
	public static final String PROPERTY_TEXT = "text";

	public OfertaDTO() {
		super();
	}

	public String getText() {
		return getCampusString() + " / " + getCursoString() + " / "
		+ getMatrizCurricularString() + " / " + getTurnoString();
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

	public Long getCampusId() {
		return get(PROPERTY_CAMPUS_ID);
	}

	public void setCampusId(Long value) {
		set(PROPERTY_CAMPUS_ID, value);
	}

	public String getCampusString() {
		return get(PROPERTY_CAMPUS_STRING);
	}

	public void setCampusString(String value) {
		set(PROPERTY_CAMPUS_STRING, value);
		set(PROPERTY_TEXT, getCampusString() + " / " + getCursoString() + " / " + getMatrizCurricularString() + " / " + getTurnoString());
	}

	public String getCursoString() {
		return get(PROPERTY_CURSO_STRING);
	}

	public void setCursoString(String value) {
		set(PROPERTY_CURSO_STRING, value);
		set(PROPERTY_TEXT, getCampusString() + " / " + getCursoString() + " / " + getMatrizCurricularString() + " / " + getTurnoString());
	}

	public void setReceita( TriedaCurrency value )
	{
		set( PROPERTY_RECEITA, value.toString() );
	}

	public TriedaCurrency getReceita()
	{
		return TriedaUtil.parseTriedaCurrency( get( PROPERTY_RECEITA ) );
	}

	public Long getMatrizCurricularId() {
		return get(PROPERTY_MATRIZ_CURRICULAR_ID);
	}

	public void setMatrizCurricularId(Long value) {
		set(PROPERTY_MATRIZ_CURRICULAR_ID, value);
	}

	public String getMatrizCurricularString() {
		return get(PROPERTY_MATRIZ_CURRICULAR_STRING);
	}

	public void setMatrizCurricularString(String value) {
		set(PROPERTY_MATRIZ_CURRICULAR_STRING, value);
		set(PROPERTY_TEXT, getCampusString() + " / " + getCursoString() + " / " + getMatrizCurricularString() + " / " + getTurnoString());
	}

	public Long getTurnoId() {
		return get(PROPERTY_TURNO_ID);
	}

	public void setTurnoId(Long value) {
		set(PROPERTY_TURNO_ID, value);
	}

	public String getTurnoString() {
		return get(PROPERTY_TURNO_STRING);
	}

	public void setTurnoString(String value) {
		set(PROPERTY_TURNO_STRING, value);
		set(PROPERTY_TEXT, getCampusString() + " / " + getCursoString() + " / " + getMatrizCurricularString() + " / " + getTurnoString());
	}

	@Override
	public String getNaturalKey() {
		return getCampusString() + "-" + getCursoString() + "-"
				+ getMatrizCurricularString() + "-" + getTurnoString();
	}

	@Override
	public int compareTo(OfertaDTO o) {
		int result = getCampusString().compareTo(o.getCampusString());
		if (result == 0) {
			result = getCursoString().compareTo(o.getCursoString());
			if (result == 0) {
				result = getMatrizCurricularString().compareTo(
						o.getMatrizCurricularString());
				if (result == 0) {
					result = getTurnoString().compareTo(o.getTurnoString());
				}
			}
		}
		return result;
	}
}