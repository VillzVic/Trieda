package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvc.model.UnidadeModel;
import com.gapso.web.trieda.client.services.UnidadesService;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class UnidadesServiceImpl extends RemoteServiceServlet implements UnidadesService {

	private static final long serialVersionUID = 5250776996542788849L;

	public PagingLoadResult<ModelData> getList() {
		List<ModelData> list = new ArrayList<ModelData>();
		for(int i = 1; i <= 10; i++) {
			list.add(new UnidadeModel("Codigo "+i, "Nome "+i, "Campus "+i, i));
		}
		return new BasePagingLoadResult<ModelData>(list);
	}

}
