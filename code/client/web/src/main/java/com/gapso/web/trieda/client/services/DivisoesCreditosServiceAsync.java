package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.CenarioDTO;
import com.gapso.web.trieda.client.mvp.model.DivisaoCreditoDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface DivisoesCreditosServiceAsync {

	void getDivisaoCredito(Long id, AsyncCallback<DivisaoCreditoDTO> callback);
	void getList(CenarioDTO cenarioDTO, PagingLoadConfig config, AsyncCallback<PagingLoadResult<DivisaoCreditoDTO>> callback);
	void save(DivisaoCreditoDTO divisaoCreditoDTO, AsyncCallback<Void> callback);
	void remove(List<DivisaoCreditoDTO> divisaoCreditoDTOList, AsyncCallback<Void> callback);
	
}
