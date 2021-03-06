package com.gapso.web.trieda.shared.dtos;

import java.util.Date;
import java.util.HashMap;
import java.util.Map;

public class TurnoDTO
	extends AbstractDTO< String >
	implements Comparable< TurnoDTO >
{
	private static final long serialVersionUID = 5815525344760896272L;

	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_CENARIO_ID = "cenarioId";
	public static final String PROPERTY_NOME = "nome";
	public static final String PROPERTY_MAX_CREDITOS = "maxCreditos";
	public static final String PROPERTY_INSTITUICAO_ENSINO_ID = "instituicaoEnsinoId";
	public static final String PROPERTY_INSTITUICAO_ENSINO_STRING = "instituicaoEnsinoString";

	private Map< Long, Map< Integer, Integer > > countHorariosAula;
	private Map< Long, String > horariosStringMap;
	private Map< Long, Date > horariosInicioMap;

	public TurnoDTO()
	{
		this.countHorariosAula = new HashMap< Long, Map< Integer, Integer > >();
	}

	public void setId( Long value )
	{
		set( PROPERTY_ID, value );
	}

	public Long getId()
	{
		return get( PROPERTY_ID );
	}

	public void setVersion( Integer value )
	{
		set( PROPERTY_VERSION, value );
	}

	public Integer getVersion()
	{
		return get( PROPERTY_VERSION );
	}

	public void setCenarioId( Long value )
	{
		set( PROPERTY_CENARIO_ID, value );
	}

	public Long getCenarioId()
	{
		return get( PROPERTY_CENARIO_ID );
	}

	public void setNome( String value )
	{
		set( PROPERTY_NOME, value );
	}

	public String getNome()
	{
		return get( PROPERTY_NOME );
	}

	public void setMaxCreditos( Integer value )
	{
		set( PROPERTY_MAX_CREDITOS, value );
	}

	public Integer getMaxCreditos()
	{
		return get( PROPERTY_MAX_CREDITOS );
	}

	public Map< Long, Map< Integer, Integer > > getCountHorariosAula()
//	public Map< Integer, Integer > getCountHorariosAula()
	{
		return this.countHorariosAula;
	}

	public void setCountHorariosAula(
		Map< Long, Map< Integer, Integer > > countHorariosAula )
//	Map< Integer, Integer > countHorariosAula )
	{
		this.countHorariosAula = countHorariosAula;
	}

	public void setHorariosStringMap(
		Map< Long, String > horariosStringMap )
	{
		this.horariosStringMap = horariosStringMap;
	}

	public Map< Long, String > getHorariosStringMap()
	{
		return this.horariosStringMap;
	}

	public void setHorariosInicioMap(
		Map< Long, Date > horariosStringMap )
	{
		this.horariosInicioMap = horariosStringMap;
	}

	public Map< Long, Date > getHorariosInicioMap()
	{
		return this.horariosInicioMap;
	}

	public int getMaxCreditos(Long semanaLetivaId, int diaSemana )
	{
		Integer value = this.countHorariosAula.get(semanaLetivaId).get( diaSemana );
		return ( ( value != null ) ? value : 0 );
	}

	@Override
	public String getNaturalKey()
	{
		return this.getNome();
	}	

	@Override
	public int compareTo( TurnoDTO o )
	{
		return this.getNome().compareTo( o.getNome() );
	}
}
