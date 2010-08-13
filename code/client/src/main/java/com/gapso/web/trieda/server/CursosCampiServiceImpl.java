package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvc.model.CursoCampusModel;
import com.gapso.web.trieda.client.services.CursosCampiService;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class CursosCampiServiceImpl extends RemoteServiceServlet implements CursosCampiService {

	private static final long serialVersionUID = 5250776996542788849L;

	public PagingLoadResult<ModelData> getList() {
		List<ModelData> list = new ArrayList<ModelData>();
		for(int i = 1; i <= 10; i++) {
			list.add(new CursoCampusModel("Turno "+i, "Campus "+i, "Curso "+i, "Matriz Curricular "+i));
		}
		return new BasePagingLoadResult<ModelData>(list);
	}

}
