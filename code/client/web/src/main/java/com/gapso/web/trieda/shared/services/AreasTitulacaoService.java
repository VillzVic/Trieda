package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.AreaTitulacaoDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("areasTitulacao")
public interface AreasTitulacaoService extends RemoteService
{
	AreaTitulacaoDTO getAreaTitulacao( Long id );
	PagingLoadResult< AreaTitulacaoDTO > getBuscaList( CenarioDTO cenarioDTO, String nome, String descricao, PagingLoadConfig config );
	ListLoadResult< AreaTitulacaoDTO > getListAll();
	void save( AreaTitulacaoDTO areaTitulacaoDTO );
	boolean remove( List< AreaTitulacaoDTO > areaTitulacaoDTOList );
	List< AreaTitulacaoDTO > getListVinculadas( CursoDTO cursoDTO );
	List< AreaTitulacaoDTO > getListNaoVinculadas( CenarioDTO cenarioDTO, CursoDTO cursoDTO );
	void vincula( CursoDTO cursoDTO, List< AreaTitulacaoDTO > areasTitulacaoDTO );
	void desvincula( CursoDTO cursoDTO, List< AreaTitulacaoDTO > areasTitulacaoDTO );
}
