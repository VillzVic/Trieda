package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.CenarioDTO;
import com.gapso.web.trieda.client.mvp.model.DivisaoCreditoDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("divisoesCreditos")
public interface DivisoesCreditosService extends RemoteService {

	DivisaoCreditoDTO getDivisaoCredito(Long id);
	PagingLoadResult<DivisaoCreditoDTO> getList(CenarioDTO cenarioDTO, PagingLoadConfig config);
	void save(DivisaoCreditoDTO divisaoCreditoDTO);
	void remove(List<DivisaoCreditoDTO> divisaoCreditoDTOList);

}
