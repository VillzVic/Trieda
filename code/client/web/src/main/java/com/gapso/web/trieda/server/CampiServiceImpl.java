package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Endereco;
import com.gapso.web.trieda.client.mvc.model.CampusModel;
import com.gapso.web.trieda.client.services.CampiService;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class CampiServiceImpl extends RemoteServiceServlet implements CampiService {

	private static final long serialVersionUID = 5250776996542788849L;

	public PagingLoadResult<ModelData> getList() {
		
		List<ModelData> list = new ArrayList<ModelData>();
		
		for(Endereco c : Endereco.findAllEnderecoes()) {
			list.add(new CampusModel(c.getCidade(), c.getLogradouro()));
		}
		return new BasePagingLoadResult<ModelData>(list);
	}

}
