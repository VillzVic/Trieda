package com.gapso.web.trieda.shared.services;

import java.util.Date;
import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.HorarioAulaDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface HorariosAulaServiceAsync {
	
	void save(CenarioDTO cenarioDTO, HorarioAulaDTO horarioAulaDTO, AsyncCallback<Void> callback);
	void remove(List<HorarioAulaDTO> horariosAulaDTOList, AsyncCallback<Void> callback);
	void getBuscaList(SemanaLetivaDTO semanaLetivaDTO, TurnoDTO turnoDTO, Date horario, PagingLoadConfig loadConfig, AsyncCallback<PagingLoadResult<HorarioAulaDTO>> callback);
	void removeWithHorario(HorarioAulaDTO horarioAulaDTO, AsyncCallback<Void> callback);
	
}
