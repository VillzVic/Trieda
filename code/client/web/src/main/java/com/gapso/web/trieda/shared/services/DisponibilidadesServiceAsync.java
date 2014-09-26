package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisponibilidadeDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface DisponibilidadesServiceAsync {

	void getDisponibilidades(CenarioDTO cenarioDTO, Long entidadeId,
			String tipoEntidade,
			AsyncCallback<PagingLoadResult<DisponibilidadeDTO>> callback);

	void saveDisponibilidadesDias(CenarioDTO cenarioDTO,
			List<DisponibilidadeDTO> disponibilidadesDTO, String tipoEntidade,
			AsyncCallback<Void> callback);

	void saveDisponibilidade(CenarioDTO cenarioDTO,
			DisponibilidadeDTO disponibilidadeDTO, String tipoEntidade,
			AsyncCallback<Void> callback);

	void remove(DisponibilidadeDTO disponibilidadeDTO, String tipoEntidade,
			AsyncCallback<Void> callback);

}
