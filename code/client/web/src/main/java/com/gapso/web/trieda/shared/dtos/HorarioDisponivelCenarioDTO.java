package com.gapso.web.trieda.shared.dtos;

import java.util.Date;

public class HorarioDisponivelCenarioDTO
	extends AbstractDTO< String >
	implements Comparable< HorarioDisponivelCenarioDTO >
{
	private static final long serialVersionUID = -4670030478798237916L;

	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_TURNO_STRING = "turnoString";
	public static final String PROPERTY_HORARIO_DATE = "horarioDate";
	public static final String PROPERTY_HORARIO_STRING = "horarioString";
	public static final String PROPERTY_SEGUNDA = "segunda";
	public static final String PROPERTY_SEGUNDA_ID = "segundaId";
	public static final String PROPERTY_TERCA = "terca";
	public static final String PROPERTY_TERCA_ID = "tercaId";
	public static final String PROPERTY_QUARTA = "quarta";
	public static final String PROPERTY_QUARTA_ID = "quartaId";
	public static final String PROPERTY_QUINTA = "quinta";
	public static final String PROPERTY_QUINTA_ID = "quintaId";
	public static final String PROPERTY_SEXTA = "sexta";
	public static final String PROPERTY_SEXTA_ID = "sextaId";
	public static final String PROPERTY_SABADO = "sabado";
	public static final String PROPERTY_SABADO_ID = "sabadoId";
	public static final String PROPERTY_DOMINGO = "domingo";
	public static final String PROPERTY_DOMINGO_ID = "domingoId";
	public static final String PROPERTY_SEMANA_LETIVA_ID = "semanaLetivaId";

	public HorarioDisponivelCenarioDTO()
	{
		super();
	}

	public void setHorarioDeAulaId( Long value )
	{
		set( PROPERTY_ID, value );
	}

	public Long getHorarioDeAulaId()
	{
		return get( PROPERTY_ID );
	}

	public void setHorarioDeAulaVersion( Integer value )
	{
		set( PROPERTY_VERSION, value );
	}

	public Integer getHorarioDeAulaVersion()
	{
		return get( PROPERTY_VERSION );
	}

	public void setTurnoString( String value )
	{
		set( PROPERTY_TURNO_STRING, value );
	}

	public String getTurnoString()
	{
		return get( PROPERTY_TURNO_STRING );
	}

	public void setHorario( Date value )
	{
		set( PROPERTY_HORARIO_DATE, value );
	}

	public Date getHorario()
	{
		return get( PROPERTY_HORARIO_DATE );
	}

	public void setHorarioString( String value )
	{
		set( PROPERTY_HORARIO_STRING, value );
	}

	public String getHorarioString()
	{
		return get( PROPERTY_HORARIO_STRING );
	}

	public void setSegunda( Boolean value )
	{
		set( PROPERTY_SEGUNDA, value );
	}

	public Boolean getSegunda()
	{
		return get( PROPERTY_SEGUNDA );
	}

	public void setSegundaId( Long value )
	{
		set( PROPERTY_SEGUNDA_ID, value );
	}

	public Long getSegundaId()
	{
		return get( PROPERTY_SEGUNDA_ID );
	}

	public void setTerca( Boolean value )
	{
		set( PROPERTY_TERCA, value );
	}

	public Boolean getTerca()
	{
		return get( PROPERTY_TERCA );
	}

	public void setTercaId( Long value )
	{
		set( PROPERTY_TERCA_ID, value );
	}

	public Long getTercaId()
	{
		return get( PROPERTY_TERCA_ID );
	}

	public void setQuarta( Boolean value )
	{
		set( PROPERTY_QUARTA, value );
	}

	public Boolean getQuarta()
	{
		return get( PROPERTY_QUARTA );
	}

	public void setQuartaId( Long value )
	{
		set( PROPERTY_QUARTA_ID, value );
	}

	public Long getQuartaId()
	{
		return get( PROPERTY_QUARTA_ID );
	}

	public void setQuinta( Boolean value )
	{
		set( PROPERTY_QUINTA, value );
	}

	public Boolean getQuinta()
	{
		return get( PROPERTY_QUINTA );
	}

	public void setQuintaId( Long value )
	{
		set( PROPERTY_QUINTA_ID, value );
	}

	public Long getQuintaId()
	{
		return get( PROPERTY_QUINTA_ID );
	}

	public void setSexta( Boolean value )
	{
		set( PROPERTY_SEXTA, value );
	}

	public Boolean getSexta()
	{
		return get( PROPERTY_SEXTA );
	}

	public void setSextaId( Long value )
	{
		set( PROPERTY_SEXTA_ID, value );
	}

	public Long getSextaId()
	{
		return get( PROPERTY_SEXTA_ID );
	}

	public void setSabado( Boolean value )
	{
		set( PROPERTY_SABADO, value );
	}

	public Boolean getSabado()
	{
		return get( PROPERTY_SABADO );
	}

	public void setSabadoId( Long value )
	{
		set( PROPERTY_SABADO_ID, value );
	}

	public Long getSabadoId()
	{
		return get( PROPERTY_SABADO_ID );
	}

	public void setDomingo( Boolean value )
	{
		set( PROPERTY_DOMINGO, value );
	}

	public Boolean getDomingo()
	{
		return get( PROPERTY_DOMINGO );
	}

	public void setDomingoId( Long value )
	{
		set( PROPERTY_DOMINGO_ID, value );
	}

	public Long getDomingoId()
	{
		return get( PROPERTY_DOMINGO_ID );
	}

	public void setSemanaLetivaId( Long value )
	{
		set( PROPERTY_SEMANA_LETIVA_ID, value );
	}

	public Long getSemanaLetivaId()
	{
		return get( PROPERTY_SEMANA_LETIVA_ID );
	}

	@Override
	public String getNaturalKey()
	{
		return getTurnoString() + "-" + getHorarioString();
	}

	@Override
	public int compareTo( HorarioDisponivelCenarioDTO o )
	{
		int result = this.getSemanaLetivaId().compareTo( o.getSemanaLetivaId() );

		if ( result == 0 )
		{
			result = getHorario().compareTo( o.getHorario() );
		}

		return result;
	}
}
