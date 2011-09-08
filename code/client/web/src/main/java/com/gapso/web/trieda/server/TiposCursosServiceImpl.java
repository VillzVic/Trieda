package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.TipoCurso;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.TipoCursoDTO;
import com.gapso.web.trieda.shared.services.TiposCursosService;

public class TiposCursosServiceImpl
	extends RemoteService
	implements TiposCursosService
{
	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public TipoCursoDTO getTipoCurso( Long id )
	{
		return ConvertBeans.toTipoCursoDTO( TipoCurso.find(
			id, getUsuario().getInstituicaoEnsino() ) );
	}

	@Override
	public ListLoadResult< TipoCursoDTO > getList()
	{
		List< TipoCursoDTO > list = new ArrayList< TipoCursoDTO >();
		List< TipoCurso > listDomains
			= TipoCurso.findAll( getUsuario().getInstituicaoEnsino() );

		for ( TipoCurso tipoCurso : listDomains )
		{
			list.add( ConvertBeans.toTipoCursoDTO( tipoCurso ) );
		}

		BaseListLoadResult< TipoCursoDTO > result
			= new BaseListLoadResult< TipoCursoDTO >( list );

		return result;
	}

	@Override
	public PagingLoadResult< TipoCursoDTO > getBuscaList(
		String nome, String descricao, PagingLoadConfig config )
	{
		List< TipoCursoDTO > list = new ArrayList< TipoCursoDTO >();
		String orderBy = config.getSortField();

		if ( orderBy != null )
		{
			if ( config.getSortDir() != null
				&& config.getSortDir().equals( SortDir.DESC ) )
			{
				orderBy = orderBy + " asc";
			}
			else
			{
				orderBy = orderBy + " desc";
			}
		}

		List< TipoCurso > listTipos = TipoCurso.findBy(
			nome, descricao, config.getOffset(), config.getLimit(),
			orderBy, getUsuario().getInstituicaoEnsino() );

		for ( TipoCurso tipoCurso : listTipos )
		{
			list.add( ConvertBeans.toTipoCursoDTO( tipoCurso ) );
		}

		BasePagingLoadResult< TipoCursoDTO > result
			= new BasePagingLoadResult< TipoCursoDTO >( list );

		result.setOffset( config.getOffset() );
		result.setTotalLength( TipoCurso.count(
			nome, descricao, getUsuario().getInstituicaoEnsino() ) );

		return result;
	}

	@Override
	public ListLoadResult< TipoCursoDTO > getList( BasePagingLoadConfig loadConfig )
	{
		return getBuscaList( loadConfig.get( "query" ).toString(), null, loadConfig );
	}

	@Override
	public void save( TipoCursoDTO tipoCursoDTO )
	{
		TipoCurso tipoCurso = ConvertBeans.toTipoCurso( tipoCursoDTO );

		if ( tipoCurso.getId() != null
			&& tipoCurso.getId() > 0 )
		{
			tipoCurso.merge();
		}
		else
		{
			tipoCurso.persist();
		}
	}

	@Override
	public void remove( List< TipoCursoDTO > tipoCursoDTOList )
	{
		for( TipoCursoDTO tipoCursoDTO : tipoCursoDTOList )
		{
			ConvertBeans.toTipoCurso( tipoCursoDTO ).remove();
		}
	}
}
