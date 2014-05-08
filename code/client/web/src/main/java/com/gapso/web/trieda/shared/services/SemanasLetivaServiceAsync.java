package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface SemanasLetivaServiceAsync
{
	void getList( CenarioDTO cenarioDTO, AsyncCallback< ListLoadResult< SemanaLetivaDTO > > callback );

	void getList( CenarioDTO cenarioDTO, BasePagingLoadConfig loadConfig,
		AsyncCallback< ListLoadResult< SemanaLetivaDTO > > callback );

	void getBuscaList( CenarioDTO cenarioDTO, String codigo, String descricao, String operadorTempo, Integer tempo, Boolean permite,  PagingLoadConfig config,
		AsyncCallback< PagingLoadResult< SemanaLetivaDTO > > callback );

	void save( SemanaLetivaDTO semanaLetivaDTO, AsyncCallback< Void > callback );

	void remove( List< SemanaLetivaDTO > semanaLetivaDTOList, AsyncCallback< Void > callback );

	void getHorariosDisponiveisCenario( SemanaLetivaDTO semanaLetivaDTO,
		AsyncCallback< PagingLoadResult< HorarioDisponivelCenarioDTO > > callback );

	void getAllHorariosDisponiveisCenario( CenarioDTO cenarioDTO, AsyncCallback< PagingLoadResult< HorarioDisponivelCenarioDTO > > callback );

	void removeHorariosDisponiveisCenario(SemanaLetivaDTO semanaLetivaDTO,
			List<HorarioDisponivelCenarioDTO> listDTO, AsyncCallback<Void> callback);
	
	void saveHorariosDisponiveisCenario( CenarioDTO cenarioDTO, SemanaLetivaDTO semanaLetivaDTO,
		List< HorarioDisponivelCenarioDTO > listDTO, AsyncCallback< Void > callback );

	void findSemanaLetiva( Long id, AsyncCallback< SemanaLetivaDTO > callback );
}
