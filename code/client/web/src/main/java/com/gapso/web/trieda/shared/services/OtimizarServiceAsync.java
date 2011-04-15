package com.gapso.web.trieda.shared.services;

import java.util.List;
import java.util.Map;

import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.ParametroDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;


public interface OtimizarServiceAsync {

	void input(ParametroDTO parametroDTO, List<CampusDTO> campi, AsyncCallback<Long> callback);
	void isOptimizing(Long round, AsyncCallback<Boolean> callback);
	void saveContent(CenarioDTO cenarioDTO, Long round, AsyncCallback<Map<String, List<String>>> callback);
	void getParametro(CenarioDTO cenarioDTO, AsyncCallback<ParametroDTO> callback);
	
}
