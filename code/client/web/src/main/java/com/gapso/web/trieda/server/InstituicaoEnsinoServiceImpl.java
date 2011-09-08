package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.InstituicaoEnsinoDTO;
import com.gapso.web.trieda.shared.services.InstituicaoEnsinoService;

public class InstituicaoEnsinoServiceImpl
	extends RemoteService implements InstituicaoEnsinoService
{
	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public InstituicaoEnsinoDTO getInstituicaoEnsino( Long id )
	{
		return ConvertBeans.toInstituicaoEnsinoDTO(	InstituicaoEnsino.find( id ) );
	}

	@Override
	public ListLoadResult< InstituicaoEnsinoDTO > getList()
	{
		List< InstituicaoEnsinoDTO > listDTO
			= new ArrayList< InstituicaoEnsinoDTO >();

		List< InstituicaoEnsino > list = InstituicaoEnsino.findAll();

		for ( InstituicaoEnsino instituicaoEnsino : list )
		{
			listDTO.add( ConvertBeans.toInstituicaoEnsinoDTO( instituicaoEnsino ) );
		}

		return new BaseListLoadResult< InstituicaoEnsinoDTO >( listDTO );
	}
}
