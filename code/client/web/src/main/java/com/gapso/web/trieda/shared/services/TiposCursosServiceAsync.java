package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.TipoCursoDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;


public interface TiposCursosServiceAsync {

	void getBuscaList(CenarioDTO cenarioDTO, String nome, String descricao, PagingLoadConfig config,
			AsyncCallback<PagingLoadResult<TipoCursoDTO>> callback);
	void save(TipoCursoDTO tipoCursoDTO, AsyncCallback<Void> callback);
	void remove(List<TipoCursoDTO> tipoCursoDTOList, AsyncCallback<Void> callback);
	void getList( CenarioDTO cenarioDTO, BasePagingLoadConfig loadConfig, AsyncCallback<ListLoadResult<TipoCursoDTO>> callback);
	void getTipoCurso(Long id, AsyncCallback<TipoCursoDTO> callback);
	void getList(CenarioDTO cenarioDTO, AsyncCallback<ListLoadResult<TipoCursoDTO>> callback);

}
