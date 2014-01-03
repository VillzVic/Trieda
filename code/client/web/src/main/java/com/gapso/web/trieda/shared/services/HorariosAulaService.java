package com.gapso.web.trieda.shared.services;

import java.util.Date;
import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.HorarioAulaDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("horariosAula")
public interface HorariosAulaService extends RemoteService {
	
	void save(CenarioDTO cenarioDTO, HorarioAulaDTO horarioAula);
	void remove(List<HorarioAulaDTO> horariosAulaList);
	PagingLoadResult<HorarioAulaDTO> getBuscaList(SemanaLetivaDTO semanaLetivaDTO, TurnoDTO turnoDTO, Date horario, PagingLoadConfig loadConfig);
	void removeWithHorario(HorarioAulaDTO horarioAulaDTO);
	
}
