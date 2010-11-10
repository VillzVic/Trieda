package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.CenarioDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface CenariosServiceAsync {

	void getMasterData(AsyncCallback<CenarioDTO> callback);
	void getCenario(Long id, AsyncCallback<CenarioDTO> callback);
	void getList(PagingLoadConfig config, AsyncCallback<PagingLoadResult<CenarioDTO>> callback);
	void getBuscaList(Integer ano, Integer semestre, PagingLoadConfig config, AsyncCallback<PagingLoadResult<CenarioDTO>> callback);
	void save(CenarioDTO cenarioDTO, AsyncCallback<Void> callback);
	void remove(List<CenarioDTO> cenarioDTOList, AsyncCallback<Void> callback);
	
}
