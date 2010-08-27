package com.gapso.web.trieda.server.util;

import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;

public class ConvertBeans {

	public static Turno toTurno(TurnoDTO turnoDTO) {
		Turno turno = new Turno();
		turno.setId(turnoDTO.getId());
		turno.setVersion(turnoDTO.getVersion());
		turno.setNome(turnoDTO.getNome());
		turno.setTempo(turnoDTO.getTempo());
		return turno;
	}

	public static TurnoDTO toTurnoDTO(Turno turno) {
		TurnoDTO turnoDTO = new TurnoDTO();
		turnoDTO.setId(turno.getId());
		turnoDTO.setVersion(turno.getVersion());
		turnoDTO.setNome(turno.getNome());
		turnoDTO.setTempo(turno.getTempo());
		return turnoDTO;
	}

}
