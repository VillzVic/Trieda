package com.gapso.web.trieda.shared.dtos;


public class AreaTitulacaoDTO
	extends AbstractDTO< String >
	implements Comparable< AreaTitulacaoDTO >
{
	private static final long serialVersionUID = -5134820110949139907L;

	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_CODIGO = "codigo";
	public static final String PROPERTY_DESCRICAO = "descricao";
	public static final String PROPERTY_CENARIO_ID = "cenarioId";

	public AreaTitulacaoDTO()
	{
		super();
	}

	public AreaTitulacaoDTO( Long id, String codigo, String descricao,
		Integer version, Long instituicaoEnsinoId, String instituicaoEnsinoString,
		Long cenarioId )
	{
		setId( id );
		setCodigo( codigo );
		setDescricao( descricao );
		setVersion( version );
		setInstituicaoEnsinoId( instituicaoEnsinoId );
		setInstituicaoEnsinoString( instituicaoEnsinoString );
		setCenarioId(cenarioId);

		setDisplayText( codigo + " (" + descricao + ")" );
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
	
	public Long getCenarioId()
	{
		return get( PROPERTY_CENARIO_ID );
	}

	public void setCenarioId( Long value )
	{
		set( PROPERTY_CENARIO_ID, value );
	}

	@Override
	public String getNaturalKey()
	{
		return getCodigo();
	}

	@Override
	public int compareTo( AreaTitulacaoDTO o )
	{
		return getCodigo().compareTo( o.getCodigo() );
	}
}
