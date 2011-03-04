package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.CursoDTO;
import com.gapso.web.trieda.shared.dtos.TipoCursoDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("cursos")
public interface CursosService extends RemoteService {

	CursoDTO getCurso(Long id);
	ListLoadResult<CursoDTO> getList(BasePagingLoadConfig loadConfig);
	PagingLoadResult<CursoDTO> getBuscaList(String nome, String codigo, TipoCursoDTO tipoCurso, PagingLoadConfig config);
	void save(CursoDTO cursoDTO);
	void remove(List<CursoDTO> cursoDTOList);
	ListLoadResult<CursoDTO> getListByCampus(CampusDTO campusDTO);
	ListLoadResult<CursoDTO> getListAll();
	
}
