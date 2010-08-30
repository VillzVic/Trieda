package com.gapso.web.trieda.server.util;

import com.gapso.trieda.domain.Calendario;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.client.mvp.model.CalendarioDTO;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;

public class ConvertBeans {

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

}
