package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.EquivalenciaDTO;
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("equivalencias")
public interface EquivalenciasService extends RemoteService {

	EquivalenciaDTO getEquivalencia(Long id);
	void save(EquivalenciaDTO equivalenciaDTO, List<CursoDTO> cursosSelecionados) throws TriedaException;
	void remove(List<EquivalenciaDTO> equivalenciaDTOList);
	PagingLoadResult<EquivalenciaDTO> getBuscaList(CenarioDTO cenarioDTO, DisciplinaDTO disciplinaDTO,
			CursoDTO cursoDTO, PagingLoadConfig config);
	List<CursoDTO> getCursosEquivalencia(EquivalenciaDTO equivalenciaDTO);
	
}
