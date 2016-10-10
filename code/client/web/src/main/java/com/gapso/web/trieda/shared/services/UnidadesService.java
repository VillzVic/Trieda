package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DeslocamentoUnidadeDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath( "unidades" )
public interface UnidadesService
	extends RemoteService
{
	ListLoadResult< UnidadeDTO > getList();
	void save( UnidadeDTO unidadeDTO );
	void remove( List< UnidadeDTO > unidadeDTOList ) throws TriedaException;
	PagingLoadResult< UnidadeDTO > getBuscaList( CenarioDTO cenarioDTO, CampusDTO campusDTO,
		String nome, String codigo, String operadorCapSalas, Double capSalas,PagingLoadConfig config );
	ListLoadResult< UnidadeDTO > getList( CenarioDTO cenarioDTO, BasePagingLoadConfig loadConfig );
	UnidadeDTO getUnidade( Long id );
	List< DeslocamentoUnidadeDTO > getDeslocamento( CampusDTO campusDTO );
	ListLoadResult< UnidadeDTO > getListByCampus( CampusDTO campusDTO );
	PagingLoadResult< HorarioDisponivelCenarioDTO > getHorariosDisponiveis( UnidadeDTO unidadeDTO );
	void saveHorariosDisponiveis( UnidadeDTO unidadeDTO, List< HorarioDisponivelCenarioDTO > listDTO );
	void saveDeslocamento( CampusDTO campus, List< DeslocamentoUnidadeDTO > list );
	ListLoadResult< UnidadeDTO > getUnidadesPorCampusOtimizado( CampusDTO campusDTO );
}
