package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.client.mvp.model.SemanaLetivaDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("semanasLetiva")
public interface SemanasLetivaService extends RemoteService {
	
	PagingLoadResult<SemanaLetivaDTO> getList(PagingLoadConfig config);
	ListLoadResult<SemanaLetivaDTO> getList();
	ListLoadResult<SemanaLetivaDTO> getList(BasePagingLoadConfig loadConfig);
	PagingLoadResult<SemanaLetivaDTO> getBuscaList(String codigo, String descricao, PagingLoadConfig config);
	void save(SemanaLetivaDTO semanaLetivaDTO);
	void remove(List<SemanaLetivaDTO> semanaLetivaDTOList);
	PagingLoadResult<HorarioDisponivelCenarioDTO> getHorariosDisponiveisCenario(SemanaLetivaDTO semanaLetivaDTO);
	void saveHorariosDisponiveisCenario(SemanaLetivaDTO semanaLetivaDTO, List<HorarioDisponivelCenarioDTO> listDTO);
	
}
