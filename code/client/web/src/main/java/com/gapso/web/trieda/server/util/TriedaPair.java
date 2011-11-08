package com.gapso.web.trieda.server.util;

public class TriedaPair< L, R >
{
	private final L left;
	private final R right;

	public TriedaPair( L left, R right )
	{
		this.left = left;
		this.right = right;
	}

	public L getLeft()
	{
		return this.left;
	}

	public R getRight()
	{
		return this.right;
	}

	@Override
	public int hashCode()
	{
		return this.left.hashCode() ^ this.right.hashCode();
	}

	@SuppressWarnings( "rawtypes" )
	@Override
	public boolean equals( Object o )
	{
	    if ( o == null )
	    {
	    	return false;
	    }
	
	    if ( !( o instanceof TriedaPair ) )
	    {
	    	return false;
	    }

	    TriedaPair pairo = (TriedaPair) o;
	    return this.left.equals( pairo.getLeft() )
	    	&& this.right.equals( pairo.getRight() );
	}
}
