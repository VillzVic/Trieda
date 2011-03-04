package com.gapso.web.trieda.client.services;

import java.util.List;
import java.util.Map;

import com.gapso.web.trieda.client.mvp.model.CenarioDTO;
import com.gapso.web.trieda.client.mvp.model.ParametroDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;


public interface OtimizarServiceAsync {

	void input(ParametroDTO parametroDTO, AsyncCallback<Long> callback);
	void isOptimizing(Long round, AsyncCallback<Boolean> callback);
	void saveContent(CenarioDTO cenarioDTO, Long round, AsyncCallback<Map<String, List<String>>> callback);
	void getParametro(CenarioDTO cenarioDTO, AsyncCallback<ParametroDTO> callback);
	
}
