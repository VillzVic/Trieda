package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

import org.springframework.transaction.annotation.Transactional;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.AlunoDemanda;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Demanda;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AlunoDemandaDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.ResumoMatriculaDTO;
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
	
	@Override
	public PagingLoadResult<ResumoMatriculaDTO> getResumoAtendimentosDisciplinaList( String codigo, CampusDTO campusDTO, CursoDTO cursoDTO,
			PagingLoadConfig config ) {
		List< ResumoMatriculaDTO > list = new ArrayList< ResumoMatriculaDTO >();
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

		Campus campus = null;
		
		if ( campusDTO != null )
		{
			campus = ConvertBeans.toCampus(campusDTO);
		}
		
		Curso curso = null;
		
		if ( cursoDTO != null )
		{
			curso = ConvertBeans.toCurso(cursoDTO);
		}
		
		int numTotalDisciplinas = AlunoDemanda.countDisciplinas(getInstituicaoEnsinoUser(), codigo, campus, curso);
		
		List< AlunoDemanda > busca = AlunoDemanda.findDisciplinasBy(getInstituicaoEnsinoUser(), codigo, campus, curso,
				config.getOffset(), config.getLimit(), orderBy);
		
		for ( AlunoDemanda disciplinas : busca )
		{
			List < AlunoDemanda > alunoDemanda = AlunoDemanda.findByDisciplinaAndCampus(getInstituicaoEnsinoUser(),
					disciplinas.getDemanda().getDisciplina(), disciplinas.getDemanda().getOferta().getCampus());
			ResumoMatriculaDTO resumoMatricula = new ResumoMatriculaDTO();
			resumoMatricula.setCampusString(disciplinas.getDemanda().getOferta().getCampus().getNome());
			resumoMatricula.setCodDisciplina(disciplinas.getDemanda().getDisciplina().getCodigo());
			int disDemandaP1 = 0;
			int disAtendidosP1 = 0;
			int disNaoAtendidosP1 = 0;
			int disAtendidosP2 = 0;
			int disAtendidosSoma = 0;
			int disDemandaNaoAtendida = 0;
			for ( AlunoDemanda demandas : alunoDemanda )
			{
				if ( demandas.getDemanda().ocupaGrade() )
				{
					if ( demandas.getPrioridade() == 1 )
					{
						disDemandaP1++;
						if ( demandas.getAtendido() )
						{
							disAtendidosP1++;
						}
						else
						{
							disNaoAtendidosP1++;
						}
					}
					else
					{
						if ( demandas.getAtendido() )
						{
							disAtendidosP2++;
						}
					}
				}
				disAtendidosSoma = disAtendidosP1 + disAtendidosP2;
				disDemandaNaoAtendida = disDemandaP1 - (disAtendidosSoma);
			}
			resumoMatricula.setDisDemandaP1(disDemandaP1);
			resumoMatricula.setDisAtendidosP1(disAtendidosP1);
			resumoMatricula.setDisNaoAtendidosP1(disNaoAtendidosP1);
			resumoMatricula.setDisAtendidosP2(disAtendidosP2);
			resumoMatricula.setDisAtendidosSoma(disAtendidosSoma);
			resumoMatricula.setDisDemandaNaoAtendida(disDemandaNaoAtendida);
			
			list.add(resumoMatricula);
		}
		BasePagingLoadResult< ResumoMatriculaDTO > result
		= new BasePagingLoadResult< ResumoMatriculaDTO >( list );

		result.setOffset( config.getOffset() );
		result.setTotalLength( numTotalDisciplinas );
	
		return result;
	}

	@Override
	public PagingLoadResult<ResumoMatriculaDTO> getResumoMatriculasList( String aluno, String matricula, CampusDTO campusDTO, CursoDTO cursoDTO,
			PagingLoadConfig config ) {
		List< ResumoMatriculaDTO > list = new ArrayList< ResumoMatriculaDTO >();
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

		Campus campus = null;
		
		if ( campusDTO != null )
		{
			campus = ConvertBeans.toCampus(campusDTO);
		}
		
		Curso curso = null;
		
		if ( cursoDTO != null )
		{
			curso = ConvertBeans.toCurso(cursoDTO);
		}
		
		int numTotalMatriculas = AlunoDemanda.countMatriculas(getInstituicaoEnsinoUser(), aluno, matricula, campus, curso);
		
		List< AlunoDemanda > busca = AlunoDemanda.findMatriculasBy(getInstituicaoEnsinoUser(), aluno, matricula, campus, curso,
				config.getOffset(), config.getLimit(), orderBy);
		
		for ( AlunoDemanda alunos : busca )
		{
			List < AlunoDemanda > alunoDemanda = AlunoDemanda.findByAluno(getInstituicaoEnsinoUser(), alunos.getAluno());
			ResumoMatriculaDTO resumoMatricula = new ResumoMatriculaDTO();
			resumoMatricula.setCampusString(alunos.getDemanda().getOferta().getCampus().getNome());
			resumoMatricula.setAlunoMatricula(alunos.getAluno().getMatricula());
			int credDemandaP1 = 0;
			int credAtendidosP1 = 0;
			int credNaoAtendidosP1 = 0;
			int credAtendidosP2 = 0;
			int disDemandaP1 = 0;
			int disAtendidosP1 = 0;
			int disNaoAtendidosP1 = 0;
			int disAtendidosP2 = 0;
			int credExcessoP2 = 0;
			for ( AlunoDemanda demandas : alunoDemanda )
			{
				if ( demandas.getDemanda().ocupaGrade() )
				{
					if ( demandas.getPrioridade() == 1 )
					{
						credDemandaP1 += demandas.getDemanda().getDisciplina().getCreditosTotal();
						disDemandaP1++;
						if ( demandas.getAtendido() )
						{
							credAtendidosP1 +=demandas.getDemanda().getDisciplina().getCreditosTotal();
							disAtendidosP1++;
						}
						else
						{
							credNaoAtendidosP1 +=demandas.getDemanda().getDisciplina().getCreditosTotal();
							disNaoAtendidosP1++;
						}
					}
					else
					{
						if ( demandas.getAtendido() )
						{
							credAtendidosP2 +=demandas.getDemanda().getDisciplina().getCreditosTotal();
							disAtendidosP2++;
						}
					}
				}
				credExcessoP2 = credAtendidosP2 - credNaoAtendidosP1;
			}
			resumoMatricula.setCredAtendidosP1(credAtendidosP1);
			resumoMatricula.setCredDemandaP1(credDemandaP1);
			resumoMatricula.setCredNaoAtendidosP1(credNaoAtendidosP1);
			resumoMatricula.setCredAtendidosP2(credAtendidosP2);
			resumoMatricula.setDisDemandaP1(disDemandaP1);
			resumoMatricula.setDisAtendidosP1(disAtendidosP1);
			resumoMatricula.setDisNaoAtendidosP1(disNaoAtendidosP1);
			resumoMatricula.setDisAtendidosP2(disAtendidosP2);
			resumoMatricula.setCredExcessoP2(credExcessoP2);
			
			list.add(resumoMatricula);
		}
		BasePagingLoadResult< ResumoMatriculaDTO > result
		= new BasePagingLoadResult< ResumoMatriculaDTO >( list );

		result.setOffset( config.getOffset() );
		result.setTotalLength( numTotalMatriculas );
	
		return result;
	}
	
	public void somaCreditosDemanda( String nome, String matricula, CampusDTO campusDTO, CursoDTO cursoDTO, int numTotalmatriculas ){
		
		Campus campus = null;
		
		if ( campusDTO != null )
		{
			campus = ConvertBeans.toCampus(campusDTO);
		}
		
		Curso curso = null;
		
		if ( cursoDTO != null )
		{
			curso = ConvertBeans.toCurso(cursoDTO);
		}
		
		List< AlunoDemanda > busca = AlunoDemanda.findMatriculasBy(getInstituicaoEnsinoUser(), nome, matricula, campus, curso,
				0, numTotalmatriculas, null);
		
		int credDemandaP1 = 0;
		int credAtendidosP1 = 0;
		int credNaoAtendidosP1 = 0;
		int credAtendidosP2 = 0;
		int count = 1;
		
		for ( AlunoDemanda alunos : busca )
		{
			List < AlunoDemanda > alunoDemanda = AlunoDemanda.findByAluno(getInstituicaoEnsinoUser(), alunos.getAluno());
			ResumoMatriculaDTO resumoMatricula = new ResumoMatriculaDTO();
			resumoMatricula.setCampusString(alunos.getDemanda().getOferta().getCampus().getNome());
			resumoMatricula.setAlunoMatricula(alunos.getAluno().getMatricula());
			for ( AlunoDemanda demandas : alunoDemanda )
			{
				if ( demandas.getDemanda().ocupaGrade() )
				{
					if ( demandas.getPrioridade() == 1 )
					{
						credDemandaP1 += demandas.getDemanda().getDisciplina().getCreditosTotal();
						if ( demandas.getAtendido() )
						{
							credAtendidosP1 +=demandas.getDemanda().getDisciplina().getCreditosTotal();
						}
						else
						{
							credNaoAtendidosP1 +=demandas.getDemanda().getDisciplina().getCreditosTotal();
						}
					}
					else
					{
						if ( demandas.getAtendido() )
						{
							credAtendidosP2 +=demandas.getDemanda().getDisciplina().getCreditosTotal();
						}
					}
				}
				if (credDemandaP1 > count*1000) {
					System.out.println( count*1000 + " Creditos processados");
					count++;
				}
			}
		}
		System.out.println("Soma demanda P1: " + credDemandaP1);
		System.out.println("Soma creditos atendidos P1: " + credAtendidosP1);
		System.out.println("Soma creditos nao atendidos P1: " + credNaoAtendidosP1);
		System.out.println("Soma creditos atendidos P2: " + credAtendidosP2);
	}
	
	public void somaDisciplinasDemanda( String nome, CampusDTO campusDTO, CursoDTO cursoDTO, int numTotalDisciplinas ){
		
		Campus campus = null;
		
		if ( campusDTO != null )
		{
			campus = ConvertBeans.toCampus(campusDTO);
		}
		
		Curso curso = null;
		
		if ( cursoDTO != null )
		{
			curso = ConvertBeans.toCurso(cursoDTO);
		}
		
		List< AlunoDemanda > busca = AlunoDemanda.findDisciplinasBy(getInstituicaoEnsinoUser(), nome, campus, curso,
				0, numTotalDisciplinas, null);
		
		int disDemandaP1 = 0;
		int disAtendidosP1 = 0;
		int disNaoAtendidosP1 = 0;
		int disAtendidosP2 = 0;
		int count = 1;
		
		for ( AlunoDemanda disciplinas : busca )
		{
			List < AlunoDemanda > alunoDemanda = AlunoDemanda.findByDisciplinaAndCampus(getInstituicaoEnsinoUser(),
					disciplinas.getDemanda().getDisciplina(), disciplinas.getDemanda().getOferta().getCampus());

			for ( AlunoDemanda demandas : alunoDemanda )
			{
				if ( demandas.getDemanda().ocupaGrade() )
				{
					if ( demandas.getPrioridade() == 1 )
					{
						disDemandaP1++;
						if ( demandas.getAtendido() )
						{
							disAtendidosP1++;
						}
						else
						{
							disNaoAtendidosP1++;
						}
					}
					else
					{
						if ( demandas.getAtendido() )
						{
							disAtendidosP2++;
						}
					}
				}
				if (disDemandaP1 > count*1000) {
					System.out.println( count*1000 + " Disciplinas processadas");
					count++;
				}
			}
		}
		System.out.println("Soma demanda P1: " + disDemandaP1);
		System.out.println("Soma disciplinas atendidas P1: " + disAtendidosP1);
		System.out.println("Soma disciplinas nao atendidas P1: " + disNaoAtendidosP1);
		System.out.println("Soma disciplinas atendidas P2: " + disAtendidosP2);
	}
}
