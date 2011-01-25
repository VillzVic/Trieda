package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.AreaTitulacaoDTO;
import com.gapso.web.trieda.client.mvp.model.CursoDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;


public interface AreasTitulacaoServiceAsync {

	void getAreaTitulacao(Long id, AsyncCallback<AreaTitulacaoDTO> callback);
	void remove(List<AreaTitulacaoDTO> areaTitulacaoDTOList, AsyncCallback<Void> callback);
	void getListAll(AsyncCallback<ListLoadResult<AreaTitulacaoDTO>> callback);
	void save(AreaTitulacaoDTO areaTitulacaoDTO, AsyncCallback<Void> callback);
	void getBuscaList(String nome, String descricao, PagingLoadConfig config, AsyncCallback<PagingLoadResult<AreaTitulacaoDTO>> callback);
	void getListVinculadas(CursoDTO cursoDTO, AsyncCallback<List<AreaTitulacaoDTO>> callback);
	void getListNaoVinculadas(CursoDTO cursoDTO, AsyncCallback<List<AreaTitulacaoDTO>> callback);
}
