package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.AtendimentoTaticoDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface AtendimentosServiceAsync {

	void getList(AsyncCallback<PagingLoadResult<AtendimentoTaticoDTO>> callback);
	void getBusca(SalaDTO sala, TurnoDTO turno, AsyncCallback<List<AtendimentoTaticoDTO>> callback);
	void getBusca(CurriculoDTO curriculoDTO, Integer periodo, TurnoDTO turnoDTO, AsyncCallback<ParDTO<List<AtendimentoTaticoDTO>, List<Integer>>> callback);
	
}
