package com.gapso.web.trieda.client.services;

import com.gapso.web.trieda.client.mvp.model.CenarioDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;


public interface OtimizarServiceAsync {

	void input(CenarioDTO cenarioDTO, AsyncCallback<Long> callback);
	void isOptimizing(Long round, AsyncCallback<Boolean> callback);
	void saveContent(CenarioDTO cenarioDTO, Long round, AsyncCallback<Boolean> callback);
	
}
