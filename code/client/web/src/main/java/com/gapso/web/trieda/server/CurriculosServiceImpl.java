package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;

import org.springframework.transaction.annotation.Transactional;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.Curso;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.services.CurriculosService;

@Transactional
public class CurriculosServiceImpl
	extends RemoteService
	implements CurriculosService
{
	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public CurriculoDTO getCurriculo( Long id )
	{
		return ConvertBeans.toCurriculoDTO(
			Curriculo.find( id, getInstituicaoEnsinoUser() ) );
	}

	@Override
	public ListLoadResult< CurriculoDTO > getListAll()
	{
		List< Curriculo > curriculos
			= Curriculo.findAll( getInstituicaoEnsinoUser() );

		List< CurriculoDTO > curriculosDTO
			= new ArrayList<CurriculoDTO>( curriculos.size() );

		for ( Curriculo curriculo : curriculos )
		{
			curriculosDTO.add(
				ConvertBeans.toCurriculoDTO( curriculo ) );
		}

		return new BaseListLoadResult< CurriculoDTO >( curriculosDTO );
	}

	@Override
	public ListLoadResult< CurriculoDTO > getListByCurso( CursoDTO cursoDTO )
	{
		Curso curso = Curso.find( cursoDTO.getId(), getInstituicaoEnsinoUser() );

		Set< Curriculo > curriculos = curso.getCurriculos();
		List< CurriculoDTO > curriculosDTO
			= new ArrayList< CurriculoDTO >( curriculos.size() );

		for ( Curriculo curriculo : curriculos )
		{
			curriculosDTO.add( ConvertBeans.toCurriculoDTO( curriculo ) );
		}

		return new BaseListLoadResult< CurriculoDTO >( curriculosDTO );
	}

	@Override
	public ListLoadResult< CurriculoDTO > getList( BasePagingLoadConfig config )
	{
		CursoDTO cursoDTO = config.get( "cursoDTO" );

		return getBuscaList( cursoDTO, config.get( "query" ).toString(), null, config );
	}

	@Override
	public PagingLoadResult< CurriculoDTO > getBuscaList( CursoDTO cursoDTO,
		String codigo, String descricao, PagingLoadConfig config )
	{
		List< CurriculoDTO > list = new ArrayList< CurriculoDTO >();
		String orderBy = config.getSortField();

		if ( orderBy != null )
		{
			if ( config.getSortDir() != null
				&& config.getSortDir().equals( SortDir.DESC ) )
			{
				orderBy = ( orderBy + " asc" );
			}
			else
			{
				orderBy = ( orderBy + " desc" );
			}
		}

		Curso curso = null;
		if ( cursoDTO != null )
		{
			curso = ConvertBeans.toCurso( cursoDTO );
		}

		List< Curriculo > listCurriculos = Curriculo.findBy( getInstituicaoEnsinoUser(),
			curso, codigo, descricao, config.getOffset(), config.getLimit(), orderBy );

		for ( Curriculo curriculo : listCurriculos )
		{
			list.add( ConvertBeans.toCurriculoDTO( curriculo ) );
		}

		BasePagingLoadResult< CurriculoDTO > result
			= new BasePagingLoadResult< CurriculoDTO >( list );

		result.setOffset( config.getOffset() );
		result.setTotalLength( Curriculo.count(
			getInstituicaoEnsinoUser(), curso, codigo, descricao ) );

		return result;
	}

	@Override
	public void save( CurriculoDTO curriculoDTO )
	{
		Curriculo curriculo = ConvertBeans.toCurriculo( curriculoDTO );

		if ( curriculo.getId() != null && curriculo.getId() > 0 )
		{
			curriculo.merge();
		}
		else
		{
			curriculo.persist();
		}
	}

	@Override
	public void remove( List< CurriculoDTO > curriculoDTOList )
	{
		for ( CurriculoDTO curriculoDTO : curriculoDTOList )
		{
			ConvertBeans.toCurriculo( curriculoDTO ).remove();
		}
	}

	@Override
	public ListLoadResult< CurriculoDisciplinaDTO > getDisciplinasList( CurriculoDTO curriculoDTO )
	{
		Curriculo curriculo = Curriculo.find(
			curriculoDTO.getId(), getInstituicaoEnsinoUser() );

		List< CurriculoDisciplinaDTO > listCurriculoDisciplinaDTO
			= new ArrayList< CurriculoDisciplinaDTO >();

		Set< CurriculoDisciplina > listCurriculoDisciplina
			= curriculo.getDisciplinas();

		for ( CurriculoDisciplina cd : listCurriculoDisciplina )
		{
			listCurriculoDisciplinaDTO.add( ConvertBeans.toCurriculoDisciplinaDTO( cd ) );
		}

		return new BaseListLoadResult< CurriculoDisciplinaDTO >( listCurriculoDisciplinaDTO );
	}

	@Override
	public List< Integer > getPeriodos( CurriculoDTO curriculoDTO )
	{
		Curriculo curriculo = Curriculo.find(
			curriculoDTO.getId(), getInstituicaoEnsinoUser() );

		if ( curriculo == null )
		{
			return new ArrayList< Integer >();
		}

		return curriculo.getPeriodos( getInstituicaoEnsinoUser() );
	}

	@Override
	public void saveDisciplina( CurriculoDTO curriculoDTO,
		CurriculoDisciplinaDTO curriculoDisciplinaDTO )
	{
		CurriculoDisciplina curriculoDisciplina
			= ConvertBeans.toCurriculoDisciplina( curriculoDisciplinaDTO );

		Curriculo curriculo = Curriculo.find(
			curriculoDTO.getId(), getInstituicaoEnsinoUser() );

		curriculoDisciplina.setCurriculo( curriculo );
		curriculoDisciplina.persist();
	}

	@Override
	public void removeDisciplina( List< CurriculoDisciplinaDTO > curriculoDisciplinaDTOList )
	{
		for ( CurriculoDisciplinaDTO curriculoDisciplinaDTO : curriculoDisciplinaDTOList )
		{
			ConvertBeans.toCurriculoDisciplina( curriculoDisciplinaDTO ).remove();
		}
	}
}
