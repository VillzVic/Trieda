package com.gapso.web.trieda.client.services;

import java.util.Date;
import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.CalendarioDTO;
import com.gapso.web.trieda.client.mvp.model.HorarioAulaDTO;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface HorariosAulaServiceAsync {
	
	void getList(PagingLoadConfig config, AsyncCallback<PagingLoadResult<HorarioAulaDTO>> callback);
	void save(HorarioAulaDTO horarioAulaDTO, AsyncCallback<Void> callback);
	void remove(List<HorarioAulaDTO> horariosAulaDTOList, AsyncCallback<Void> callback);
	void getBuscaList(CalendarioDTO calendarioDTO, TurnoDTO turnoDTO, Date horario, PagingLoadConfig loadConfig, AsyncCallback<PagingLoadResult<HorarioAulaDTO>> callback);
	
}
