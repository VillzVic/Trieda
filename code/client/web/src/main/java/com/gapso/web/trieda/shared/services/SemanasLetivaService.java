package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath( "semanasLetiva" )
public interface SemanasLetivaService
	extends RemoteService
{
	ListLoadResult< SemanaLetivaDTO > getList( CenarioDTO cenarioDTO );
	ListLoadResult< SemanaLetivaDTO > getList( CenarioDTO cenarioDTO, 
		BasePagingLoadConfig loadConfig );
	PagingLoadResult<SemanaLetivaDTO> getBuscaList( CenarioDTO cenarioDTO,
		String codigo, String descricao, PagingLoadConfig config );
	void save( SemanaLetivaDTO semanaLetivaDTO );
	void remove( List< SemanaLetivaDTO > semanaLetivaDTOList );
	PagingLoadResult< HorarioDisponivelCenarioDTO > getHorariosDisponiveisCenario(
		SemanaLetivaDTO semanaLetivaDTO );
	PagingLoadResult< HorarioDisponivelCenarioDTO > getAllHorariosDisponiveisCenario(  CenarioDTO cenarioDTO  );
	void saveHorariosDisponiveisCenario( CenarioDTO cenarioDTO, SemanaLetivaDTO semanaLetivaDTO,
		List< HorarioDisponivelCenarioDTO > listDTO );
	void removeHorariosDisponiveisCenario(SemanaLetivaDTO semanaLetivaDTO,
			List<HorarioDisponivelCenarioDTO> listDTO);
	SemanaLetivaDTO findSemanaLetiva( Long id );
}
