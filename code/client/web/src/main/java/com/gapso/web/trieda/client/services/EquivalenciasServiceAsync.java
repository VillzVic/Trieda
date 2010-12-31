package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.DisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.EquivalenciaDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;


public interface EquivalenciasServiceAsync {

	void getEquivalencia(Long id, AsyncCallback<EquivalenciaDTO> callback);
	void getBuscaList(DisciplinaDTO disciplinaDTO, PagingLoadConfig config, AsyncCallback<PagingLoadResult<EquivalenciaDTO>> callback);
	void save(EquivalenciaDTO equivalenciaDTO, List<DisciplinaDTO> eliminaList, AsyncCallback<Void> callback);
	void remove(List<EquivalenciaDTO> equivalenciaDTOList, AsyncCallback<Void> callback);
	
}
