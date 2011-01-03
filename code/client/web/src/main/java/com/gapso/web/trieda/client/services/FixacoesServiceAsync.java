package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.FixacaoDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;


public interface FixacoesServiceAsync {

	void getFixacao(Long id, AsyncCallback<FixacaoDTO> callback);
	void getBuscaList(String codigo, PagingLoadConfig config, AsyncCallback<PagingLoadResult<FixacaoDTO>> callback);
	void save(FixacaoDTO fixacaoDTO, AsyncCallback<Void> callback);
	void remove(List<FixacaoDTO> fixacaoDTOList, AsyncCallback<Void> callback);
	
}
