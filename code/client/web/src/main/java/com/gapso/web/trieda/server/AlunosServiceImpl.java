package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import org.springframework.transaction.annotation.Transactional;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Aluno;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AlunoDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
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
		if ( id == null )
		{
			return null;
		}

		Aluno aluno = Aluno.find(
			id, getInstituicaoEnsinoUser() );

		if ( aluno == null )
		{
			return null;
		}

		return ConvertBeans.toAlunoDTO( aluno );
	}
	
	@Override
	public PagingLoadResult<AlunoDTO> getBuscaList(CenarioDTO cenarioDTO, String nome, String matricula, PagingLoadConfig config) {
		Cenario cenario = Cenario.find(cenarioDTO.getId(),this.getInstituicaoEnsinoUser());

		List<AlunoDTO> list = new ArrayList<AlunoDTO>();
		String orderBy = config.getSortField();

		if (orderBy != null) {
			if (config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = (orderBy + " asc");
			}
			else {
				orderBy = (orderBy + " desc");
			}
		}

		List<Aluno> alunos = Aluno.findBy(this.getInstituicaoEnsinoUser(),cenario,nome,matricula,config.getOffset(),config.getLimit(),orderBy);
		for (Aluno aluno : alunos) {
			list.add(ConvertBeans.toAlunoDTO(aluno));
		}

		BasePagingLoadResult<AlunoDTO> result = new BasePagingLoadResult<AlunoDTO> (list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Aluno.count(this.getInstituicaoEnsinoUser(),cenario,nome,matricula));

		return result;
	}

	@Override
	public PagingLoadResult< AlunoDTO > getAlunosList(
		String nome, String matricula )
	{
		List< Aluno > listDomains = null;

		// Carregar uma quantidade menor de dados
		if ( ( nome == null || nome.equals( "" ) )
			&& ( matricula == null || matricula.equals( "" ) ) )
		{
			listDomains = Aluno.findMaxResults(
				getInstituicaoEnsinoUser(), 20 );
		}
		// Todos os alunos que segundo
		// à busca realizada pelo usuário
		else
		{
			listDomains = Aluno.findByNomeMatricula(
				getInstituicaoEnsinoUser(), nome, matricula );
		}

		List< AlunoDTO > list	
			= new ArrayList< AlunoDTO >();

		list.addAll( ConvertBeans.toListAlunoDTO( listDomains ) );

		Collections.sort( list );

		BasePagingLoadResult< AlunoDTO > result
			= new BasePagingLoadResult< AlunoDTO >( list );

		result.setTotalLength( list.size() );

		return result;
	}

	@Override
	public PagingLoadResult<AlunoDTO> getAlunosListByCampus(CampusDTO campusDTO){
		Campus campus = Campus.find(campusDTO.getId(), this.getInstituicaoEnsinoUser());
		boolean tatico = AtendimentoTatico.findAllByCampus(this.getInstituicaoEnsinoUser(), campus).size() != 0;

		List<Aluno> listDomains = Aluno.findByCampus(getInstituicaoEnsinoUser(), campus, tatico);

		List<AlunoDTO> list = new ArrayList<AlunoDTO>();
		list.addAll( ConvertBeans.toListAlunoDTO(listDomains));

		BasePagingLoadResult<AlunoDTO> result = new BasePagingLoadResult<AlunoDTO>(list);
		result.setTotalLength(list.size());

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
