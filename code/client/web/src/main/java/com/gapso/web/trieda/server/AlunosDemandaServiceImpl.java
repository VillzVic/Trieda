package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.transaction.annotation.Transactional;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Aluno;
import com.gapso.trieda.domain.AlunoDemanda;
import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AlunoDemandaDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.ResumoFaixaDemandaDTO;
import com.gapso.web.trieda.shared.dtos.ResumoMatriculaDTO;
import com.gapso.web.trieda.shared.services.AlunosDemandaService;
import com.gapso.web.trieda.shared.util.TriedaUtil;
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
		
		// Conta numero total de matriculas (para ser mostrado na paginacao)
		int numTotalDisciplinas = AlunoDemanda.countDisciplinas( getInstituicaoEnsinoUser(), codigo, campus, curso );
		
		// Busca as disciplinas de acordo com a paginacao (offset e limit). No caso da exportacao excel o limite é o total de disciplinas.
		List< AlunoDemanda > busca = AlunoDemanda.findDisciplinasBy( getInstituicaoEnsinoUser(), codigo, campus, curso,
				config.getOffset(), config.getLimit(), orderBy );
		
		// Se a busca tiver um tamanho muito grande (por exemplo no caso da exportacao do excel). 
		// Faz um pre-processamento antes para melhorar o desempenho.
		Map< Long, List<AlunoDemanda> > demandaMapAluno = new HashMap< Long, List<AlunoDemanda> >();
		if (config.getLimit() > 100) 
		{
			List< AlunoDemanda > totalAlunoDemanda = AlunoDemanda.findAll( getInstituicaoEnsinoUser() );
			for ( AlunoDemanda disciplinas : busca )
			{
				List<AlunoDemanda> demandas = new ArrayList<AlunoDemanda>();
				for ( AlunoDemanda alunoDemanda : totalAlunoDemanda )
				{
					if ( alunoDemanda.getDemanda().getDisciplina().getId() == disciplinas.getDemanda().getDisciplina().getId() 
							&& alunoDemanda.getDemanda().getOferta().getCampus().getId() == disciplinas.getDemanda().getOferta().getCampus().getId() )
					{
						demandas.add(alunoDemanda);
					}
				}
				long key = 31*(31 + disciplinas.getDemanda().getDisciplina().getId()) + disciplinas.getDemanda().getOferta().getCampus().getId();
				demandaMapAluno.put( key, demandas );
			}
		}
		
		List < AlunoDemanda > alunoDemanda;
		for ( AlunoDemanda disciplinas : busca )
		{
			if (config.getLimit() > 100)
			{
				alunoDemanda = demandaMapAluno.get( 31*(31 + disciplinas.getDemanda().getDisciplina().getId()) +
						disciplinas.getDemanda().getOferta().getCampus().getId() );
			}
			else 
			{
				alunoDemanda = AlunoDemanda.findByDisciplinaAndCampus(getInstituicaoEnsinoUser(),
						disciplinas.getDemanda().getDisciplina(), disciplinas.getDemanda().getOferta().getCampus());
			}
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
		
		// Conta numero total de matriculas (para ser mostrado na paginacao)
		int numTotalMatriculas = AlunoDemanda.countMatriculas(getInstituicaoEnsinoUser(), aluno, matricula, campus, curso);
		
		// Busca as matriculas de acordo com a paginacao (offset e limit). No caso da exportacao excel o limite é o total de matriculas.
		List< AlunoDemanda > busca = AlunoDemanda.findMatriculasBy(getInstituicaoEnsinoUser(), aluno, matricula, campus, curso,
				config.getOffset(), config.getLimit(), orderBy);
		
		// Se a busca tiver um tamanho muito grande (por exemplo no caso da exportacao do excel). 
		// Faz um pre-processamento antes para melhorar o desempenho.
		Map< Aluno, List<AlunoDemanda> > demandaMapAluno = new HashMap< Aluno, List<AlunoDemanda> >();
		if (config.getLimit() > 100) 
		{
			List< AlunoDemanda > totalAlunoDemanda = AlunoDemanda.findAll(getInstituicaoEnsinoUser());
			for (AlunoDemanda alunos : busca) 
			{
				List<AlunoDemanda> demandas = new ArrayList<AlunoDemanda>();
				for (AlunoDemanda alunoDemanda : totalAlunoDemanda)
				{
					if ( alunoDemanda.getAluno().getId() == alunos.getAluno().getId() )
					{
						demandas.add(alunoDemanda);
					}
				}
				demandaMapAluno.put(alunos.getAluno(), demandas);
			}
		}
		
		List < AlunoDemanda > alunoDemanda;
		for ( AlunoDemanda alunos : busca )
		{
			if (config.getLimit() > 100)
			{
				alunoDemanda = demandaMapAluno.get(alunos.getAluno());
			}
			else 
			{
				alunoDemanda = AlunoDemanda.findByAluno(getInstituicaoEnsinoUser(), alunos.getAluno());
			}
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
	
	@Override
	public List< ResumoFaixaDemandaDTO > getResumoFaixaDemandaList( CampusDTO campusDTO ) {
		List< ResumoMatriculaDTO > disciplinas = new ArrayList< ResumoMatriculaDTO >();
		
		List< ResumoFaixaDemandaDTO > result = new ArrayList< ResumoFaixaDemandaDTO >();
		
		boolean ehTatico = false;
		
		int numLinhas = 7;
		
		Campus campus = null;
		
		if ( campusDTO != null )
		{
			campus = ConvertBeans.toCampus(campusDTO);
			if ( campus.isOtimizado(getInstituicaoEnsinoUser()) )
			{
				ehTatico = campus.isOtimizadoTatico(getInstituicaoEnsinoUser());
			}
		}
		
		PagingLoadConfig todasPaginas = new BasePagingLoadConfig( 0, AlunoDemanda.countDisciplinas(getInstituicaoEnsinoUser(), null, campus, null) );
		
		disciplinas = getResumoAtendimentosDisciplinaList( null, campusDTO, null, todasPaginas ).getData();
		
		Map< String, Disciplina > codigoMapDisciplina = new HashMap< String, Disciplina >();
		List< Disciplina > totalDisciplinas = Disciplina.findAll( getInstituicaoEnsinoUser() );
		for ( Disciplina disciplina : totalDisciplinas ) 
		{
			codigoMapDisciplina.put( disciplina.getCodigo(), disciplina );
		}
		
		int[] demandaP1 = {0, 0, 0, 0, 0, 0, 0};
		int[] atendimentoP1 = {0, 0, 0, 0, 0, 0, 0};
		int[] atendimentoSoma = {0, 0, 0, 0, 0, 0, 0};
		int[] turmasAbertas = {0, 0, 0, 0, 0, 0, 0};
		int[] creditosPagos = {0, 0, 0, 0, 0, 0, 0};
		double[] mediaTurma = {0, 0, 0, 0, 0, 0, 0};
		int[] demandaP1Acum = {0, 0, 0, 0, 0, 0, 0};
		int[] atendimentoSomaAcum = {0, 0, 0, 0, 0, 0, 0};
		
		
		List< Disciplina > disciplinas100 = new ArrayList< Disciplina >();
		List< Disciplina > disciplinas75 = new ArrayList< Disciplina >();
		List< Disciplina > disciplinas50 = new ArrayList< Disciplina >();
		List< Disciplina > disciplinas30 = new ArrayList< Disciplina >();
		List< Disciplina > disciplinas20 = new ArrayList< Disciplina >();
		List< Disciplina > disciplinas10 = new ArrayList< Disciplina >();
		List< Disciplina > disciplinas0 = new ArrayList< Disciplina >();
		for ( ResumoMatriculaDTO disciplina : disciplinas )
		{
			if ( disciplina.getDisDemandaP1() > 100 )
			{
				demandaP1[0] += disciplina.getDisDemandaP1();
				atendimentoP1[0] += disciplina.getDisAtendidosP1();
				atendimentoSoma[0] += disciplina.getDisAtendidosP1() + disciplina.getDisAtendidosP2();
				disciplinas100.add( codigoMapDisciplina.get( disciplina.getCodDisciplina() ) );
			}
			else if (76 <= disciplina.getDisDemandaP1() && disciplina.getDisDemandaP1() <= 100 )
			{
				demandaP1[1] += disciplina.getDisDemandaP1();
				atendimentoP1[1] += disciplina.getDisAtendidosP1();
				atendimentoSoma[1] += disciplina.getDisAtendidosP1() + disciplina.getDisAtendidosP2();
				disciplinas75.add( codigoMapDisciplina.get( disciplina.getCodDisciplina() ) );
			}
			else if (51 <= disciplina.getDisDemandaP1() && disciplina.getDisDemandaP1() <= 75 )
			{
				demandaP1[2] += disciplina.getDisDemandaP1();
				atendimentoP1[2] += disciplina.getDisAtendidosP1();
				atendimentoSoma[2] += disciplina.getDisAtendidosP1() + disciplina.getDisAtendidosP2();
				disciplinas50.add( codigoMapDisciplina.get( disciplina.getCodDisciplina() ) );
			}
			else if (31 <= disciplina.getDisDemandaP1() && disciplina.getDisDemandaP1() <= 50 )
			{
				demandaP1[3] += disciplina.getDisDemandaP1();
				atendimentoP1[3] += disciplina.getDisAtendidosP1();
				atendimentoSoma[3] += disciplina.getDisAtendidosP1() + disciplina.getDisAtendidosP2();
				disciplinas30.add( codigoMapDisciplina.get( disciplina.getCodDisciplina() ) );
			}
			else if (21 <= disciplina.getDisDemandaP1() && disciplina.getDisDemandaP1() <= 30 )
			{
				demandaP1[4] += disciplina.getDisDemandaP1();
				atendimentoP1[4] += disciplina.getDisAtendidosP1();
				atendimentoSoma[4] += disciplina.getDisAtendidosP1() + disciplina.getDisAtendidosP2();
				disciplinas20.add( codigoMapDisciplina.get( disciplina.getCodDisciplina() ) );
			}
			else if (10 <= disciplina.getDisDemandaP1() && disciplina.getDisDemandaP1() <= 20 )
			{
				demandaP1[5] += disciplina.getDisDemandaP1();
				atendimentoP1[5] += disciplina.getDisAtendidosP1();
				atendimentoSoma[5] += disciplina.getDisAtendidosP1() + disciplina.getDisAtendidosP2();
				disciplinas10.add( codigoMapDisciplina.get( disciplina.getCodDisciplina() ) );
			}
			else if ( disciplina.getDisDemandaP1() < 10 )
			{
				demandaP1[6] += disciplina.getDisDemandaP1();
				atendimentoP1[6] += disciplina.getDisAtendidosP1();
				atendimentoSoma[6] += disciplina.getDisAtendidosP1() + disciplina.getDisAtendidosP2();
				disciplinas0.add( codigoMapDisciplina.get( disciplina.getCodDisciplina() ) );
			}
		}
		
		for ( int i = 0; i < numLinhas; i++ )
		{
			ResumoFaixaDemandaDTO resumoFaixaDemandaDTO = new ResumoFaixaDemandaDTO();
			
			double atendimentoP1Percent = ((double)atendimentoP1[i]) / demandaP1[i];
			double atendimentoSomaPercent = ((double)atendimentoSoma[i]) / demandaP1[i];
			
			switch (i)
			{
				case 0:
					resumoFaixaDemandaDTO.setDemandaDisc("Acima de 100 alunos");
					if ( ehTatico )
					{
						creditosPagos[i] = AtendimentoTatico.countCreditosByTurmas( getInstituicaoEnsinoUser(), campus, disciplinas100 );
						turmasAbertas[i] = AtendimentoTatico.countTurmaByDisciplinas( getInstituicaoEnsinoUser(), campus, disciplinas100 );
					}
					else
					{
						creditosPagos[i] = AtendimentoOperacional.countCreditosByTurmas( getInstituicaoEnsinoUser(), campus, disciplinas100 );
						turmasAbertas[i] = AtendimentoOperacional.countTurmaByDisciplinas( getInstituicaoEnsinoUser(), campus, disciplinas100 );
					}
					mediaTurma[i] = ( (double) atendimentoSoma[i] ) / turmasAbertas[i];
					demandaP1Acum[i] = demandaP1[i];
					atendimentoSomaAcum[i] = atendimentoSoma[i];
					break;
				case 1:
					resumoFaixaDemandaDTO.setDemandaDisc("Entre 76 e 100 alunos");
					if ( ehTatico )
					{
						creditosPagos[i] = AtendimentoTatico.countCreditosByTurmas( getInstituicaoEnsinoUser(), campus, disciplinas75 );
						turmasAbertas[i] = AtendimentoTatico.countTurmaByDisciplinas( getInstituicaoEnsinoUser(), campus, disciplinas75 );
					}
					else
					{
						creditosPagos[i] = AtendimentoOperacional.countCreditosByTurmas( getInstituicaoEnsinoUser(), campus, disciplinas75 );
						turmasAbertas[i] = AtendimentoOperacional.countTurmaByDisciplinas( getInstituicaoEnsinoUser(), campus, disciplinas75 );
					}
					mediaTurma[i] = ( (double) atendimentoSoma[i] ) / turmasAbertas[i];
					demandaP1Acum[i] = demandaP1[i] + demandaP1Acum[i-1];
					atendimentoSomaAcum[i] = atendimentoSoma[i] + atendimentoSomaAcum[i-1];
					break;
				case 2:
					resumoFaixaDemandaDTO.setDemandaDisc("Entre 51 e 75 alunos");
					if ( ehTatico )
					{
						creditosPagos[i] = AtendimentoTatico.countCreditosByTurmas( getInstituicaoEnsinoUser(), campus, disciplinas50 );
						turmasAbertas[i] = AtendimentoTatico.countTurmaByDisciplinas( getInstituicaoEnsinoUser(), campus, disciplinas50 );
					}
					else
					{
						creditosPagos[i] = AtendimentoOperacional.countCreditosByTurmas( getInstituicaoEnsinoUser(), campus, disciplinas50 );
						turmasAbertas[i] = AtendimentoOperacional.countTurmaByDisciplinas( getInstituicaoEnsinoUser(), campus, disciplinas50 );
					}
					mediaTurma[i] = ( (double) atendimentoSoma[i] ) / turmasAbertas[i];
					demandaP1Acum[i] = demandaP1[i] + demandaP1Acum[i-1];
					atendimentoSomaAcum[i] = atendimentoSoma[i] + atendimentoSomaAcum[i-1];
					break;
				case 3:
					resumoFaixaDemandaDTO.setDemandaDisc("Entre 31 e 50 alunos");
					if ( ehTatico )
					{
						creditosPagos[i] = AtendimentoTatico.countCreditosByTurmas( getInstituicaoEnsinoUser(), campus, disciplinas30 );
						turmasAbertas[i] = AtendimentoTatico.countTurmaByDisciplinas( getInstituicaoEnsinoUser(), campus, disciplinas30 );
					}
					else
					{
						creditosPagos[i] = AtendimentoOperacional.countCreditosByTurmas( getInstituicaoEnsinoUser(), campus, disciplinas30 );
						turmasAbertas[i] = AtendimentoOperacional.countTurmaByDisciplinas( getInstituicaoEnsinoUser(), campus, disciplinas30 );
					}
					mediaTurma[i] = ( (double) atendimentoSoma[i] ) / turmasAbertas[i];
					demandaP1Acum[i] = demandaP1[i] + demandaP1Acum[i-1];
					atendimentoSomaAcum[i] = atendimentoSoma[i] + atendimentoSomaAcum[i-1];
					break;
				case 4:
					resumoFaixaDemandaDTO.setDemandaDisc("Entre 21 e 30 alunos");
					if ( ehTatico )
					{
						creditosPagos[i] = AtendimentoTatico.countCreditosByTurmas( getInstituicaoEnsinoUser(), campus, disciplinas20 );
						turmasAbertas[i] = AtendimentoTatico.countTurmaByDisciplinas( getInstituicaoEnsinoUser(), campus, disciplinas20 );
					}
					else
					{
						creditosPagos[i] = AtendimentoOperacional.countCreditosByTurmas( getInstituicaoEnsinoUser(), campus, disciplinas20 );
						turmasAbertas[i] = AtendimentoOperacional.countTurmaByDisciplinas( getInstituicaoEnsinoUser(), campus, disciplinas20 );
					}
					mediaTurma[i] = ( (double) atendimentoSoma[i] ) / turmasAbertas[i];
					demandaP1Acum[i] = demandaP1[i] + demandaP1Acum[i-1];
					atendimentoSomaAcum[i] = atendimentoSoma[i] + atendimentoSomaAcum[i-1];
					break;
				case 5:
					resumoFaixaDemandaDTO.setDemandaDisc("Entre 10 e 20 alunos");
					if ( ehTatico )
					{
						creditosPagos[i] = AtendimentoTatico.countCreditosByTurmas( getInstituicaoEnsinoUser(), campus, disciplinas10 );
						turmasAbertas[i] = AtendimentoTatico.countTurmaByDisciplinas( getInstituicaoEnsinoUser(), campus, disciplinas10 );
					}
					else
					{
						creditosPagos[i] = AtendimentoOperacional.countCreditosByTurmas( getInstituicaoEnsinoUser(), campus, disciplinas10 );
						turmasAbertas[i] = AtendimentoOperacional.countTurmaByDisciplinas( getInstituicaoEnsinoUser(), campus, disciplinas10 );
					}
					mediaTurma[i] = ( (double) atendimentoSoma[i] ) / turmasAbertas[i];
					demandaP1Acum[i] = demandaP1[i] + demandaP1Acum[i-1];
					atendimentoSomaAcum[i] = atendimentoSoma[i] + atendimentoSomaAcum[i-1];
					break;
				case 6:
					resumoFaixaDemandaDTO.setDemandaDisc("Abaixo de 10 alunos");
					if ( ehTatico )
					{
						creditosPagos[i] = AtendimentoTatico.countCreditosByTurmas( getInstituicaoEnsinoUser(), campus, disciplinas0 );
						turmasAbertas[i] = AtendimentoTatico.countTurmaByDisciplinas( getInstituicaoEnsinoUser(), campus, disciplinas0 );
					}
					else
					{
						creditosPagos[i] = AtendimentoOperacional.countCreditosByTurmas( getInstituicaoEnsinoUser(), campus, disciplinas0 );
						turmasAbertas[i] = AtendimentoOperacional.countTurmaByDisciplinas( getInstituicaoEnsinoUser(), campus, disciplinas0 );
					}
					mediaTurma[i] = ( (double) atendimentoSoma[i] ) / turmasAbertas[i];
					demandaP1Acum[i] = demandaP1[i] + demandaP1Acum[i-1];
					atendimentoSomaAcum[i] = atendimentoSoma[i] + atendimentoSomaAcum[i-1];
					break;
			}
			
			double atendimentoSomaAcumPercent = ( (double)atendimentoSomaAcum[i] ) / demandaP1Acum[i];
			
			resumoFaixaDemandaDTO.setCampusNome( campus.getNome() );
			resumoFaixaDemandaDTO.setDemandaP1( demandaP1[i] );
			resumoFaixaDemandaDTO.setAtendimentoP1( atendimentoP1[i] );
			resumoFaixaDemandaDTO.setAtendimentoPercentP1( TriedaUtil.round(atendimentoP1Percent*100.0,2)+"%" );
			resumoFaixaDemandaDTO.setAtendimentoSoma(atendimentoSoma[i]);
			resumoFaixaDemandaDTO.setAtendimentoSomaPercent( TriedaUtil.round(atendimentoSomaPercent*100.0,2)+"%" );
			resumoFaixaDemandaDTO.setTurmasAbertas( turmasAbertas[i] );
			resumoFaixaDemandaDTO.setMediaTurma( TriedaUtil.round(mediaTurma[i], 2) );
			resumoFaixaDemandaDTO.setCreditosPagos( creditosPagos[i] );
			resumoFaixaDemandaDTO.setDemandaAcumP1( demandaP1Acum[i] );
			resumoFaixaDemandaDTO.setAtendimentoSomaAcum( atendimentoSomaAcum[i] );
			resumoFaixaDemandaDTO.setAtendimentoSomaAcumPercent( TriedaUtil.round(atendimentoSomaAcumPercent*100.0,2)+"%" );
			
			result.add( resumoFaixaDemandaDTO );
		}
		
		
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
