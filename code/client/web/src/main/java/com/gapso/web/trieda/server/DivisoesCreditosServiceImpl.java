package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvc.model.DivisaoCreditosModel;
import com.gapso.web.trieda.client.services.DivisoesCreditosService;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class DivisoesCreditosServiceImpl extends RemoteServiceServlet implements DivisoesCreditosService {

	private static final long serialVersionUID = 5250776996542788849L;

	public PagingLoadResult<ModelData> getList() {
		List<ModelData> list = new ArrayList<ModelData>();
		list.add(new DivisaoCreditosModel(1, 1, 0, 0, 0, 0, 0, 0));
		list.add(new DivisaoCreditosModel(2, 2, 0, 0, 0, 0, 0, 0));
		list.add(new DivisaoCreditosModel(3, 3, 0, 0, 0, 0, 0, 0));
		list.add(new DivisaoCreditosModel(4, 2, 2, 0, 0, 0, 0, 0));
		list.add(new DivisaoCreditosModel(5, 3, 2, 0, 0, 0, 0, 0));
		list.add(new DivisaoCreditosModel(6, 3, 3, 0, 0, 0, 0, 0));
		list.add(new DivisaoCreditosModel(7, 4, 0, 3, 0, 0, 0, 0));
		list.add(new DivisaoCreditosModel(8, 4, 0, 2, 0, 2, 0, 0));
		list.add(new DivisaoCreditosModel(9, 3, 0, 3, 3, 0, 0, 0));
		list.add(new DivisaoCreditosModel(10,4, 0, 2, 0, 0, 2, 2));
		return new BasePagingLoadResult<ModelData>(list);
	}

}
