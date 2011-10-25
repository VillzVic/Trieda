package com.gapso.web.trieda.shared.util.view;

import java.io.Serializable;

public class TriedaException
	extends Exception
	implements Serializable
{
	private static final long serialVersionUID = 7252239658737885318L;

	public TriedaException( )
	{
		super();
	}

	public TriedaException( Exception e )
	{
		super( e.getMessage(), e.getCause() );
	}
}