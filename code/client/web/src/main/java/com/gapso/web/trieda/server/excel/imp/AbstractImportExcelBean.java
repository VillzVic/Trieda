package com.gapso.web.trieda.server.excel.imp;

import java.util.List;

import com.gapso.web.trieda.shared.util.TriedaUtil;

public abstract class AbstractImportExcelBean
{
	private int row;

	public AbstractImportExcelBean( int row )
	{
		this.row = row;
	}

	public int getRow()
	{
		return this.row;
	}

	protected boolean isEmptyField( String value )
	{
		return ( value == null || value.equals( "" ) );
	}

	protected void checkMandatoryField( String value,
		ImportExcelError errorType, List< ImportExcelError > errorsList )
	{
		if ( isEmptyField( value ) )
		{
			errorsList.add( errorType );
		}
	}

	protected Double checkNonNegativeDoubleField(
		String value, ImportExcelError doubleErrorType,
		ImportExcelError nonNegativeErrorType,
		List< ImportExcelError > errorsList )
	{
		Double doubleValue = null;

		if ( !isEmptyField( value ) )
		{
			try
			{
				value = TriedaUtil.financialFormatToDoubleFormat( value );
				doubleValue = Double.valueOf( value );

				if ( doubleValue < 0.0 )
				{
					errorsList.add( nonNegativeErrorType );
				}
			}
			catch ( NumberFormatException e )
			{
				errorsList.add( doubleErrorType );
			}
		}

		return doubleValue;
	}
	
	protected Integer checkNonNegativeIntegerField(
		String value, ImportExcelError integerErrorType,
		ImportExcelError nonNegativeErrorType,
		List< ImportExcelError > errorsList )
	{
		Integer integerValue = null;

		if ( !isEmptyField( value ) )
		{
			try
			{
				value = TriedaUtil.financialFormatToDoubleFormat( value );
				integerValue = Double.valueOf( value ).intValue();

				if ( integerValue < 0 )
				{
					errorsList.add( nonNegativeErrorType );
				}
			}
			catch ( NumberFormatException e )
			{
				errorsList.add( integerErrorType );
			}
		}

		return integerValue;
	}

	protected Boolean checkBooleanField(
		String value, ImportExcelError errorType,
		List< ImportExcelError > errorsList )
	{
		Boolean booleanValue = null;

		if ( !isEmptyField( value ) )
		{
			if ( "Sim".equalsIgnoreCase( value ) )
			{
				booleanValue = true;
			}
			else
			{
				booleanValue = false;
			}
		}

		if ( booleanValue == null )
		{
			errorsList.add( errorType );
		}

		return booleanValue;
	}

	protected < EnumType > EnumType checkEnumField(
		String value, Class< EnumType > enumClass,
		ImportExcelError enumErrorTupe, List< ImportExcelError > errorsList )
	{
		EnumType enumValue = null;

		if ( enumClass.isEnum() )
		{
			for ( EnumType enumConstant : enumClass.getEnumConstants() )
			{
				if ( enumConstant.toString().equalsIgnoreCase( value ) )
				{
					enumValue = enumConstant;
					break;
				}
			}

			if ( enumValue == null )
			{
				errorsList.add( enumErrorTupe );
			}
		}

		return enumValue;
	}
}
