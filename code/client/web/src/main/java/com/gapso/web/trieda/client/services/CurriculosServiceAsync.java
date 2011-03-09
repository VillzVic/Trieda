package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.CurriculoDTO;
import com.gapso.web.trieda.client.mvp.model.CursoDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDisciplinaDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface CurriculosServiceAsync {

	void getCurriculo(Long id, AsyncCallback<CurriculoDTO> callback);
	void getList(BasePagingLoadConfig loadConfig, AsyncCallback<ListLoadResult<CurriculoDTO>> callback);
	void getBuscaList(CursoDTO cursoDTO, String codigo, String descricao, PagingLoadConfig config, AsyncCallback<PagingLoadResult<CurriculoDTO>> callback);
	void save(CurriculoDTO curriculoDTO, AsyncCallback<Void> callback);
	void remove(List<CurriculoDTO> curriculoDTOList, AsyncCallback<Void> callback);
	void getDisciplinasList(CurriculoDTO curriculoDTO, AsyncCallback<ListLoadResult<CurriculoDisciplinaDTO>> callback);
	void saveDisciplina(CurriculoDTO curriculoDTO, CurriculoDisciplinaDTO curriculoDisciplinaDTO, AsyncCallback<Void> callback);
	void removeDisciplina(List<CurriculoDisciplinaDTO> curriculoDisciplinaDTOList, AsyncCallback<Void> callback);
	void getPeriodos(CurriculoDTO curriculoDTO, AsyncCallback<List<Integer>> callback);
	void getListByCurso(CursoDTO cursoDTO, AsyncCallback<ListLoadResult<CurriculoDTO>> callback);
	void getListAll(AsyncCallback<ListLoadResult<CurriculoDTO>> callback);
	
}
