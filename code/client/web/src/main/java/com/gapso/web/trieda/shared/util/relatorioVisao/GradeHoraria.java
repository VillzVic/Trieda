package com.gapso.web.trieda.shared.util.relatorioVisao;

import java.util.ArrayList;
import java.util.List;

public class GradeHoraria {
	static final double PIXELS_POR_MINUTO = 1.2;//2.2;
	
	static public List<String> processaLabelsDasLinhasDaGradeHoraria(List<String> horariosDaGradeHoraria, List<String> horariosDeInicioDeAula, List<String> horariosDeFimDeAula) {
		List<String> labelsDasLinhasDaGradeHorariaProcessadas = new ArrayList<String>();
		
		for (int i = 0; i < horariosDaGradeHoraria.size()-1; i++) {
			String hi = horariosDaGradeHoraria.get(i);
			boolean ehHorarioDeFimDeAula = horariosDeFimDeAula.contains(hi);
			boolean ehHorarioDeInicioDeAula = horariosDeInicioDeAula.contains(hi);
			if (!ehHorarioDeFimDeAula || ehHorarioDeInicioDeAula) {
				String hf = horariosDaGradeHoraria.get(i+1); 
				labelsDasLinhasDaGradeHorariaProcessadas.add(hi + " / " + hf);
			}
		}
		
		return labelsDasLinhasDaGradeHorariaProcessadas;
	}
}
