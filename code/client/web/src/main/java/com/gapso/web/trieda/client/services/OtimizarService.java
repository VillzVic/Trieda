package com.gapso.web.trieda.client.services;

import java.util.List;
import java.util.Map;

import com.gapso.web.trieda.client.mvp.model.CenarioDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("otimizar")
public interface OtimizarService extends RemoteService {

	Long input(CenarioDTO cenarioDTO);
	Boolean isOptimizing(Long round);
	Map<String, List<String>> saveContent(CenarioDTO cenarioDTO, Long round);

}
