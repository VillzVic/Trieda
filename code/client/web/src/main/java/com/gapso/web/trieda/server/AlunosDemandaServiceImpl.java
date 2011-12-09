package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

import org.springframework.transaction.annotation.Transactional;

import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.gapso.trieda.domain.AlunoDemanda;
import com.gapso.trieda.domain.Demanda;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AlunoDemandaDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.services.AlunosDemandaService;
import com.gapso.web.trieda.shared.util.view.TriedaException;

@Transactional
public class AlunosDemandaServiceImpl
	extends RemoteService
	implements AlunosDemandaService
{
	private static final long serialVersionUID = -8767166425201585831L;

	@Override
	public AlunoDemandaDTO getAlunoDemanda( Long id )
	{
		return ConvertBeans.toAlunoDemandaDTO(
			AlunoDemanda.find( id, getInstituicaoEnsinoUser() ) );
	}

	@Override
	public ListLoadResult< AlunoDemandaDTO > getAlunosDemandaList( DemandaDTO demandaDTO )
	{
		List< AlunoDemandaDTO > list	
			= new ArrayList< AlunoDemandaDTO >();

		Demanda demanda = null;

		if ( demandaDTO != null )
		{
			demanda = Demanda.find( demandaDTO.getId(), getInstituicaoEnsinoUser() );

			List< AlunoDemanda > listDomains = AlunoDemanda.findByDemanda(
				getInstituicaoEnsinoUser(), demanda );

			list.addAll( ConvertBeans.toListAlunoDemandaDTO( listDomains ) );
		}

		Collections.sort( list, new Comparator< AlunoDemandaDTO >()
		{
			@Override
			public int compare( AlunoDemandaDTO o1, AlunoDemandaDTO o2 )
			{
				return o1.getAlunoString().compareTo( o2.getAlunoString() );
			}
		});

		BasePagingLoadResult< AlunoDemandaDTO > result
			= new BasePagingLoadResult< AlunoDemandaDTO >( list );

		result.setTotalLength( list.size() );

		return result;
	}

	@Override
	public void saveAlunoDemanda(
		DemandaDTO demandaDTO, AlunoDemandaDTO alunoDemandaDTO ) throws TriedaException
	{
		try {
			AlunoDemanda alunoDemanda
				= ConvertBeans.toAlunoDemanda( alunoDemandaDTO );
	
			if ( alunoDemanda.getId() != null
				&& alunoDemanda.getId() > 0 )
			{
				alunoDemanda.merge();
			}
			else
			{
				alunoDemanda.persist();
			}
		} catch (Exception e) {
			e.printStackTrace();
			throw new TriedaException(e);
		}
	}

	@Override
	public void removeAlunosDemanda( List< AlunoDemandaDTO > list )
	{
		for ( AlunoDemandaDTO alunoDemandaDTO : list )
		{
			AlunoDemanda alunoDemanda =
				ConvertBeans.toAlunoDemanda( alunoDemandaDTO );

			alunoDemanda.remove();
		}
	}
}
