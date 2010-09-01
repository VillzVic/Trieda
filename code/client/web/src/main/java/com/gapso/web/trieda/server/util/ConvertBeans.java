package com.gapso.web.trieda.server.util;

import java.util.Calendar;

import com.gapso.trieda.domain.Calendario;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.client.mvp.model.CalendarioDTO;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.HorarioAulaDTO;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;

public class ConvertBeans {
	
	// CAMPUS
	public static Campus toCampus(CampusDTO dto) {
		Campus domain = new Campus();
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		domain.setNome(dto.getNome());
		domain.setCodigo(dto.getCodigo());
		return domain;
	}
	
	public static CampusDTO toCampusDTO(Campus domain) {
		CampusDTO dto = new CampusDTO();
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setNome(domain.getNome());
		dto.setCodigo(domain.getCodigo());
		return dto;
	}

	// TURNO
	public static Turno toTurno(TurnoDTO dto) {
		Turno domain = new Turno();
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		domain.setNome(dto.getNome());
		domain.setTempo(dto.getTempo());
		return domain;
	}

	public static TurnoDTO toTurnoDTO(Turno domain) {
		TurnoDTO dto = new TurnoDTO();
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setNome(domain.getNome());
		dto.setTempo(domain.getTempo());
		return dto;
	}

	// CALENDÁRIO
	public static Calendario toCalendario(CalendarioDTO dto) {
		Calendario domain = new Calendario();
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		domain.setCodigo(dto.getCodigo());
		domain.setDescricao(dto.getDescricao());
		return domain;
	}

	public static CalendarioDTO toCalendarioDTO(Calendario domain) {
		CalendarioDTO dto = new CalendarioDTO();
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setCodigo(domain.getCodigo());
		dto.setDescricao(domain.getDescricao());
		return dto;
	}

	// HORARIO DE AULA
	public static HorarioAula toHorarioAula(HorarioAulaDTO dto) {
		HorarioAula domain = new HorarioAula();
		domain.setId(dto.getId());
		domain.setVersion(dto.getVersion());
		Calendario calendario = Calendario.find(dto.getCalendarioId());
		domain.setCalendario(calendario);
		Turno turno = Turno.find(dto.getTurnoId());
		domain.setTurno(turno);
		domain.setHorario(dto.getInicio());
		return domain;
	}

	public static HorarioAulaDTO toHorarioAulaDTO(HorarioAula domain) {
		HorarioAulaDTO dto = new HorarioAulaDTO();
		dto.setId(domain.getId());
		dto.setVersion(domain.getVersion());
		dto.setCalendarioId(domain.getCalendario().getId());
		dto.setCalendarioString(domain.getCalendario().getCodigo());
		Turno turno = domain.getTurno();
		dto.setTurnoId(turno.getId());
		dto.setTurnoString(turno.getNome());

		dto.setInicio(domain.getHorario());

		Calendar fimCal = Calendar.getInstance();
		fimCal.setTime(domain.getHorario());
		fimCal.add(Calendar.MINUTE, turno.getTempo());
		dto.setFim(fimCal.getTime());
		return dto;
	}

}
