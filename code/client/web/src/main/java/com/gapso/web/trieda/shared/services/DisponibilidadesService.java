package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisponibilidadeDTO;
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("disponibilidades")
public interface DisponibilidadesService extends RemoteService
{

	PagingLoadResult<DisponibilidadeDTO> getDisponibilidades(
			CenarioDTO cenarioDTO, Long entidadeId, String tipoEntidade);

	void saveDisponibilidadesDias(CenarioDTO cenarioDTO,
			List<DisponibilidadeDTO> disponibilidadesDTO, String tipoEntidade);

	void saveDisponibilidade(CenarioDTO cenarioDTO,
			DisponibilidadeDTO disponibilidadeDTO, String tipoEntidade) throws TriedaException;

	void remove(DisponibilidadeDTO disponibilidadeDTO, String tipoEntidade);

}
