package com.gapso.web.trieda.shared.dtos;

public class InstituicaoEnsinoDTO extends AbstractDTO<String>
	implements Comparable< InstituicaoEnsinoDTO >
{
	private static final long serialVersionUID = -2710219870848022928L;

	// Propriedades
	public static final String PROPERTY_ID = "id";
	public static final String PROPERTY_VERSION = "version";
	public static final String PROPERTY_NOME_INSTITUICAO = "nomeInstituicao";
	

	public InstituicaoEnsinoDTO()
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

	public void setNomeInstituicao( String value )
	{
		set( PROPERTY_NOME_INSTITUICAO, value );
	}

	public String getNomeInstituicao()
	{
		return get( PROPERTY_NOME_INSTITUICAO );
	}

	@Override
	public String getNaturalKey()
	{
		return getNomeInstituicao();
	}

	@Override
	public int compareTo( InstituicaoEnsinoDTO o )
	{
		return getNomeInstituicao().compareTo( o.getNomeInstituicao() );
	}
}
