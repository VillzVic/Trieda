package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.AreaTitulacaoDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("areasTitulacao")
public interface AreasTitulacaoService extends RemoteService {

	public PagingLoadResult<ModelData> getList();
	
	PagingLoadResult<AreaTitulacaoDTO> getBuscaList(String nome, String descricao, PagingLoadConfig config);
	void save(AreaTitulacaoDTO areaTitulacaoDTO);
	void remove(List<AreaTitulacaoDTO> areaTitulacaoDTOList);
	
}
