package com.gapso.web.trieda.shared.services;

import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface UniquesServiceAsync
{
	void existUnique( CenarioDTO cenarioDTO, String value,
		String uniqueDomain, AsyncCallback< Boolean > callback );
}
