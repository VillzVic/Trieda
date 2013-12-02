package com.gapso.web.trieda.shared.util;

import java.util.Date;
import java.util.List;

import com.gapso.web.trieda.shared.dtos.AtendimentoRelatorioDTO.ReportType;
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.user.client.Window.Location;

public class TriedaUtil
{
	static public String formatStringCPF( String number )
	{
		if ( number.length() > 11 )
		{
			return number;
		}

		if ( number.length() < 11 )
		{
			String temp = "";
			int cont = ( 11 - number.length() );

			while ( cont-- > 0 )
			{
				temp += "0";
			}

			number = temp + number;
		}

		int index = 0;
		String result = "";

		for ( int i = 0; i < 3; i++ )
		{
			result += number.charAt( index );
			index++;
		}

		result += ".";

		for ( int i = 0; i < 3; i++ )
		{
			result += number.charAt( index );
			index++;
		}

		result += ".";

		for ( int i = 0; i < 3; i++ )
		{
			result += number.charAt( index );
			index++;
		}
		
		result += "-";

		for ( int i = 0; i < 2; i++ )
		{
			result += number.charAt( index );
			index++;
		}

		return result;
	}

	static public double round( double value, int precision )
	{
		if ( precision != 0 )
		{
			double precisionFactor = 10.0;
			for (int i = 1; i < precision; i++)
			{
				precisionFactor *= 10.0 ;
			}
			value = Math.round( value * precisionFactor );
			return ( value / precisionFactor );
		}

		return value;
	}

	static public String truncate( String text, int max )
	
	{
		return truncate( text, max, true );
	}

	static public String truncate( String text, int max, boolean etc )
	{
		if ( max <= 0 )
		{
			return text;
		}

		String ret = text;
		if ( ret.length() > max )
		{
			ret = ret.substring( 0, max );
			if ( etc )
			{
				ret += "...";
			}
		}

		return ret;
	}

	static public String financialFormatToDoubleFormat( String value )
	{
		boolean contemVirgula = value.contains( "," );
		boolean contemPonto = value.contains( "." );

		if ( contemVirgula && contemPonto )
		{
			value = value.replace( ".", "" );
		}

		if ( contemVirgula )
		{
			value = value.replace( ",", "." );
		}

		return value;
	}

	static public boolean isBlank( String value )
	{
		return ( ( value == null ) || value == "" );
	}

	static public boolean isBlank( Long value )
	{
		return ( value == null || value == 0 );
	}

	static public String paramsDebug()
	{
		String codesvr = Location.getParameter( "gwt.codesvr" );
		if ( isBlank( codesvr ) )
		{
			return "";
		}

		return ( "?gwt.codesvr=" + codesvr );
	}

	static public String arrayJoin( List< String > l, String s )
	{
		String ret = "";
		if ( l.size() == 0 )
		{
			return ret;
		}

		ret = l.get( 0 );
		for ( int i = 1; i < l.size(); i++ )
		{
			ret = ( ret + s + l.get( i ) );
		}

		return ret;
	}

	static public Double roundTwoDecimals( double d )
	{
		long y = (long)( d * 100 );
		return ( (double) y / 100 );
	}

	@SuppressWarnings( "deprecation" )
	static public String shortTimeString( Date date )
	{
		if ( date == null )
		{
			return "";
		}

		Integer h = date.getHours();
		String hour = h.toString();
		if ( hour.length() < 2 )
		{
			hour = "0" + hour;
		}

		Integer m = date.getMinutes();
		String minute = m.toString();
		if ( minute.length() < 2 )
		{
			minute = "0" + minute;
		}

		return ( hour + ":" + minute );
	}

	static public TriedaCurrency parseTriedaCurrency( Double d )
	{
		Double doubleValue = ( d == null ? 0.0 : d );
		return new TriedaCurrency( doubleValue );
	}

	static public TriedaCurrency parseTriedaCurrency( Object o )
	{
		String s = ( o == null ? "" : o.toString() );
		return TriedaUtil.parseTriedaCurrency( s );
	}

	static public TriedaCurrency parseTriedaCurrency( String s )
	{
		Double d = 0.0;
		try
		{
			d = Double.parseDouble( s );
		}
		catch ( Exception e )
		{
			d = 0.0;
		}

		TriedaCurrency tc = new TriedaCurrency( d );
		return tc;
	}
	
	static public String beginBold(ReportType type) {
		return type.equals(ReportType.WEB) ? "<b>" : "";
	}
	
	static public String endBold(ReportType type) {
		return type.equals(ReportType.WEB) ? "</b>" : "";
	}
	
	static public String newLine(ReportType type) {
		return type.equals(ReportType.WEB) ? "<br/>" : "\n";
	}
	
	static public String extractMessage(Throwable caught) {
		String caughtMessage = "";
		if (caught != null) {
			if (caught instanceof TriedaException) {
				caughtMessage = ((TriedaException)caught).getCompleteMessage();
			} else {
				caughtMessage = "Message: " + caught.getMessage();
				Throwable throwable = caught.getCause();
				while (throwable != null) {
					caughtMessage += " Cause: " + throwable.getMessage();
					throwable = throwable.getCause();
				}
			}
		}
		return caughtMessage;
	}
}
