package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.controller.mvc.model.DemandaModel;
import com.gapso.web.trieda.client.services.DemandasService;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class DemandasServiceImpl extends RemoteServiceServlet implements DemandasService {

	private static final long serialVersionUID = 5250776996542788849L;

	public PagingLoadResult<ModelData> getList() {
		List<ModelData> list = new ArrayList<ModelData>();
		for(int i = 1; i <= 10; i++) {
			list.add(new DemandaModel("Campus "+i, "Turno "+i, "Curso "+i, "Disciplina "+i, 20+i));
		}
		return new BasePagingLoadResult<ModelData>(list);
	}

}
