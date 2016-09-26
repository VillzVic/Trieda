package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
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
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Turma;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.server.util.TriedaServerUtil;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.TurmaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.services.TurmaService;
import com.gapso.web.trieda.shared.util.view.TriedaException;

@Transactional
public class TurmaServiceImpl
	extends RemoteService
	implements TurmaService
{
	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public ListLoadResult< TurmaDTO > getListByDisciplina( CenarioDTO cenarioDTO, DisciplinaDTO disciplinaDTO )
	{
		List< TurmaDTO > list = new ArrayList< TurmaDTO >();
		Disciplina disciplina = Disciplina.find(disciplinaDTO.getId(), this.getInstituicaoEnsinoUser() );
		Cenario cenario = Cenario.find(cenarioDTO.getId(), this.getInstituicaoEnsinoUser() );

		List< Turma > turmas = Turma.findByDisciplina(
			getInstituicaoEnsinoUser(), cenario, disciplina );

		for ( Turma turma : turmas )
		{
			list.add( ConvertBeans.toTurmaDTO( turma ) );
		}

		return new BaseListLoadResult< TurmaDTO >( list );
	}
	
	
	@Override
	public ListLoadResult< TurmaDTO > getListAllDisciplinaTodos( CenarioDTO cenarioDTO ){
		List< TurmaDTO > list = new ArrayList< TurmaDTO >();
		/*Disciplina disciplina = null;
		Cenario cenario = Cenario.find(cenarioDTO.getId(), this.getInstituicaoEnsinoUser() );

		List< Turma > turmas = Turma.findByDisciplina(
			getInstituicaoEnsinoUser(), cenario, disciplina );

		for ( Turma turma : turmas )
		{
			list.add( ConvertBeans.toTurmaDTO( turma ) );
		}*/
		
		TurmaDTO t = new TurmaDTO();
		t.setId(Long.parseLong("1"));
		t.setNome("Teste 1");
		list.add(t);

		return new BaseListLoadResult< TurmaDTO >( list );
	}
	
	@Override
	public ListLoadResult< TurmaDTO > getListAll( CenarioDTO cenarioDTO ){
		List< TurmaDTO > list = new ArrayList< TurmaDTO >();
		Cenario cenario = Cenario.find(cenarioDTO.getId(), this.getInstituicaoEnsinoUser() );
		List< Turma > turmas = Turma.findAll(getInstituicaoEnsinoUser(), cenario);

		for ( Turma turma : turmas )
		{
			list.add( ConvertBeans.toTurmaDTO( turma ) );
		}
		
		return new BaseListLoadResult< TurmaDTO >( list );

	}
	
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
