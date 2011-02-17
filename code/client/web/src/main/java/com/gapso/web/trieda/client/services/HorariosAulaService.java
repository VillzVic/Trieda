package com.gapso.web.trieda.client.services;

import java.util.Date;
import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.SemanaLetivaDTO;
import com.gapso.web.trieda.client.mvp.model.HorarioAulaDTO;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("horariosAula")
public interface HorariosAulaService extends RemoteService {
	
	PagingLoadResult<HorarioAulaDTO> getList(PagingLoadConfig config);
	void save(HorarioAulaDTO horarioAula);
	void remove(List<HorarioAulaDTO> horariosAulaList);
	PagingLoadResult<HorarioAulaDTO> getBuscaList(SemanaLetivaDTO semanaLetivaDTO, TurnoDTO turnoDTO, Date horario, PagingLoadConfig loadConfig);
	void removeWithHorario(HorarioAulaDTO horarioAulaDTO);
	
}
