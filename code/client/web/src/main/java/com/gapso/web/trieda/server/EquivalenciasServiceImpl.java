package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.Equivalencia;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.EquivalenciaDTO;
import com.gapso.web.trieda.shared.services.EquivalenciasService;

public class EquivalenciasServiceImpl
	extends RemoteService implements EquivalenciasService
{
	private static final long serialVersionUID = 345113452407626806L;

	@Override
	public EquivalenciaDTO getEquivalencia( Long id )
	{
		if ( id == null )
		{
			return null;
		}

		Equivalencia equivalencia
			= Equivalencia.find( id, getInstituicaoEnsinoUser() );  
		
		if ( equivalencia == null )
		{
			return null;
		}

		return ConvertBeans.toEquivalenciaDTO( equivalencia );
	}
	
	@Override
	public PagingLoadResult<EquivalenciaDTO> getBuscaList(DisciplinaDTO disciplinaDTO, PagingLoadConfig config) {
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}

		Disciplina disciplina = (disciplinaDTO == null) ? null :
			Disciplina.find( disciplinaDTO.getId(), getInstituicaoEnsinoUser() );

		List< Equivalencia > list = Equivalencia.findBy( getInstituicaoEnsinoUser(),
			disciplina, config.getOffset(), config.getLimit(), orderBy );

		List< EquivalenciaDTO > listDTO
			= new ArrayList< EquivalenciaDTO >();

		for ( Equivalencia equivalencia : list )
		{
			listDTO.add( ConvertBeans.toEquivalenciaDTO( equivalencia ) );
		}

		BasePagingLoadResult< EquivalenciaDTO > result
			= new BasePagingLoadResult< EquivalenciaDTO >( listDTO );

		result.setOffset( config.getOffset() );
		result.setTotalLength( Equivalencia.count(
			getInstituicaoEnsinoUser(), disciplina ) );

		return result;
	}
	
	@Override
	public void save( EquivalenciaDTO equivalenciaDTO, List< DisciplinaDTO > eliminaList )
	{
		Equivalencia equivalencia
			= ConvertBeans.toEquivalencia( equivalenciaDTO );

		for ( DisciplinaDTO d : eliminaList )
		{
			equivalencia.getElimina().add(
				Disciplina.find( d.getId(), getInstituicaoEnsinoUser() ) );
		}

		equivalencia.persist();
	}
	
	@Override
	public void remove( List< EquivalenciaDTO > equivalenciaDTOList )
	{
		for ( EquivalenciaDTO turnoDTO : equivalenciaDTOList )
		{
			Equivalencia equivalencia
				= Equivalencia.find( turnoDTO.getId(), getInstituicaoEnsinoUser() ); 

			if ( equivalencia != null )
			{
				equivalencia.remove();
			}
		}
	}

}
