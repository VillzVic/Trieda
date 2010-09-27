package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.DisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.TipoDisciplinaDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("disciplinas")
public interface DisciplinasService extends RemoteService {

	PagingLoadResult<DisciplinaDTO> getBuscaList(String nome, String codigo, TipoDisciplinaDTO tipoDisciplinaDTO, PagingLoadConfig config);
	void save(DisciplinaDTO disciplinaDTO);
	void remove(List<DisciplinaDTO> disciplinaDTOList);
	ListLoadResult<TipoDisciplinaDTO> getTipoDisciplinaList();
	TipoDisciplinaDTO getTipoDisciplina(Long id);
	
}
