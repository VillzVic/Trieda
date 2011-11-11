package com.gapso.web.trieda.shared.dtos;

import java.util.Date;
import java.util.Map;

public class SemanaLetivaDTO
	extends AbstractDTO< String >
	implements Comparable< SemanaLetivaDTO >
{
	private static final long serialVersionUID = -5419695301010396477L;

	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_CODIGO = "codigo";
	public static final String PROPERTY_DESCRICAO = "descricao";
	public static final String PROPERTY_TEMPO = "tempo";
	public static final String PROPERTY_MAX_CREDITOS = "maxCreditos";

	private Map< Long, String > horariosStringMap;
	private Map< Long, Date > horariosInicioMap;

	public SemanaLetivaDTO()
	{
		super();
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

	public void setCodigo( String value )
	{
		set( PROPERTY_CODIGO, value );
	}

	public String getCodigo()
	{
		return get( PROPERTY_CODIGO );
	}

	public void setDescricao( String value )
	{
		set( PROPERTY_DESCRICAO, value );
	}

	public String getDescricao()
	{
		return get( PROPERTY_DESCRICAO );
	}

	public void setTempo( Integer value )
	{
		set( PROPERTY_TEMPO, value );
	}

	public Integer getTempo()
	{
		return get( PROPERTY_TEMPO );
	}

	public void setMaxCreditos( Integer value )
	{
		set( PROPERTY_MAX_CREDITOS, value );
	}

	public Integer getMaxCreditos()
	{
		return get( PROPERTY_MAX_CREDITOS );
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

	@Override
	public String getNaturalKey()
	{
		return getCodigo();
	}

	@Override
	public int compareTo( SemanaLetivaDTO o )
	{
		return getCodigo().compareTo( o.getCodigo() );
	}	
}
