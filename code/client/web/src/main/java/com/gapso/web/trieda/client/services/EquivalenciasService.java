package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.DisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.EquivalenciaDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("equivalencias")
public interface EquivalenciasService extends RemoteService {

	EquivalenciaDTO getEquivalencia(Long id);
	PagingLoadResult<EquivalenciaDTO> getBuscaList(DisciplinaDTO disciplinaDTO, PagingLoadConfig config);
	void save(EquivalenciaDTO equivalenciaDTO, List<DisciplinaDTO> eliminaList);
	void remove(List<EquivalenciaDTO> equivalenciaDTOList);
	
}
