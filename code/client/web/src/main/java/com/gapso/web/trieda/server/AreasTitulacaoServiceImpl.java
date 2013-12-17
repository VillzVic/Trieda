package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Set;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.AreaTitulacao;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curso;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AreaTitulacaoDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.services.AreasTitulacaoService;

public class AreasTitulacaoServiceImpl
	extends RemoteService implements AreasTitulacaoService
{
	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public AreaTitulacaoDTO getAreaTitulacao( Long id )
	{
		return ConvertBeans.toAreaTitulacaoDTO(
			AreaTitulacao.find( id, this.getInstituicaoEnsinoUser() ) );
	}

	@Override
	public PagingLoadResult< AreaTitulacaoDTO > getBuscaList(
		CenarioDTO cenarioDTO, String nome, String descricao,
		PagingLoadConfig config )
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		List< AreaTitulacaoDTO > list = new ArrayList< AreaTitulacaoDTO >();
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

		List< AreaTitulacao > listAreaTitulacao = AreaTitulacao.findBy(
			this.getInstituicaoEnsinoUser(), cenario, nome, descricao,
			config.getOffset(), config.getLimit(), orderBy );

		for ( AreaTitulacao areaTitulacao : listAreaTitulacao )
		{
			list.add( ConvertBeans.toAreaTitulacaoDTO( areaTitulacao ) );
		}

		BasePagingLoadResult< AreaTitulacaoDTO > result
			= new BasePagingLoadResult< AreaTitulacaoDTO >( list );

		result.setOffset( config.getOffset() );
		result.setTotalLength( AreaTitulacao.count(
			this.getInstituicaoEnsinoUser(), cenario, nome, descricao ) );

		return result;
	}

	@Override
	public ListLoadResult< AreaTitulacaoDTO > getListAll()
	{
		List< AreaTitulacaoDTO > listDTO = new ArrayList< AreaTitulacaoDTO >();
		List< AreaTitulacao > list = AreaTitulacao.findByCenario(
			this.getInstituicaoEnsinoUser(), getCenario() );

		for ( AreaTitulacao areaTitulacao : list )
		{
			listDTO.add( ConvertBeans.toAreaTitulacaoDTO( areaTitulacao ) );
		}

		return new BaseListLoadResult<AreaTitulacaoDTO>( listDTO );
	}

	@Override
	public void save( AreaTitulacaoDTO areaTitulacaoDTO )
	{
		AreaTitulacao areaTitulacao
			= ConvertBeans.toAreaTitulacao( areaTitulacaoDTO );

		if ( areaTitulacao.getId() != null && areaTitulacao.getId() > 0 )
		{
			areaTitulacao.merge();
		}
		else
		{
			areaTitulacao.persist();
		}
	}

	@Override
	public boolean remove( List< AreaTitulacaoDTO > areaTitulacaoDTOList )
	{
		// Caso alguma das áreas de titulação não possa ser removida, a flag
		// retorna 'false'. Caso todas sejam removidas, retorna 'true'.
		boolean flag = true;

		for ( AreaTitulacaoDTO areaTitulacaoDTO : areaTitulacaoDTOList )
		{
			flag &= ConvertBeans.toAreaTitulacao( areaTitulacaoDTO ).remove();
		}

		return flag;
	}

	@Override
	public List< AreaTitulacaoDTO > getListVinculadas( CursoDTO cursoDTO )
	{
		if ( cursoDTO == null )
		{
			return Collections.< AreaTitulacaoDTO >emptyList();
		}

		Curso curso = Curso.find(
			cursoDTO.getId(), this.getInstituicaoEnsinoUser() );

		Set< AreaTitulacao > areaTitulacaoList = curso.getAreasTitulacao();
		List< AreaTitulacaoDTO > areaTitulacaoDTOList
			= new ArrayList< AreaTitulacaoDTO >( areaTitulacaoList.size() );

		for ( AreaTitulacao areaTitulacao : areaTitulacaoList )
		{
			areaTitulacaoDTOList.add(
				ConvertBeans.toAreaTitulacaoDTO( areaTitulacao ) );
		}

		return areaTitulacaoDTOList;
	}

	@Override
	public List< AreaTitulacaoDTO > getListNaoVinculadas( CenarioDTO cenarioDTO, CursoDTO cursoDTO )
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		if ( cursoDTO == null )
		{
			return Collections.< AreaTitulacaoDTO >emptyList();
		}

		Curso curso = Curso.find(
			cursoDTO.getId(), this.getInstituicaoEnsinoUser() );

		Set< AreaTitulacao > areaTitulacaoList = curso.getAreasTitulacao();
		List< AreaTitulacao > naoAssociadasList
			= AreaTitulacao.findByCenario( this.getInstituicaoEnsinoUser(), cenario );

		naoAssociadasList.removeAll( areaTitulacaoList );

		List< AreaTitulacaoDTO > areaTitulacaoDTOList
			= new ArrayList< AreaTitulacaoDTO >( naoAssociadasList.size() );

		for ( AreaTitulacao areaTitulacao : naoAssociadasList )
		{
			areaTitulacaoDTOList.add(
				ConvertBeans.toAreaTitulacaoDTO( areaTitulacao ) );
		}

		return areaTitulacaoDTOList;
	}

	@Override
	public void vincula( CursoDTO cursoDTO, List< AreaTitulacaoDTO > areasTitulacaoDTO )
	{
		Curso curso = Curso.find(
			cursoDTO.getId(), this.getInstituicaoEnsinoUser() );

		for ( AreaTitulacaoDTO areaTitulacaoDTO : areasTitulacaoDTO )
		{
			AreaTitulacao area = AreaTitulacao.find(
				areaTitulacaoDTO.getId(), this.getInstituicaoEnsinoUser() );

			area.getCursos().add( curso );
			area.merge();
		}
	}

	@Override
	public void desvincula( CursoDTO cursoDTO, List< AreaTitulacaoDTO > areasTitulacaoDTO )
	{
		Curso curso = Curso.find(
			cursoDTO.getId(), this.getInstituicaoEnsinoUser() );

		for ( AreaTitulacaoDTO areaTitulacaoDTO : areasTitulacaoDTO )
		{
			AreaTitulacao area = AreaTitulacao.find(
				areaTitulacaoDTO.getId(), this.getInstituicaoEnsinoUser() );
			area.getCursos().remove( curso );
			area.merge();
		}
	}
}
