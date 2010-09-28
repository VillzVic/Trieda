package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.CurriculoDTO;
import com.gapso.web.trieda.client.mvp.model.CursoDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("curriculos")
public interface CurriculosService extends RemoteService {
	
	CurriculoDTO getCurriculo(Long id);
	PagingLoadResult<CurriculoDTO> getBuscaList(CursoDTO cursoDTO, String codigo, String descricao, PagingLoadConfig config);
	void save(CurriculoDTO curriculoDTO);
	void remove(List<CurriculoDTO> curriculoDTOList);
	
}
