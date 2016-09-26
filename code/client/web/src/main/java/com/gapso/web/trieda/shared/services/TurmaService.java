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
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath( "turmas" )
public interface TurmaService
	extends RemoteService
{
	ListLoadResult< TurmaDTO > getListByDisciplina(CenarioDTO cenarioDTO, DisciplinaDTO disciplinaDTO );
	ListLoadResult< TurmaDTO > getListAllDisciplinaTodos( CenarioDTO cenarioDTO );
	ListLoadResult< TurmaDTO > getListAll( CenarioDTO cenarioDTO );
	TurmaDTO getTurma(Long id);
}
