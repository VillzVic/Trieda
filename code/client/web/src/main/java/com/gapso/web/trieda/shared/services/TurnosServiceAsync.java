package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface TurnosServiceAsync
{
	void getTurno( Long id, AsyncCallback< TurnoDTO > callback );
	void getList(  CenarioDTO cenarioDTO, AsyncCallback< ListLoadResult< TurnoDTO > > callback );
	void getList( CenarioDTO cenarioDTO, BasePagingLoadConfig loadConfig, AsyncCallback< ListLoadResult< TurnoDTO > > callback );
	void save( TurnoDTO turnoDTO, AsyncCallback< Void > callback );
	void remove( List< TurnoDTO > turnoDTOList, AsyncCallback< Void > callback );
	void getBuscaList( String nome, CenarioDTO cenarioDTO, PagingLoadConfig config, AsyncCallback< PagingLoadResult< TurnoDTO > > callback );
	void getListByCampus( CenarioDTO cenarioDTO, CampusDTO campusDTO, AsyncCallback< ListLoadResult< TurnoDTO > > callback );
	void getListOtimizedOnly( AsyncCallback< ListLoadResult< TurnoDTO > > callback );
	void getTurnosNaoSelecionadosParaOtimizacao(CenarioDTO cenarioDTO, List<TurnoDTO> turnosSelecionados,
			AsyncCallback<ListLoadResult<TurnoDTO>> callback);
}
