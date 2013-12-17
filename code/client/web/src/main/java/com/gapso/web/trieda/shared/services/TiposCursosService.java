package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.TipoCursoDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath( "tiposCursos" )
public interface TiposCursosService	
	extends RemoteService
{
	PagingLoadResult< TipoCursoDTO > getBuscaList( CenarioDTO cenarioDTO, String nome, String descricao, PagingLoadConfig config );
	void save( TipoCursoDTO tipoCursoDTO );
	void remove( List< TipoCursoDTO > tipoCursoDTOList );
	ListLoadResult< TipoCursoDTO > getList( CenarioDTO cenarioDTO, BasePagingLoadConfig loadConfig );
	TipoCursoDTO getTipoCurso( Long id );
	ListLoadResult< TipoCursoDTO > getList( CenarioDTO cenarioDTO );
}
