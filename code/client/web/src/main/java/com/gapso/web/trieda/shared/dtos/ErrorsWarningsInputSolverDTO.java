package com.gapso.web.trieda.shared.dtos;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class ErrorsWarningsInputSolverDTO
	extends AbstractDTO< String >
{
	private static final long serialVersionUID = -5134820110949139907L;

	public static final String PROPERTY_ERRORS_WARNINGS = "errorsWarnings";
	public static final String PROPERTY_VALID_INPUT = "validInput";

	public ErrorsWarningsInputSolverDTO()
	{
		super();
		this.setValidInput( false );

		Map< String, List< String > > map
			= new HashMap< String, List< String > >();

		map.put( "errors", new ArrayList< String >() );
		map.put( "warnings", new ArrayList< String >() );

		setErrorsWarnings( map );
	}

	public void setErrorsWarnings( Map< String, List< String > > value )
	{
		set( PROPERTY_ERRORS_WARNINGS, value );
	}

	public Map< String, List< String > > getErrorsWarnings()
	{
		return get( PROPERTY_ERRORS_WARNINGS );
	}

	public void setValidInput( Boolean value )
	{
		set( PROPERTY_VALID_INPUT, value );
	}

	public Boolean getValidInput()
	{
		return get( PROPERTY_VALID_INPUT );
	}

	public Integer getTotalErrorsWarnings()
	{
		return this.getErrorsWarnings().get( "errors" ).size()
			+ this.getErrorsWarnings().get( "warnings" ).size(); 
	}

	@Override
	public String getNaturalKey()
	{
		if ( getValidInput() == false )
		{
			return "Input invalido.";
		}

		if ( getErrorsWarnings().get( "errors" ).size() > 0
			|| getErrorsWarnings().get( "warnings" ).size() > 0 )
		{
			return "Input valido, contendo alguns alertas.";
		}

		return "Input valido.";
	}
}
