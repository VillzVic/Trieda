package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

import org.springframework.transaction.annotation.Transactional;

import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Aluno;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AlunoDTO;
import com.gapso.web.trieda.shared.services.AlunosService;

@Transactional
public class AlunosServiceImpl
	extends RemoteService
	implements AlunosService
{
	private static final long serialVersionUID = -3593829435380014345L;

	@Override
	public AlunoDTO getAluno( Long id )
	{
		return ConvertBeans.toAlunoDTO(
			Aluno.find( id, getInstituicaoEnsinoUser() ) );
	}

	@Override
	public PagingLoadResult< AlunoDTO > getAlunosList(
		String nome, String cpf )
	{
		List< AlunoDTO > list	
			= new ArrayList< AlunoDTO >();

		List< Aluno > listDomains
			= Aluno.findByNomeCpf( getInstituicaoEnsinoUser(), nome, cpf );

		list.addAll( ConvertBeans.toListAlunoDTO( listDomains ) );

		Collections.sort( list, new Comparator< AlunoDTO >()
		{
			@Override
			public int compare( AlunoDTO o1, AlunoDTO o2 )
			{
				return o1.getNome().compareTo( o2.getNome() );
			}
		});

		BasePagingLoadResult< AlunoDTO > result
			= new BasePagingLoadResult< AlunoDTO >( list );

		result.setTotalLength( list.size() );

		return result;
	}

	@Override
	public void saveAluno( AlunoDTO alunoDTO )
	{
		onlyAdministrador();

		Aluno aluno = ConvertBeans.toAluno( alunoDTO );

		try
		{
			if ( aluno.getId() != null
				&& aluno.getId() > 0 )
			{
				aluno.merge();
			}
			else
			{
				aluno.persist();
			}			
		}
		catch( Exception e )
		{
			System.out.println( e.getCause() );
		}
	}

	@Override
	public void removeAlunos( List< AlunoDTO > list )
	{
		for ( AlunoDTO alunoDTO : list )
		{
			Aluno aluno = ConvertBeans.toAluno( alunoDTO );
			aluno.remove();
		}
	}
}
