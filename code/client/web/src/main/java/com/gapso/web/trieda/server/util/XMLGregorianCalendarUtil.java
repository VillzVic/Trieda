package com.gapso.web.trieda.server.util;

import java.util.Calendar;
import java.util.Date;

import com.sun.org.apache.xerces.internal.jaxp.datatype.XMLGregorianCalendarImpl;

@SuppressWarnings("restriction")
public class XMLGregorianCalendarUtil
	extends XMLGregorianCalendarImpl
{
	private static final long serialVersionUID = -4867849278241181172L;

	public XMLGregorianCalendarUtil( Date date )
	{
		Calendar calendar = Calendar.getInstance();

		calendar.setTime( date );
		setYear( calendar.get( Calendar.YEAR ) );
		setMonth( calendar.get( Calendar.MONTH ) + 1 );
		setDay( calendar.get( Calendar.DAY_OF_MONTH ) );
		setHour( calendar.get( Calendar.HOUR_OF_DAY ) );
		setMinute( calendar.get( Calendar.MINUTE ) );
		setSecond( calendar.get( Calendar.SECOND ) );
	}

	@Override
	public String toXMLFormat()
	{
		return ( getHour() + ":" + getMinute() + ":00" );
	}
}
