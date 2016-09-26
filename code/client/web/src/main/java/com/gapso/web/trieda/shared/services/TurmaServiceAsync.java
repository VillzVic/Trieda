package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.BaseTreeModel;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.TurmaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface TurmaServiceAsync {
	void getListByDisciplina(CenarioDTO cenarioDTO, DisciplinaDTO campusDTO, AsyncCallback< ListLoadResult< TurmaDTO > > callback );
	void getListAllDisciplinaTodos( CenarioDTO cenarioDTO, AsyncCallback< ListLoadResult< TurmaDTO > > callback );
	void getListAll( CenarioDTO cenarioDTO, AsyncCallback< ListLoadResult< TurmaDTO > > callback );
	void getTurma( Long id, AsyncCallback< TurmaDTO > callback );
}
