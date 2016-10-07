package com.gapso.web.trieda.shared.services;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.TurmaDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface TurmaServiceAsync {
	void getListByDisciplina(CenarioDTO cenarioDTO, DisciplinaDTO campusDTO, AsyncCallback< ListLoadResult< TurmaDTO > > callback );
	void getListAllDisciplinaTodos( CenarioDTO cenarioDTO, AsyncCallback< ListLoadResult< TurmaDTO > > callback );
	void getListAll( CenarioDTO cenarioDTO, AsyncCallback< ListLoadResult< TurmaDTO > > callback );
	void getTurma( Long id, AsyncCallback< TurmaDTO > callback );
}
