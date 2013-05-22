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
import com.gapso.trieda.domain.AtendimentoFaixaDemanda;
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
import com.gapso.web.trieda.shared.dtos.AtendimentoFaixaDemandaDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
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
			for (AlunoDemanda alunoDemanda : totalAlunoDemanda) {
				long key = 31*(31 + alunoDemanda.getDemanda().getDisciplina().getId()) + alunoDemanda.getDemanda().getOferta().getCampus().getId();
				if (demandaMapAluno.get(key) == null) {
					List<AlunoDemanda> demandas = new ArrayList<AlunoDemanda>();
					demandas.add(alunoDemanda);
					demandaMapAluno.put(key, demandas);
				}
				else {
					demandaMapAluno.get(key).add(alunoDemanda);
				}
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
		
		// Conta numero total de matriculas (para ser mostrado na paginacao
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
			for (AlunoDemanda alunoDemanda : totalAlunoDemanda) {
				if (demandaMapAluno.get(alunoDemanda.getAluno()) == null) {
					List<AlunoDemanda> demandas = new ArrayList<AlunoDemanda>();
					demandas.add(alunoDemanda);
					demandaMapAluno.put(alunoDemanda.getAluno(), demandas);
				}
				else {
					demandaMapAluno.get(alunoDemanda.getAluno()).add(alunoDemanda);
				}
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
	public List< AtendimentoFaixaDemandaDTO > getResumoFaixaDemandaList( CampusDTO campusDTO, List<ParDTO<Integer, Integer>> faixas) {
		Campus campus = null;
		List< AtendimentoFaixaDemandaDTO > result = new ArrayList< AtendimentoFaixaDemandaDTO >();
		
		if ( campusDTO != null )
		{
			campus = ConvertBeans.toCampus(campusDTO);
		}
		
		if( AtendimentoFaixaDemanda.isSalvo(getInstituicaoEnsinoUser(), campus) && faixas == null) {
			result = ConvertBeans.toListAtendimentoFaixaDemandaDTO( AtendimentoFaixaDemanda.findByCampus(getInstituicaoEnsinoUser(), campus) );
		}
		else {
			AtendimentoFaixaDemanda.deleteAllFromCampus(campus);
			for (AtendimentoFaixaDemanda atendimento : createResumoFaixaDemandaList(campusDTO, faixas)) {
				result.add(ConvertBeans.toAtendimentoFaixaDemandaDTO(atendimento));
				atendimento.persist();
			}
			
		}
		
		return result;
	}
	
	public List< AtendimentoFaixaDemanda > createResumoFaixaDemandaList( CampusDTO campusDTO, List<ParDTO<Integer, Integer>> faixas ) {
		List< ResumoMatriculaDTO > disciplinas = new ArrayList< ResumoMatriculaDTO >();
		
		List< AtendimentoFaixaDemanda > result = new ArrayList< AtendimentoFaixaDemanda >();
		
		boolean ehTatico = false;
		
		Campus campus = null;
		
		if ( campusDTO != null )
		{
			campus = ConvertBeans.toCampus(campusDTO);
			if ( campus.isOtimizado(getInstituicaoEnsinoUser()) )
			{
				ehTatico = campus.isOtimizadoTatico(getInstituicaoEnsinoUser());
			}
		}
		
		if ( faixas == null ) {
			faixas = new ArrayList<ParDTO<Integer, Integer>>();
			faixas.add(ParDTO.create(100, 0));
			faixas.add(ParDTO.create(100, 76));
			faixas.add(ParDTO.create(75, 51));
			faixas.add(ParDTO.create(50, 31));
			faixas.add(ParDTO.create(30, 14));
			faixas.add(ParDTO.create(0, 14));
		}
		
		int numFaixas = faixas.size();
		
		PagingLoadConfig todasPaginas = new BasePagingLoadConfig( 0, AlunoDemanda.countDisciplinas(getInstituicaoEnsinoUser(), null, campus, null) );
		
		disciplinas = getResumoAtendimentosDisciplinaList( null, campusDTO, null, todasPaginas ).getData();
		
		Map< String, Disciplina > codigoMapDisciplina = new HashMap< String, Disciplina >();
		List< Disciplina > totalDisciplinas = Disciplina.findAll( getInstituicaoEnsinoUser() );
		for ( Disciplina disciplina : totalDisciplinas ) 
		{
			codigoMapDisciplina.put( disciplina.getCodigo(), disciplina );
		}
		
		int demandaP1[] = new int[numFaixas];
		int atendimentoP1[] = new int[numFaixas];
		int atendimentoSoma[] = new int[numFaixas];
		int turmasAbertas[] = new int[numFaixas];
		int creditosPagos[] = new int[numFaixas];
		double mediaTurma[] = new double[numFaixas];
		int demandaP1Acum[] = new int[numFaixas];
		int atendimentoSomaAcum[] = new int[numFaixas];
		double receita[] = new double[numFaixas];
		
		List<List<Disciplina>> listDisciplinas = new ArrayList<List<Disciplina>>();
		
		//Inicializando lsitas
		for ( int i = 0; i < numFaixas; i++ )
		{
			demandaP1[i] = 0;
			atendimentoP1[i] = 0;
			atendimentoSoma[i] = 0;
			turmasAbertas[i] = 0;
			creditosPagos[i] = 0;
			mediaTurma[i] = 0;
			demandaP1Acum[i] = 0;
			atendimentoSomaAcum[i] = 0;
			receita[i] = 0;
			listDisciplinas.add(new ArrayList<Disciplina>());
		}

		for ( ResumoMatriculaDTO disciplina : disciplinas )
		{
			int indiceFaixa = 0;
			for ( ParDTO<Integer, Integer> faixa : faixas ) {
				int faixaSup = faixa.getPrimeiro();
				int faixaInf= faixa.getSegundo();
				if ( faixa.getSegundo() == 0 ) {
					if ( disciplina.getDisDemandaP1() > faixaSup )
					{
						demandaP1[indiceFaixa] += disciplina.getDisDemandaP1();
						atendimentoP1[indiceFaixa] += disciplina.getDisAtendidosP1();
						atendimentoSoma[indiceFaixa] += disciplina.getDisAtendidosP1() + disciplina.getDisAtendidosP2();
						listDisciplinas.get(indiceFaixa).add( codigoMapDisciplina.get( disciplina.getCodDisciplina() ) );
					}
				}
				else if ( faixa.getPrimeiro() == 0 ) {
					if ( disciplina.getDisDemandaP1() < faixaInf )
					{
						demandaP1[indiceFaixa] += disciplina.getDisDemandaP1();
						atendimentoP1[indiceFaixa] += disciplina.getDisAtendidosP1();
						atendimentoSoma[indiceFaixa] += disciplina.getDisAtendidosP1() + disciplina.getDisAtendidosP2();
						listDisciplinas.get(indiceFaixa).add( codigoMapDisciplina.get( disciplina.getCodDisciplina() ) );
					}
				}
				else if ( faixaInf <= disciplina.getDisDemandaP1() && disciplina.getDisDemandaP1() <= faixaSup )
				{
					demandaP1[indiceFaixa] += disciplina.getDisDemandaP1();
					atendimentoP1[indiceFaixa] += disciplina.getDisAtendidosP1();
					atendimentoSoma[indiceFaixa] += disciplina.getDisAtendidosP1() + disciplina.getDisAtendidosP2();
					listDisciplinas.get(indiceFaixa).add( codigoMapDisciplina.get( disciplina.getCodDisciplina() ) );
				}
				indiceFaixa++;
			}
		}
		
		double custoDocenteSemanalAcum = 0.0;
		double receitaAcum = 0.0;
		for ( int i = 0; i < numFaixas; i++ )
		{
			AtendimentoFaixaDemanda resumoFaixaDemanda = new AtendimentoFaixaDemanda();
			
			double atendimentoP1Percent = ((double)atendimentoP1[i]) / demandaP1[i];
			double atendimentoSomaPercent = ((double)atendimentoSoma[i]) / demandaP1[i];
			
			resumoFaixaDemanda.setFaixaSuperior(faixas.get(i).getPrimeiro());
			resumoFaixaDemanda.setFaixaInferior(faixas.get(i).getSegundo());
			if ( ehTatico )
			{
				creditosPagos[i] = AtendimentoTatico.countCreditosByTurmas( getInstituicaoEnsinoUser(), campus, listDisciplinas.get(i) );
				turmasAbertas[i] = AtendimentoTatico.countTurmaByDisciplinas( getInstituicaoEnsinoUser(), campus, listDisciplinas.get(i) );
				receita[i] = AtendimentoTatico.calcReceita( getInstituicaoEnsinoUser(), campus, listDisciplinas.get(i) );
			}
			else
			{
				creditosPagos[i] = AtendimentoOperacional.countCreditosByTurmas( getInstituicaoEnsinoUser(), campus, listDisciplinas.get(i) );
				turmasAbertas[i] = AtendimentoOperacional.countTurmaByDisciplinas( getInstituicaoEnsinoUser(), campus, listDisciplinas.get(i) );
				receita[i] = AtendimentoOperacional.calcReceita( getInstituicaoEnsinoUser(), campus, listDisciplinas.get(i) );
			}
			mediaTurma[i] = ( (double) atendimentoSoma[i] ) / turmasAbertas[i];
			demandaP1Acum[i] = (i == 0 ? demandaP1[i] : demandaP1[i] + demandaP1Acum[i-1]);
			atendimentoSomaAcum[i] = (i == 0 ? atendimentoSoma[i] : atendimentoSoma[i] + atendimentoSomaAcum[i-1]);

			double atendimentoSomaAcumPercent = ( (double)atendimentoSomaAcum[i] ) / demandaP1Acum[i];
			double custoDocenteSemanal = (double)creditosPagos[i] * campus.getValorCredito() * 4.5 * 6;
			double custoDocentePorReceitaPercent = custoDocenteSemanal / receita[i];
			custoDocenteSemanalAcum += custoDocenteSemanal;
			receitaAcum += receita[i];
			double custoDocentePorReceitaAcumPercent = custoDocenteSemanalAcum / receitaAcum;
			
			resumoFaixaDemanda.setCampus( campus );
			resumoFaixaDemanda.setDemandaP1( demandaP1[i] );
			resumoFaixaDemanda.setAtendimentoP1( atendimentoP1[i] );
			resumoFaixaDemanda.setAtendimentoPercentP1( atendimentoP1Percent );
			resumoFaixaDemanda.setAtendimentoSoma(atendimentoSoma[i]);
			resumoFaixaDemanda.setAtendimentoSomaPercent( atendimentoSomaPercent );
			resumoFaixaDemanda.setTurmasAbertas( turmasAbertas[i] );
			resumoFaixaDemanda.setMediaTurma( TriedaUtil.round(mediaTurma[i], 2) );
			resumoFaixaDemanda.setCreditosPagos( creditosPagos[i] );
			resumoFaixaDemanda.setDemandaAcumP1( demandaP1Acum[i] );
			resumoFaixaDemanda.setAtendimentoSomaAcum( atendimentoSomaAcum[i] );
			resumoFaixaDemanda.setAtendimentoAcumPercent( atendimentoSomaAcumPercent );
			resumoFaixaDemanda.setReceitaSemanal(TriedaUtil.round(receita[i],2));
			resumoFaixaDemanda.setCustoDocenteSemanal( TriedaUtil.round(custoDocenteSemanal,2) );
			resumoFaixaDemanda.setCustoDocentePercent( TriedaUtil.round(custoDocentePorReceitaPercent,2) );
			resumoFaixaDemanda.setReceitaAcumulada(TriedaUtil.round(receitaAcum,2));
			resumoFaixaDemanda.setCustoDocenteAcumulado( TriedaUtil.round(custoDocenteSemanalAcum,2) );
			resumoFaixaDemanda.setCustoDocentePorReceitaAcumuladoPercent( custoDocentePorReceitaAcumPercent );
			
			result.add( resumoFaixaDemanda );
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
