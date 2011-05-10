package com.gapso.web.trieda.shared.services;

import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("uniques")
public interface UniquesService extends RemoteService {

	Boolean existUnique(CenarioDTO cenarioDTO, String value, String uniqueDomain);
	
}
