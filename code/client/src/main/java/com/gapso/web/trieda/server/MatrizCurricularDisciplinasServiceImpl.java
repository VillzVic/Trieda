package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvc.model.MatrizCurricularDisciplinaModel;
import com.gapso.web.trieda.client.services.MatrizCurricularDisciplinasService;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class MatrizCurricularDisciplinasServiceImpl extends RemoteServiceServlet implements MatrizCurricularDisciplinasService {

	private static final long serialVersionUID = 5250776996542788849L;

	public PagingLoadResult<ModelData> getList() {
		List<ModelData> list = new ArrayList<ModelData>();
		for(int i = 1; i <= 10; i++) {
			list.add(new MatrizCurricularDisciplinaModel("Matriz Curricular "+i, "Curso "+i, (i%3)+1,  "Disciplina "+i, i, i+1, i+i+1));
		}
		return new BasePagingLoadResult<ModelData>(list);
	}

}
