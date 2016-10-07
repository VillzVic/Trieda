package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import org.springframework.transaction.annotation.Transactional;

import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.Turma;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.TurmaDTO;
import com.gapso.web.trieda.shared.services.TurmaService;

@Transactional
public class TurmaServiceImpl
	extends RemoteService
	implements TurmaService
{
	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public ListLoadResult< TurmaDTO > getListByDisciplina( CenarioDTO cenarioDTO, DisciplinaDTO disciplinaDTO )
	{
		/*
		List< TurmaDTO > list = new ArrayList< TurmaDTO >();		
		Disciplina disciplina = Disciplina.find(disciplinaDTO.getId(), this.getInstituicaoEnsinoUser() );		
		Cenario cenario = Cenario.find(cenarioDTO.getId(), this.getInstituicaoEnsinoUser() );
		
		
		List< Turma > turmas = Turma.findByDisciplina(getInstituicaoEnsinoUser(), cenario, disciplina );
		for ( Turma turma : turmas ){ list.add( ConvertBeans.toTurmaDTO( turma ) );}
		
		return new BaseListLoadResult< TurmaDTO >( list );
		*/
		
		
		List< TurmaDTO > ls = new ArrayList< TurmaDTO >();
		TurmaDTO t = new TurmaDTO();
		t.setId(Long.parseLong("1"));
		t.setNome("Teste 1");
		ls.add(t);
		return new BaseListLoadResult< TurmaDTO >( ls );
				
	}
	
	
	@Override
	public ListLoadResult< TurmaDTO > getListAllDisciplinaTodos( CenarioDTO cenarioDTO ){
		/*
		List< TurmaDTO > list = new ArrayList< TurmaDTO >();
		
		Disciplina disciplina = null;
		Cenario cenario = Cenario.find(cenarioDTO.getId(), this.getInstituicaoEnsinoUser() );

		List< Turma > turmas = Turma.findByDisciplina(getInstituicaoEnsinoUser(), cenario, disciplina );

		for ( Turma turma : turmas )
		{
			list.add( ConvertBeans.toTurmaDTO( turma ) );
		}
		
		return new BaseListLoadResult< TurmaDTO >( list );
		*/
		
		List< TurmaDTO > ls = new ArrayList< TurmaDTO >();
		TurmaDTO t = new TurmaDTO();
		t.setId(Long.parseLong("1"));
		t.setNome("Teste 1");
		ls.add(t);
		return new BaseListLoadResult< TurmaDTO >( ls );
	}
	
	@Override
	public ListLoadResult< TurmaDTO > getListAll( CenarioDTO cenarioDTO ){
		/*
		List< TurmaDTO > list = new ArrayList< TurmaDTO >();
		Cenario cenario = Cenario.find(cenarioDTO.getId(), this.getInstituicaoEnsinoUser() );
		List< Turma > turmas = Turma.findAll(getInstituicaoEnsinoUser(), cenario);

		for ( Turma turma : turmas )
		{
			list.add( ConvertBeans.toTurmaDTO( turma ) );
		}
		
		return new BaseListLoadResult< TurmaDTO >( list );
		*/
		
		List< TurmaDTO > ls = new ArrayList< TurmaDTO >();
		TurmaDTO t = new TurmaDTO();
		t.setId(Long.parseLong("1"));
		t.setNome("Teste 1");
		ls.add(t);
		return new BaseListLoadResult< TurmaDTO >( ls );
	}
	
	/*@Override
	public ListLoadResult< AtendimentoOperacionalDTO > getTurmasPorDisciplina( DisciplinaDTO disciplinaDTO )
	{
		List< AtendimentoOperacionalDTO > list = new ArrayList< AtendimentoOperacionalDTO >();
		Disciplina disciplina = Disciplina.find(
						disciplinaDTO.getId(), getInstituicaoEnsinoUser() );
		List< AtendimentoOperacional > turmas = AtendimentoOperacional.findByDisciplinaOtimizada(
			getInstituicaoEnsinoUser(), disciplina );
		
		for ( AtendimentoOperacional turma : turmas )
		{
			list.add( ConvertBeans.toAtendimentoOperacionalDTO( turma ) );
		}

		return new BaseListLoadResult< AtendimentoOperacionalDTO >( list );
	}*/
	
	@Override
	public TurmaDTO getTurma( Long id )
	{
		if ( id == null )
		{
			return null;
		}

		Turma turma = Turma.find(
			id, getInstituicaoEnsinoUser() );

		if ( turma == null )
		{
			return null;
		}

		return ConvertBeans.toTurmaDTO( turma );
	}
}
