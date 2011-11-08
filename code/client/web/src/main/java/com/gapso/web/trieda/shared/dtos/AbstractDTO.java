package com.gapso.web.trieda.shared.dtos;

import com.extjs.gxt.ui.client.data.BaseModel;

public abstract class AbstractDTO< NKType >
	extends BaseModel
{
	private static final long serialVersionUID = 2957871960531183286L;

	// Propriedades
	public static final String PROPERTY_DISPLAY_TEXT = "displayText";
	public static final String PROPERTY_INSTITUICAO_ENSINO_ID = "instituicaoEnsinoId";
	public static final String PROPERTY_INSTITUICAO_ENSINO_STRING = "instituicaoEnsinoString";

	public abstract NKType getNaturalKey();

	public void setDisplayText( String value )
	{
		set( PROPERTY_DISPLAY_TEXT, value );
	}

	public String getDisplayText()
	{
		return get( PROPERTY_DISPLAY_TEXT );
	}

	final public void setInstituicaoEnsinoId( Long value )
	{
		set( PROPERTY_INSTITUICAO_ENSINO_ID, value );
	}

	final public Long getInstituicaoEnsinoId()
	{
		return get( PROPERTY_INSTITUICAO_ENSINO_ID );
	}

	final public void setInstituicaoEnsinoString( String value )
	{
		set( PROPERTY_INSTITUICAO_ENSINO_STRING, value );
	}

	final public String getInstituicaoEnsinoString()
	{
		return get( PROPERTY_INSTITUICAO_ENSINO_STRING );
	}

	@Override
	public int hashCode()
	{
		final int prime = 31;
		int result = 1;

		result = ( prime * result + (
			( getNaturalKey() == null ) ? 0 : getNaturalKey().hashCode() ) );

		return result;
	}

	@Override
	public boolean equals( Object obj )
	{
		if ( this == obj )
		{
			return true;
		}

		if ( obj == null )
		{
			return false;
		}

		if ( getClass() != obj.getClass() )
		{
			return false;
		}

		@SuppressWarnings( "unchecked" )
		AbstractDTO< NKType > other
			= ( AbstractDTO< NKType > ) obj;

		if ( getNaturalKey() == null )
		{
			if ( other.getNaturalKey() != null )
			{
				return false;
			}
		}
		else if ( !getNaturalKey().equals( other.getNaturalKey() ) )
		{
			return false;
		}

		return true;
	}

	@Override
	public String toString()
	{
		return getDisplayText();
	}
}