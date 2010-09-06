package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.UnidadeDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("unidades")
public interface UnidadesService extends RemoteService {
	
	PagingLoadResult<UnidadeDTO> getList(PagingLoadConfig config);
	ListLoadResult<UnidadeDTO> getList();
	void save(UnidadeDTO unidadeDTO);
	void remove(List<UnidadeDTO> unidadeDTOList);
	PagingLoadResult<UnidadeDTO> getBuscaList(CampusDTO campusDTO, String nome, String codigo, PagingLoadConfig config);
}
