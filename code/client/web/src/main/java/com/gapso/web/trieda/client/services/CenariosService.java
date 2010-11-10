package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.CenarioDTO;
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
	void save(CenarioDTO cenarioDTO);
	void remove(List<CenarioDTO> cenarioDTOList);

}
