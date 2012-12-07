package com.gapso.web.trieda.server.util;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.TreeMap;

import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
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
	
	public static List<HorarioDisponivelCenarioDTO> ordenaHorariosPorSemanaLetivaETurno(List<HorarioDisponivelCenario> list) {
		List<HorarioDisponivelCenarioDTO> listDTO = ConvertBeans.toHorarioDisponivelCenarioDTO(list);
		ordenaHorariosDTOPorSemanaLetivaETurno(listDTO);
		return listDTO;
	}
	
	public static void ordenaHorariosDTOPorSemanaLetivaETurno(List<HorarioDisponivelCenarioDTO> listDTO) {
		// [SemanaLetiva -> List<HorarioDisponivelCenarioDTO>]
		Map<String, List<HorarioDisponivelCenarioDTO>> semanaLetivaToHorariosMap = new TreeMap<String, List<HorarioDisponivelCenarioDTO>>();
		for (HorarioDisponivelCenarioDTO o : listDTO) { 
			List<HorarioDisponivelCenarioDTO> horarios = semanaLetivaToHorariosMap.get(o.getSemanaLetivaString());
			if (horarios == null) {
				horarios = new ArrayList<HorarioDisponivelCenarioDTO>();
				semanaLetivaToHorariosMap.put(o.getSemanaLetivaString(), horarios);
			}
			horarios.add(o);
		}
		
		listDTO.clear();
		
		for (Entry<String,List<HorarioDisponivelCenarioDTO>> entrySL : semanaLetivaToHorariosMap.entrySet()) {
			// [Turno -> List<HorarioDisponivelCenarioDTO>]
			Map<String, List<HorarioDisponivelCenarioDTO>> turnoToHorariosMap = new HashMap<String, List<HorarioDisponivelCenarioDTO>>();
			for (HorarioDisponivelCenarioDTO o : entrySL.getValue()) {
				List<HorarioDisponivelCenarioDTO> horarios = turnoToHorariosMap.get(o.getTurnoString());
				if (horarios == null) {
					horarios = new ArrayList<HorarioDisponivelCenarioDTO>();
					turnoToHorariosMap.put(o.getTurnoString(), horarios);
				}
				horarios.add(o);
			}
	
			for (Entry<String, List<HorarioDisponivelCenarioDTO>> entryTurno : turnoToHorariosMap.entrySet()) {
				Collections.sort(entryTurno.getValue());
			}
	
			// [UltimoHorario -> List<"Turno">]
			Map<Date, List<String>> horariosFinalToTurnosMap = new TreeMap<Date, List<String>>();
			for (Entry<String, List<HorarioDisponivelCenarioDTO>> entryTurno : turnoToHorariosMap.entrySet()) {
				Date ultimoHorario = entryTurno.getValue().get(entryTurno.getValue().size() - 1).getHorario();
				// trata o horário para que todas as datas (dia/mês/ano) sejam iguais, pois, somente o horário importa
				Calendar ultimoHorarioCalendar = Calendar.getInstance();
				ultimoHorarioCalendar.setTime(ultimoHorario);
				ultimoHorarioCalendar.set(1979, Calendar.NOVEMBER, 6);
				ultimoHorario = ultimoHorarioCalendar.getTime();
	
				List<String> turnos = horariosFinalToTurnosMap.get(ultimoHorario);
				if (turnos == null) {
					turnos = new ArrayList<String>();
					horariosFinalToTurnosMap.put(ultimoHorario,turnos);
				}
	
				turnos.add(entryTurno.getKey());
			}
	
			for (Entry<Date, List<String>> entry : horariosFinalToTurnosMap.entrySet()) {
				for (String turno : entry.getValue()) {
					listDTO.addAll(turnoToHorariosMap.get(turno));
				}
			}
		}
	}
}
