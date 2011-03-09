package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.AtendimentoTaticoDTO;
import com.gapso.web.trieda.client.mvp.model.CurriculoDTO;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("atendimentos")
public interface AtendimentosService extends RemoteService {
	
	PagingLoadResult<AtendimentoTaticoDTO> getList();
	List<AtendimentoTaticoDTO> getBusca(SalaDTO sala, TurnoDTO turno);
	ParDTO<List<AtendimentoTaticoDTO>, List<Integer>> getBusca(CurriculoDTO curriculoDTO, Integer periodo, TurnoDTO turnoDTO);
	
}
