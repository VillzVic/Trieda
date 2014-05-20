package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.DivisaoCredito;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DivisaoCreditoDTO;
import com.gapso.web.trieda.shared.services.DivisoesCreditosService;
import com.gapso.web.trieda.shared.util.view.TriedaException;

public class DivisoesCreditosServiceImpl
	extends RemoteService implements DivisoesCreditosService
{
	private static final long serialVersionUID = 245474536841101290L;

	@Override
	public DivisaoCreditoDTO getDivisaoCredito( Long id )
	{
		return ConvertBeans.toDivisaoCreditoDTO(
			DivisaoCredito.find( id, getInstituicaoEnsinoUser() ) );
	}

	@Override
	public PagingLoadResult< DivisaoCreditoDTO > getList(
		CenarioDTO cenarioDTO, PagingLoadConfig config )
	{
		Cenario cenario = Cenario.find( cenarioDTO.getId(),
			this.getInstituicaoEnsinoUser() );

		List< DivisaoCreditoDTO > list = new ArrayList< DivisaoCreditoDTO >();
		List< DivisaoCredito > divisoesCreditos = DivisaoCredito.findWithoutDisciplina(
			cenario, config.getOffset(), config.getLimit(), getInstituicaoEnsinoUser() );

		for ( DivisaoCredito divisaoCredito : divisoesCreditos )
		{
			list.add( ConvertBeans.toDivisaoCreditoDTO( divisaoCredito ) );
		}

		Collections.sort( list, new Comparator< DivisaoCreditoDTO >()
		{
			@Override
			public int compare( DivisaoCreditoDTO d1, DivisaoCreditoDTO d2 )
			{
				return d1.getTotalCreditos().compareTo( d2.getTotalCreditos() );
			}
		});

		BasePagingLoadResult< DivisaoCreditoDTO > result
			= new BasePagingLoadResult< DivisaoCreditoDTO >( list );

		result.setOffset( config.getOffset() );
		result.setTotalLength( DivisaoCredito.count(
			cenario, getInstituicaoEnsinoUser() ) );

		return result;
	}
	
	@Override
	public PagingLoadResult< DivisaoCreditoDTO > getListComDisciplinas(
		CenarioDTO cenarioDTO, PagingLoadConfig config )
	{
		Cenario cenario = Cenario.find( cenarioDTO.getId(),
			this.getInstituicaoEnsinoUser() );

		List< DivisaoCreditoDTO > list = new ArrayList< DivisaoCreditoDTO >();
		List< DivisaoCredito > divisoesCreditos = DivisaoCredito.findWithDisciplina(
			cenario, config.getOffset(), config.getLimit(), getInstituicaoEnsinoUser() );

		for ( DivisaoCredito divisaoCredito : divisoesCreditos )
		{
			list.add( ConvertBeans.toDivisaoCreditoDTO( divisaoCredito ) );
		}

		Collections.sort( list, new Comparator< DivisaoCreditoDTO >()
		{
			@Override
			public int compare( DivisaoCreditoDTO d1, DivisaoCreditoDTO d2 )
			{
				return d1.getTotalCreditos().compareTo( d2.getTotalCreditos() );
			}
		});

		BasePagingLoadResult< DivisaoCreditoDTO > result
			= new BasePagingLoadResult< DivisaoCreditoDTO >( list );

		result.setOffset( config.getOffset() );
		result.setTotalLength( DivisaoCredito.countWithDisciplina(
			cenario, getInstituicaoEnsinoUser() ) );

		return result;
	}
	
	@Override
	public void save( DivisaoCreditoDTO divisaoCreditoDTO ) throws TriedaException
	{
		Cenario cenario = Cenario.find(divisaoCreditoDTO.getCenarioId(), getInstituicaoEnsinoUser());
		
		DivisaoCredito divisaoCredito
			= ConvertBeans.toDivisaoCredito( divisaoCreditoDTO );

		if ( divisaoCredito.getId() != null
			&& divisaoCredito.getId() > 0 )
		{
			divisaoCredito.merge();
		}
		else
		{
			if (DivisaoCredito.findByDias(getInstituicaoEnsinoUser(), cenario, divisaoCredito.getDia1(), divisaoCredito.getDia2(),
					divisaoCredito.getDia3(), divisaoCredito.getDia4(), divisaoCredito.getDia5(), divisaoCredito.getDia6(),
					divisaoCredito.getDia7()).isEmpty())
				divisaoCredito.persist();
			else {
				throw new TriedaException("Esta divisão de creditos já existe");
			}
		}
	}
	
	@Override
	public void saveWithDisciplina( DivisaoCreditoDTO divisaoCreditoDTO )
	{
		DivisaoCredito divisaoCredito
			= ConvertBeans.toDivisaoCredito( divisaoCreditoDTO );

		if ( divisaoCredito.getId() != null
			&& divisaoCredito.getId() > 0 )
		{
			divisaoCredito.merge();
		}
		else
		{
			divisaoCredito.persist();
			divisaoCredito.getDisciplina().setDivisaoCreditos(divisaoCredito);
			divisaoCredito.getDisciplina().merge();
		}
	}

	@Override
	public void remove( List< DivisaoCreditoDTO > divisaoCreditoDTOList )
	{
		for ( DivisaoCreditoDTO divisaoCreditoDTO : divisaoCreditoDTOList )
		{
			DivisaoCredito divisaoCredito
				= ConvertBeans.toDivisaoCredito( divisaoCreditoDTO );

			if ( divisaoCredito != null )
			{
				divisaoCredito.remove();
			}
		}
	}
	
	@Override
	public void removeWithDisciplina( List< DivisaoCreditoDTO > divisaoCreditoDTOList )
	{
		for ( DivisaoCreditoDTO divisaoCreditoDTO : divisaoCreditoDTOList )
		{
			DivisaoCredito divisaoCredito
				= ConvertBeans.toDivisaoCredito( divisaoCreditoDTO );

			if ( divisaoCredito != null )
			{
				divisaoCredito.getDisciplina().setDivisaoCreditos(null);
				divisaoCredito.remove();
			}
		}
	}
}
