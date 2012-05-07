package com.gapso.web.trieda.server.util;

import java.util.Calendar;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.List;

import com.google.gwt.dev.util.Pair;

public class TriedaServerUtil{
	
	@SuppressWarnings("deprecation")
	public static Calendar dateToCalendar(Date date) {
		Calendar horario = Calendar.getInstance();
		
		horario.clear();
		horario.set(2012,Calendar.FEBRUARY,1,date.getHours(),date.getMinutes(),0);
		horario.set(Calendar.MILLISECOND,0);
		
		return horario;
	}
	
	public static void ordenaParesDeHorarios(List<Pair<Calendar, Calendar>> pares){
		Collections.sort(pares, new Comparator<Pair<Calendar, Calendar>>() {
			@Override
			public int compare(Pair<Calendar, Calendar> o1, Pair<Calendar, Calendar> o2) {
				Calendar horarioInicial1 = o1.getLeft();
				Calendar horarioInicial2 = o2.getLeft();
				int ret = horarioInicial1.compareTo(horarioInicial2);
				if(ret == 0){
					Calendar horarioFinal1 = o1.getRight();
					Calendar horarioFinal2 = o2.getRight();
					return horarioFinal1.compareTo(horarioFinal2);
				}
				return ret;
			}
		});
	}
}
