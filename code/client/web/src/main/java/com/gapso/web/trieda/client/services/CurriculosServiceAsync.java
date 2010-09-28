package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.CurriculoDTO;
import com.gapso.web.trieda.client.mvp.model.CursoDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface CurriculosServiceAsync {

	void getCurriculo(Long id, AsyncCallback<CurriculoDTO> callback);
	void getBuscaList(CursoDTO cursoDTO, String codigo, String descricao, PagingLoadConfig config, AsyncCallback<PagingLoadResult<CurriculoDTO>> callback);
	void save(CurriculoDTO curriculoDTO, AsyncCallback<Void> callback);
	void remove(List<CurriculoDTO> curriculoDTOList, AsyncCallback<Void> callback);
	
}
