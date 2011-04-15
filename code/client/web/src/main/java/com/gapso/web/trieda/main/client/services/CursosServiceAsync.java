package com.gapso.web.trieda.main.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.TipoCursoDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;


public interface CursosServiceAsync {

	void getCurso(Long id, AsyncCallback<CursoDTO> callback);
	void getList(BasePagingLoadConfig loadConfig, AsyncCallback<ListLoadResult<CursoDTO>> callback);
	void getBuscaList(String nome, String codigo, TipoCursoDTO tipoCurso, PagingLoadConfig config, AsyncCallback<PagingLoadResult<CursoDTO>> callback);
	void save(CursoDTO cursoDTO, AsyncCallback<Void> callback);
	void remove(List<CursoDTO> cursoDTOList, AsyncCallback<Void> callback);
	void getListAll(AsyncCallback<ListLoadResult<CursoDTO>> callback);
	void getListByCampus(CampusDTO campusDTO, List<CursoDTO> retirarCursosDTO, AsyncCallback<ListLoadResult<CursoDTO>> callback);
	
}
