package com.gapso.web.trieda.shared.services;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.TurmaDTO;
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
