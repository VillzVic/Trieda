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
		return ( 31 * this.left.hashCode() * this.right.hashCode() );
	}

	@SuppressWarnings( { "unchecked", "rawtypes" } )
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

	    TriedaPair pair = (TriedaPair) o;

	    if ( this.getLeft() == null )
	    {
	    	if ( pair.getLeft() != null )
	    	{
	    		return false;
	    	}
	    }
	    else if ( this.getLeft().getClass() != pair.getLeft().getClass() )
	    {
	    	return false;
	    }

	    if ( this.getRight() == null )
	    {
	    	if ( pair.getRight() != null )
	    	{
	    		return false;
	    	}
	    }
	    else if ( this.getRight().getClass() != pair.getRight().getClass() )
	    {
	    	return false;
	    }

	    TriedaPair< L, R > pairo = (TriedaPair< L, R >) o;

	    return this.getLeft().equals( pairo.getLeft() )
	    	&& this.getRight().equals( pairo.getRight() );
	}
}
