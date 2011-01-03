package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.FixacaoDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("fixacoes")
public interface FixacoesService extends RemoteService {

	FixacaoDTO getFixacao(Long id);
	PagingLoadResult<FixacaoDTO> getBuscaList(String codigo, PagingLoadConfig config);
	void save(FixacaoDTO fixacaoDTO);
	void remove(List<FixacaoDTO> fixacaoDTOList);
	
}
