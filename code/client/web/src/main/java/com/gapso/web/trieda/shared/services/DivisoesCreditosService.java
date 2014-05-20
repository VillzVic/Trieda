package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DivisaoCreditoDTO;
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("divisoesCreditos")
public interface DivisoesCreditosService extends RemoteService {

	DivisaoCreditoDTO getDivisaoCredito(Long id);
	PagingLoadResult<DivisaoCreditoDTO> getList(CenarioDTO cenarioDTO, PagingLoadConfig config);
	void save(DivisaoCreditoDTO divisaoCreditoDTO) throws TriedaException;
	void remove(List<DivisaoCreditoDTO> divisaoCreditoDTOList);
	PagingLoadResult<DivisaoCreditoDTO> getListComDisciplinas(CenarioDTO cenarioDTO, PagingLoadConfig config);
	void saveWithDisciplina(DivisaoCreditoDTO divisaoCreditoDTO);
	void removeWithDisciplina(List<DivisaoCreditoDTO> divisaoCreditoDTOList);

}
