package com.gapso.web.trieda.shared.services;

import java.util.Date;
import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.HorarioAulaDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.TrioDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.util.view.TriedaException;
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
	ParDTO<HorarioDisponivelCenarioDTO, SemanaLetivaDTO> getHorarioDisponivelCenario(Long id);
	TrioDTO<HorarioDisponivelCenarioDTO, String, SemanaLetivaDTO> findHorarioDisponivelCenario(String horarioInicialString, Integer semana, DemandaDTO demandaDTO) throws TriedaException;
	
}
