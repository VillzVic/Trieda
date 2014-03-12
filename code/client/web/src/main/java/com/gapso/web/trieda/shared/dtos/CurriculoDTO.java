package com.gapso.web.trieda.shared.dtos;

import java.util.Set;

public class CurriculoDTO extends AbstractDTO< String >
	implements Comparable< CurriculoDTO >
{
	private static final long serialVersionUID = -5134820110949139907L;

	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_CENARIO_ID = "cenarioId";
	public static final String PROPERTY_CODIGO = "codigo";
	public static final String PROPERTY_DESCRICAO = "descricao";
	public static final String PROPERTY_CURSO_ID = "cursoId";
	public static final String PROPERTY_CURSO_STRING = "cursoString";
	public static final String PROPERTY_PERIODOS = "periodos";
	public static final String PROPERTY_PERIODOS_LIST = "periodosList";
	public static final String PROPERTY_SEMANA_LETIVA_ID = "semanaLetivaId";
	public static final String PROPERTY_SEMANA_LETIVA_STRING = "semanaLetivaString";

	public CurriculoDTO()
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

	public void setCenarioId( Long value )
	{
		set( PROPERTY_CENARIO_ID, value );
	}

	public Long getCenarioId()
	{
		return get( PROPERTY_CENARIO_ID );
	}

	public String getCodigo()
	{
		return get( PROPERTY_CODIGO );
	}

	public void setCodigo( String value )
	{
		set( PROPERTY_CODIGO, value );
	}
	
	public String getDescricao()
	{
		return get( PROPERTY_DESCRICAO );
	}

	public void setDescricao( String value )
	{
		set( PROPERTY_DESCRICAO, value );
	}
	
	public Long getCursoId()
	{
		return get( PROPERTY_CURSO_ID );
	}

	public void setCursoId( Long value )
	{
		set( PROPERTY_CURSO_ID, value );
	}

	public String getCursoString()
	{
		return get(PROPERTY_CURSO_STRING);
	}

	public void setCursoString( String value )
	{
		set( PROPERTY_CURSO_STRING, value );
	}
	
	public void setPeriodos( String value )
	{
		set( PROPERTY_PERIODOS, value );
	}

	public String getPeriodos()
	{
		return get( PROPERTY_PERIODOS );
	}
	
	public void setPeriodosList( Set<Integer> value )
	{
		set( PROPERTY_PERIODOS_LIST, value );
	}

	public Set<Integer> getPeriodosList()
	{
		return get( PROPERTY_PERIODOS_LIST );
	}

	public void setSemanaLetivaId( Long value )
	{
		set( PROPERTY_SEMANA_LETIVA_ID, value );
	}

	public Long getSemanaLetivaId()
	{
		return get( PROPERTY_SEMANA_LETIVA_ID );
	}

	public void setSemanaLetivaString( String value )
	{
		set( PROPERTY_SEMANA_LETIVA_STRING, value );
	}

	public String getSemanaLetivaString()
	{
		return get( PROPERTY_SEMANA_LETIVA_STRING );
	}

	@Override
	public String getNaturalKey()
	{
		return getCursoString() + "-" + getCodigo();
	}

	@Override
	public int compareTo( CurriculoDTO o )
	{
		int result = getCursoString().compareTo( o.getCursoString() );
		if ( result == 0 )
		{
			result = getCodigo().compareTo( o.getCodigo() );
		}

		return result;
	}
}
