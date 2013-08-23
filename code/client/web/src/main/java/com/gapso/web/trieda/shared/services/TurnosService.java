package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath( "turnos" )
public interface TurnosService
	extends RemoteService
{
	TurnoDTO getTurno( Long id );
	ListLoadResult< TurnoDTO > getList( CenarioDTO cenarioDTO );
	ListLoadResult< TurnoDTO > getList( CenarioDTO cenarioDTO, BasePagingLoadConfig loadConfig );
	void save( TurnoDTO turnoDTO ) throws TriedaException;
	void remove( List< TurnoDTO > turnoDTOList ) throws TriedaException;
	PagingLoadResult< TurnoDTO > getBuscaList( String nome, CenarioDTO cenarioDTO, PagingLoadConfig config );
	ListLoadResult< TurnoDTO > getListByCampus( CenarioDTO cenarioDTO, CampusDTO campusDTO );
	ListLoadResult< TurnoDTO > getListOtimizedOnly();
}
