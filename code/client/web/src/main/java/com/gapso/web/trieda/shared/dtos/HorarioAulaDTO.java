package com.gapso.web.trieda.shared.dtos;

import java.util.Date;

public class HorarioAulaDTO extends AbstractDTO< String >
	implements Comparable< HorarioAulaDTO >
{
	private static final long serialVersionUID = -4670030478798237916L;

	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_SEMANA_LETIVA_ID = "semanaLetivaId";
	public static final String PROPERTY_SEMANA_LETIVA_STRING = "semanaLetivaString";
	public static final String PROPERTY_TURNO_ID = "turnoId";
	public static final String PROPERTY_TURNO_STRING = "turnoString";
	public static final String PROPERTY_INICIO = "inicio";
	public static final String PROPERTY_FIM = "fim";

	public HorarioAulaDTO() {
	}

	public HorarioAulaDTO(Long id, Long semanaLetivaId,
			String semanaLetivaString, String turnoString, Long turnoId,
			Date inicio, Date fim, Integer version) {
		setId(id);
		setSemanaLetivaId(semanaLetivaId);
		setSemanaLetivaString(semanaLetivaString);
		setTurnoId(turnoId);
		setTurnoString(turnoString);
		setInicio(inicio);
		setFim(fim);
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

	public void setSemanaLetivaId(Long value) {
		set(PROPERTY_SEMANA_LETIVA_ID, value);
	}

	public Long getSemanaLetivaId() {
		return get(PROPERTY_SEMANA_LETIVA_ID);
	}

	public void setSemanaLetivaString(String value) {
		set(PROPERTY_SEMANA_LETIVA_STRING, value);
	}

	public String getSemanaLetivaString() {
		return get(PROPERTY_SEMANA_LETIVA_STRING);
	}

	public void setTurnoId(Long value) {
		set(PROPERTY_TURNO_ID, value);
	}

	public Long getTurnoId() {
		return get(PROPERTY_TURNO_ID);
	}

	public void setTurnoString(String value) {
		set(PROPERTY_TURNO_STRING, value);
	}

	public String getTurnoString() {
		return get(PROPERTY_TURNO_STRING);
	}

	public void setInicio(Date value) {
		set(PROPERTY_INICIO, value);
	}

	public Date getInicio() {
		return get(PROPERTY_INICIO);
	}

	public void setFim(Date value) {
		set(PROPERTY_FIM, value);
	}

	public Date getFim() {
		return get(PROPERTY_FIM);
	}

	@Override
	public String getNaturalKey() {
		return getSemanaLetivaString() + "-" + getTurnoString() + "-"
				+ getInicio() + "-" + getFim();
	}

	@Override
	public int compareTo(HorarioAulaDTO o) {
		int compareSemanaLetiva = getSemanaLetivaString().compareTo(
				o.getSemanaLetivaString());
		if (compareSemanaLetiva == 0) {
			int compareTurno = getTurnoString().compareTo(o.getTurnoString());
			if (compareTurno == 0) {
				return getInicio().compareTo(o.getInicio());
			}
			return compareTurno;
		}
		return compareSemanaLetiva;
	}
}