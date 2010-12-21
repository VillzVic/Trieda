package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.CursoDTO;
import com.gapso.web.trieda.client.mvp.model.TipoCursoDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;


public interface CursosServiceAsync {

	void getCurso(Long id, AsyncCallback<CursoDTO> callback);
	void getList(BasePagingLoadConfig loadConfig, AsyncCallback<ListLoadResult<CursoDTO>> callback);
	void getBuscaList(String nome, String codigo, TipoCursoDTO tipoCurso, PagingLoadConfig config, AsyncCallback<PagingLoadResult<CursoDTO>> callback);
	void save(CursoDTO cursoDTO, AsyncCallback<Void> callback);
	void remove(List<CursoDTO> cursoDTOList, AsyncCallback<Void> callback);
	void getListByCampus(CampusDTO campusDTO, AsyncCallback<ListLoadResult<CursoDTO>> callback);
	void getListAll(AsyncCallback<ListLoadResult<CursoDTO>> callback);
	
}
