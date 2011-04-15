package com.gapso.web.trieda.main.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.AreaTitulacaoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("areasTitulacao")
public interface AreasTitulacaoService extends RemoteService {

	AreaTitulacaoDTO getAreaTitulacao(Long id);
	PagingLoadResult<AreaTitulacaoDTO> getBuscaList(String nome, String descricao, PagingLoadConfig config);
	ListLoadResult<AreaTitulacaoDTO> getListAll();
	void save(AreaTitulacaoDTO areaTitulacaoDTO);
	void remove(List<AreaTitulacaoDTO> areaTitulacaoDTOList);
	List<AreaTitulacaoDTO> getListVinculadas(CursoDTO cursoDTO);
	List<AreaTitulacaoDTO> getListNaoVinculadas(CursoDTO cursoDTO);
	void vincula(CursoDTO cursoDTO, List<AreaTitulacaoDTO> areasTitulacaoDTO);
	void desvincula(CursoDTO cursoDTO, List<AreaTitulacaoDTO> areasTitulacaoDTO);
	
}
