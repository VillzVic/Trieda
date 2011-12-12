package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath( "semanasLetiva" )
public interface SemanasLetivaService
	extends RemoteService
{
	ListLoadResult< SemanaLetivaDTO > getList();
	ListLoadResult< SemanaLetivaDTO > getList(
		BasePagingLoadConfig loadConfig );
	PagingLoadResult<SemanaLetivaDTO> getBuscaList(
		String codigo, String descricao, PagingLoadConfig config );
	void save( SemanaLetivaDTO semanaLetivaDTO );
	void remove( List< SemanaLetivaDTO > semanaLetivaDTOList );
	PagingLoadResult< HorarioDisponivelCenarioDTO > getHorariosDisponiveisCenario(
		SemanaLetivaDTO semanaLetivaDTO );
	PagingLoadResult< HorarioDisponivelCenarioDTO > getAllHorariosDisponiveisCenario();
	void saveHorariosDisponiveisCenario( SemanaLetivaDTO semanaLetivaDTO,
		List< HorarioDisponivelCenarioDTO > listDTO );
	SemanaLetivaDTO findSemanaLetiva( Long id );
}
