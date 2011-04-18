package com.gapso.web.trieda.shared.services;

import java.util.List;
import java.util.Set;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.TreeNodeDTO;
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
	List<TreeNodeDTO> getResumos(CenarioDTO cenario);

}