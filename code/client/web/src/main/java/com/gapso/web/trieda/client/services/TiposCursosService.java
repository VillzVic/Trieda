package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.TipoCursoDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("tiposCursos")
public interface TiposCursosService extends RemoteService {

	PagingLoadResult<TipoCursoDTO> getBuscaList(String nome, String descricao, PagingLoadConfig config);
	void save(TipoCursoDTO tipoCursoDTO);
	void remove(List<TipoCursoDTO> tipoCursoDTOList);
	ListLoadResult<TipoCursoDTO> getList(BasePagingLoadConfig loadConfig);
	TipoCursoDTO getTipoCurso(Long id);

}
