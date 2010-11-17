package com.gapso.web.trieda.client.services;

import java.util.List;
import java.util.Set;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.CenarioDTO;
import com.gapso.web.trieda.client.mvp.model.SemanaLetivaDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("cenarios")
public interface CenariosService extends RemoteService {

	CenarioDTO getMasterData();
	CenarioDTO getCenario(Long id);
	PagingLoadResult<CenarioDTO> getList(PagingLoadConfig config);
	PagingLoadResult<CenarioDTO> getBuscaList(Integer ano, Integer semestre, PagingLoadConfig config);
	void editar(CenarioDTO cenarioDTO);
	void criar(CenarioDTO cenarioDTO, SemanaLetivaDTO semanaLetivaDTO, Set<CampusDTO> campiDTO);
	void clonar(CenarioDTO cenarioDTO);
	void remove(List<CenarioDTO> cenarioDTOList);

}
