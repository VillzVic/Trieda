package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvc.model.ProfessorDisciplinaModel;
import com.gapso.web.trieda.client.services.ProfessoresDisciplinasService;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class ProfessoresDisciplinasServiceImpl extends RemoteServiceServlet implements ProfessoresDisciplinasService {

	private static final long serialVersionUID = 5250776996542788849L;

	public PagingLoadResult<ModelData> getList() {
		List<ModelData> list = new ArrayList<ModelData>();
		for(int i = 1; i <= 10; i++) {
			list.add(new ProfessorDisciplinaModel("111.222.333-44", "Nome "+i, "Disciplina "+i, i, (i%2==0)?"Boa":"Ruim"));
		}
		return new BasePagingLoadResult<ModelData>(list);
	}

}
