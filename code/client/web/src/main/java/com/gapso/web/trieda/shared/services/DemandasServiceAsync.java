package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface DemandasServiceAsync
{
	void getBuscaList( CenarioDTO cenarioDTO,CampusDTO campusDTO, CursoDTO cursoDTO,
		CurriculoDTO curriculoDTO, TurnoDTO turnoDTO, DisciplinaDTO disciplinaDTO,
		PagingLoadConfig config, AsyncCallback< PagingLoadResult< DemandaDTO > > callback );
	void save( DemandaDTO demandaDTO, AsyncCallback< Void > callback );
	void remove( List< DemandaDTO > demandaDTOList, AsyncCallback< Void > callback );
	void findPeriodo(DemandaDTO demandaDTO, AsyncCallback< Integer > callback );
}
