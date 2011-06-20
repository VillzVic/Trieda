package com.gapso.web.solverws.util;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.concurrent.atomic.AtomicLong;

public class UniqueId {

	private static AtomicLong idCounter = new AtomicLong();
	public static Long createID() {
		DateFormat df = new SimpleDateFormat("yyyyMMddHHmmss");
		String uniqueIDString = df.format(new java.util.Date());

		Long uniqueID = idCounter.getAndIncrement();
		return Long.valueOf(uniqueIDString + uniqueID);
	}
	
}
