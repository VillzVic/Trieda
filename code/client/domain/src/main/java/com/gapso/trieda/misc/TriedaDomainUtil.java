package com.gapso.trieda.misc;

import java.lang.reflect.InvocationTargetException;
import java.util.Calendar;
import java.util.Date;

import com.gapso.trieda.domain.TriedaPar;

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
	
	static public Calendar dateToCalendar(Date date) {
		Calendar cal = Calendar.getInstance();
		cal.setTime(date);
		cal.set(1979,Calendar.NOVEMBER,6);
		return cal;
	}
	
	static public Calendar getCalendar(int hourOfDay, int minute) {
		Calendar cal = Calendar.getInstance();
		cal.set(1979,Calendar.NOVEMBER,6,hourOfDay,minute,0);
		return cal;
	}
	
	static public boolean estahContido(TriedaPar<Date, Date> intervalo1, TriedaPar<Date, Date> intervalo2) {
        // dado que o que interessa aqui é comparar somente os horários, então, é necessário
        // garantir que todas as datas na comparação estejam dentro de um mesmo dia
		Calendar hi1 = dateToCalendar(intervalo1.getPrimeiro());
		Calendar hf1 = dateToCalendar(intervalo1.getSegundo());
		Calendar hi2 = dateToCalendar(intervalo2.getPrimeiro());
		Calendar hf2 = dateToCalendar(intervalo2.getSegundo());
		
		return (hi2.compareTo(hi1) <= 0) && (hf1.compareTo(hf2) <= 0);
    }
	
	private static final boolean transactionDebugging = true;
	private static final boolean verboseTransactionDebugging = true;
	public static void showTransactionStatus(String message) {
		System.out.println(((transactionActive()) ? "[+] " : "[-] ") + message);
	}
 
	// Some guidance from: http://java.dzone.com/articles/monitoring-declarative-transac?page=0,1
	public static boolean transactionActive() {
		try {
			ClassLoader contextClassLoader = Thread.currentThread().getContextClassLoader();
			Class tsmClass = contextClassLoader.loadClass("org.springframework.transaction.support.TransactionSynchronizationManager");
			Boolean isActive = (Boolean) tsmClass.getMethod("isActualTransactionActive", null).invoke(null, null);
			return isActive;
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		} catch (IllegalArgumentException e) {
			e.printStackTrace();
		} catch (SecurityException e) {
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			e.printStackTrace();
		} catch (InvocationTargetException e) {
			e.printStackTrace();
		} catch (NoSuchMethodException e) {
			e.printStackTrace();
		}
		
		// If we got here it means there was an exception
		throw new IllegalStateException("ServerUtils.transactionActive was unable to complete properly");
	}
	
	public static void transactionRequired(String message) {
		// Are we debugging transactions?
		if (!transactionDebugging) {
			// No, just return
			return;
		}
		// Are we doing verbose transaction debugging?
		if (verboseTransactionDebugging) {
			// Yes, show the status before we get to the possibility of throwing an exception
			showTransactionStatus(message);
		}
		// Is there a transaction active?
		if (!transactionActive()) {
			// No, throw an exception
			throw new IllegalStateException("Transaction required but not active [" + message + "]");
		}
	}
}
