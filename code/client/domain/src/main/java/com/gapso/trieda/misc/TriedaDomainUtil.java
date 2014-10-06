package com.gapso.trieda.misc;

import java.util.Date;

public class TriedaDomainUtil {
	@SuppressWarnings( "deprecation" )
	static public String shortTimeString(Date date) {
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
}
