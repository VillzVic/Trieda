package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.HashSet;
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
import com.gapso.trieda.domain.Aluno;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.AlunoDTO;
import com.gapso.web.trieda.shared.dtos.AlunoDisciplinaCursadaDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaRequisitoDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.services.CurriculosService;
import com.gapso.web.trieda.shared.util.view.TriedaException;

@Transactional
public class CurriculosServiceImpl
	extends RemoteService
	implements CurriculosService
{
	private static final long serialVersionUID = 5250776996542788849L;
	
	/**
	 * @see com.gapso.web.trieda.shared.services.CurriculosService#saveDisciplina(com.gapso.web.trieda.shared.dtos.CurriculoDTO, com.gapso.web.trieda.shared.dtos.CurriculoDisciplinaDTO)
	 */
	@Override
	public void saveDisciplina(CurriculoDTO curriculoDTO, CurriculoDisciplinaDTO curriculoDisciplinaDTO) throws TriedaException {
		CurriculoDisciplina curriculoDisciplina = ConvertBeans.toCurriculoDisciplina(curriculoDisciplinaDTO);
		Curriculo curriculo = Curriculo.find(curriculoDTO.getId(),getInstituicaoEnsinoUser());
		
		// obtém as semanas letivas já associadas com a disciplina
		Set<SemanaLetiva> semanasLetivas = curriculoDisciplina.getDisciplina().getSemanasLetivas();
		
		// verifica se a disciplina é compatível com a matriz curricular em termos de semanas letivas
		if (semanasLetivas.isEmpty() || semanasLetivas.contains(curriculo.getSemanaLetiva())) {
			curriculoDisciplina.setCurriculo(curriculo);
			curriculoDisciplina.persist();
		} else {
			String msg = "A disciplina [" + curriculoDisciplina.getDisciplina().getCodigo() + "] já tem relação com a(s) semana(s) letiva(s) [";
			for (SemanaLetiva semanaLetiva : semanasLetivas) {
				msg += semanaLetiva.getCodigo() + ", ";
			}
			msg += "], por isso, não será possível associá-la com o currículo [" + curriculo.getCodigo() + "] que está associado com a semana letiva [" + curriculo.getSemanaLetiva().getCodigo() + "].";
			
			throw new TriedaException(msg);
		}
	}

	@Override
	public void saveDisciplinaPreRequisito(CenarioDTO cenarioDTO, DisciplinaRequisitoDTO disciplinaRequisitoDTO, DisciplinaDTO disciplinaDTO) {
		CurriculoDisciplina curriculoDisciplina = CurriculoDisciplina.findByCurriculoAndPeriodoAndDisciplina(getInstituicaoEnsinoUser(),
				ConvertBeans.toCenario(cenarioDTO), Curriculo.find(disciplinaRequisitoDTO.getCurriculoId(), getInstituicaoEnsinoUser()),
				disciplinaRequisitoDTO.getPeriodo(), Disciplina.find(disciplinaRequisitoDTO.getDisciplinaId(), getInstituicaoEnsinoUser()));
		
		Disciplina disciplinaRequisito = Disciplina.find(disciplinaDTO.getId(), getInstituicaoEnsinoUser());
		
		if (curriculoDisciplina != null) {
			curriculoDisciplina.getPreRequisitos().add(disciplinaRequisito);
			curriculoDisciplina.merge();
		}
	}
	
	@Override
	public void saveDisciplinaCoRequisito(CenarioDTO cenarioDTO, DisciplinaRequisitoDTO disciplinaRequisitoDTO, DisciplinaDTO disciplinaDTO) {
		CurriculoDisciplina curriculoDisciplina = CurriculoDisciplina.findByCurriculoAndPeriodoAndDisciplina(getInstituicaoEnsinoUser(),
				ConvertBeans.toCenario(cenarioDTO), Curriculo.find(disciplinaRequisitoDTO.getCurriculoId(), getInstituicaoEnsinoUser()),
				disciplinaRequisitoDTO.getPeriodo(), Disciplina.find(disciplinaRequisitoDTO.getDisciplinaId(), getInstituicaoEnsinoUser()));
		
		Disciplina disciplinaRequisito = Disciplina.find(disciplinaDTO.getId(), getInstituicaoEnsinoUser());
		
		if (curriculoDisciplina != null) {
			curriculoDisciplina.getCoRequisitos().add(disciplinaRequisito);
			curriculoDisciplina.merge();
		}
	}
	
	@Override
	public void saveAlunoDisciplinaCursada(CenarioDTO cenarioDTO, AlunoDTO alunoDTO, List<CurriculoDisciplinaDTO> curriculosDisciplinasDTO) {
		Aluno aluno = Aluno.find(alunoDTO.getId(), getInstituicaoEnsinoUser());
		
		for ( CurriculoDisciplinaDTO curriculoDisciplinaDTO : curriculosDisciplinasDTO )
		{
			aluno.getCursou().add(CurriculoDisciplina.find(curriculoDisciplinaDTO.getId(), getInstituicaoEnsinoUser()));
		}
		
		aluno.merge();
	}
	
	/**
	 * @see com.gapso.web.trieda.shared.services.CurriculosService#save(com.gapso.web.trieda.shared.dtos.CurriculoDTO)
	 */
	@Override
	public void save(CurriculoDTO curriculoDTO) throws TriedaException {
		try {
			Curriculo curriculo = ConvertBeans.toCurriculo(curriculoDTO);
			
			// verifica a validade dos dados da semana letiva
			if (!curriculo.getSemanaLetiva().getHorariosAula().isEmpty()) {
				if (curriculo.getId() != null && curriculo.getId() > 0) {
					curriculo.merge();
				}
				else {
					curriculo.persist();
				}
			} else {
				throw new TriedaException("Não será possível salvar a matriz curricular em questão, pois, a semana letiva [" + curriculo.getSemanaLetiva().getCodigo() + "] ainda não tem turnos nem horários de aula.");
			}
		} catch (Exception e) {
			e.printStackTrace();
			throw new TriedaException(e);
		}
	}

	@Override
	public CurriculoDTO getCurriculo( Long id )
	{
		return ConvertBeans.toCurriculoDTO(
			Curriculo.find( id, getInstituicaoEnsinoUser() ) );
	}

	@Override
	public ListLoadResult< CurriculoDTO > getListAll( CenarioDTO cenarioDTO )
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		List< Curriculo > curriculos
			= Curriculo.findByCenario( getInstituicaoEnsinoUser(), cenario );

		List< CurriculoDTO > curriculosDTO
			= new ArrayList<CurriculoDTO>( curriculos.size() );

		for ( Curriculo curriculo : curriculos )
		{
			curriculosDTO.add(
				ConvertBeans.toCurriculoDTO( curriculo ) );
		}

		return new BaseListLoadResult< CurriculoDTO >( curriculosDTO );
	}

	@Override
	public ListLoadResult< CurriculoDTO > getListByCurso( CursoDTO cursoDTO )
	{
		Curso curso = Curso.find( cursoDTO.getId(), getInstituicaoEnsinoUser() );

		Set< Curriculo > curriculos = curso.getCurriculos();
		List< CurriculoDTO > curriculosDTO
			= new ArrayList< CurriculoDTO >( curriculos.size() );

		for ( Curriculo curriculo : curriculos )
		{
			curriculosDTO.add( ConvertBeans.toCurriculoDTO( curriculo ) );
		}

		return new BaseListLoadResult< CurriculoDTO >( curriculosDTO );
	}

	@Override
	public ListLoadResult< CurriculoDTO > getList( CenarioDTO cenarioDTO, SemanaLetivaDTO semanaLetivaDTO,
			BasePagingLoadConfig config )
	{
		CursoDTO cursoDTO = config.get( "cursoDTO" );

		return getBuscaList( cenarioDTO, cursoDTO, semanaLetivaDTO, config.get( "query" ).toString(), null, null,  config );
	}

	@Override
	public PagingLoadResult< CurriculoDTO > getBuscaList( CenarioDTO cenarioDTO, 
		CursoDTO cursoDTO, SemanaLetivaDTO semanaLetivaDTO, String codigo, String descricao, 
		String periodo, PagingLoadConfig config )
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		List< CurriculoDTO > list = new ArrayList< CurriculoDTO >();
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

		Curso curso = null;
		if ( cursoDTO != null )
		{
			curso = ConvertBeans.toCurso( cursoDTO );
		}
		
		SemanaLetiva semanaLetiva = null;
		if(semanaLetivaDTO != null){
			semanaLetiva = ConvertBeans.toSemanaLetiva(semanaLetivaDTO);
		}

		List< Curriculo > listCurriculos = Curriculo.findBy( getInstituicaoEnsinoUser(),
			cenario, curso, semanaLetiva, codigo, descricao, periodo, config.getOffset(), config.getLimit(), orderBy );

		for ( Curriculo curriculo : listCurriculos )
		{
			list.add( ConvertBeans.toCurriculoDTO( curriculo ) );
		}

		BasePagingLoadResult< CurriculoDTO > result
			= new BasePagingLoadResult< CurriculoDTO >( list );

		result.setOffset( config.getOffset() );
		result.setTotalLength( Curriculo.count(
			getInstituicaoEnsinoUser(), cenario, curso, semanaLetiva, codigo, descricao, periodo) );

		return result;
	}

	@Override
	public void remove( List< CurriculoDTO > curriculoDTOList )
	{
		for ( CurriculoDTO curriculoDTO : curriculoDTOList )
		{
			ConvertBeans.toCurriculo( curriculoDTO ).remove();
		}
	}

	@Override
	public ListLoadResult< CurriculoDisciplinaDTO > getDisciplinasList( CurriculoDTO curriculoDTO )
	{
		Curriculo curriculo = Curriculo.find(
			curriculoDTO.getId(), getInstituicaoEnsinoUser() );

		List< CurriculoDisciplinaDTO > listCurriculoDisciplinaDTO
			= new ArrayList< CurriculoDisciplinaDTO >();

		Set< CurriculoDisciplina > listCurriculoDisciplina
			= curriculo.getDisciplinas();

		for ( CurriculoDisciplina cd : listCurriculoDisciplina )
		{
			listCurriculoDisciplinaDTO.add( ConvertBeans.toCurriculoDisciplinaDTO( cd ) );
		}

		return new BaseListLoadResult< CurriculoDisciplinaDTO >( listCurriculoDisciplinaDTO );
	}

	@Override
	public List< Integer > getPeriodos( CurriculoDTO curriculoDTO, CenarioDTO cenarioDTO )
	{
		Cenario cenario;
		if (cenarioDTO == null)
		{
			cenario = Cenario.find(
					curriculoDTO.getCenarioId(), getInstituicaoEnsinoUser());
		}
		else
		{
			cenario = Cenario.find(
					cenarioDTO.getId(), getInstituicaoEnsinoUser());
		}

		if ( curriculoDTO == null )
		{
			Set<Integer> periodos = new HashSet<Integer>();
			List<CurriculoDisciplina> curriculosDisciplina = CurriculoDisciplina.findByCenario(getInstituicaoEnsinoUser(), cenario);
			for (CurriculoDisciplina curriculoDisciplina : curriculosDisciplina)
			{
				periodos.add(curriculoDisciplina.getPeriodo());
			}
			return new ArrayList<Integer>(periodos);
		}
		else
		{
			Curriculo curriculo = Curriculo.find(
					curriculoDTO.getId(), getInstituicaoEnsinoUser() );
			return curriculo.getPeriodos();
		}
	}

	@Override
	public void removeDisciplina( List< CurriculoDisciplinaDTO > curriculoDisciplinaDTOList )
	{
		for ( CurriculoDisciplinaDTO curriculoDisciplinaDTO : curriculoDisciplinaDTOList )
		{
			ConvertBeans.toCurriculoDisciplina( curriculoDisciplinaDTO ).remove();
		}
	}
	
	@Override
	public void removeDisciplinasPreRequisitos( CenarioDTO cenarioDTO, List< DisciplinaRequisitoDTO > disciplinasRequisitosDTO )
	{
		for (DisciplinaRequisitoDTO disciplinaRequisitoDTO : disciplinasRequisitosDTO) {
			CurriculoDisciplina curriculoDisciplina = CurriculoDisciplina.find(disciplinaRequisitoDTO.getCurriculoDisciplinaId(), getInstituicaoEnsinoUser());
			
			Disciplina disciplinaRequisito = Disciplina.find(disciplinaRequisitoDTO.getDisciplinaRequisitoId(),
					getInstituicaoEnsinoUser());
			
			if (curriculoDisciplina != null) {
				curriculoDisciplina.getPreRequisitos().remove(disciplinaRequisito);
				curriculoDisciplina.persist();
			}
		}
	}
	
	@Override
	public void removeAlunosDisciplinasCursadas( CenarioDTO cenarioDTO, List< AlunoDisciplinaCursadaDTO > alunosDisciplinasCursadasDTO )
	{
		for (AlunoDisciplinaCursadaDTO alunoDisciplinaCursadaDTO : alunosDisciplinasCursadasDTO) {
			CurriculoDisciplina curriculoDisciplina = CurriculoDisciplina.find(alunoDisciplinaCursadaDTO.getCurriculoDisciplinaId(), getInstituicaoEnsinoUser());
			
			Aluno cursadoPor = Aluno.find(alunoDisciplinaCursadaDTO.getAlunoId(),
					getInstituicaoEnsinoUser());
			
			if (curriculoDisciplina != null) {
				curriculoDisciplina.getCursadoPor().remove(cursadoPor);
				curriculoDisciplina.persist();
			}
		}
	}
	
	@Override
	public void removeDisciplinasCoRequisitos( CenarioDTO cenarioDTO, List< DisciplinaRequisitoDTO > disciplinasRequisitosDTO )
	{
		for (DisciplinaRequisitoDTO disciplinaRequisitoDTO : disciplinasRequisitosDTO) {
			CurriculoDisciplina curriculoDisciplina = CurriculoDisciplina.find(disciplinaRequisitoDTO.getCurriculoDisciplinaId(), getInstituicaoEnsinoUser());
			
			Disciplina disciplinaRequisito = Disciplina.find(disciplinaRequisitoDTO.getDisciplinaRequisitoId(),
					getInstituicaoEnsinoUser());
			
			if (curriculoDisciplina != null) {
				curriculoDisciplina.getCoRequisitos().remove(disciplinaRequisito);
				curriculoDisciplina.persist();
			}
		}
	}
	
	@Override
	public PagingLoadResult< DisciplinaRequisitoDTO > getDisciplinasPreRequisitosList( CenarioDTO cenarioDTO,
			DisciplinaDTO disciplinaDTO, CurriculoDTO curriculoDTO, Integer periodo, PagingLoadConfig config )
	{
		Cenario cenario = ConvertBeans.toCenario(cenarioDTO);
		
		Curriculo curriculo = null;

		if ( curriculoDTO != null )
		{
			curriculo = ConvertBeans.toCurriculo(curriculoDTO);
		}
		
		Disciplina disciplina = null;

		if ( disciplinaDTO != null )
		{
			disciplina = ConvertBeans.toDisciplina(disciplinaDTO);
		}

		List< Object[] > listCurriculoDisciplinaRequisito = findCurriculosDisciplinasAssociacao( 
				cenario, disciplina, curriculo, null, null, periodo, "preRequisitos", config);
		
		List< DisciplinaRequisitoDTO > disciplinasRequisitoDTO = new ArrayList<DisciplinaRequisitoDTO>();

		for ( Object[] cdr : listCurriculoDisciplinaRequisito )
		{
			disciplinasRequisitoDTO.add(buildDisciplinaRequisito( (CurriculoDisciplina)cdr[0], (Disciplina)cdr[1]));
		}

		BasePagingLoadResult< DisciplinaRequisitoDTO > result
		= new BasePagingLoadResult< DisciplinaRequisitoDTO >( disciplinasRequisitoDTO );

		result.setOffset( config.getOffset() );

		result.setTotalLength( 
				CurriculoDisciplina.count(getInstituicaoEnsinoUser(), cenario, curriculo, disciplina, null, null,  periodo, "preRequisitos"));
		
		return result;
	}
	
	@Override
	public PagingLoadResult< DisciplinaRequisitoDTO > getDisciplinasCoRequisitosList( CenarioDTO cenarioDTO,
			DisciplinaDTO disciplinaDTO, CurriculoDTO curriculoDTO, Integer periodo, PagingLoadConfig config )
	{
		Cenario cenario = ConvertBeans.toCenario(cenarioDTO);
		
		Curriculo curriculo = null;

		if ( curriculoDTO != null )
		{
			curriculo = ConvertBeans.toCurriculo(curriculoDTO);
		}
		
		Disciplina disciplina = null;

		if ( disciplinaDTO != null )
		{
			disciplina = ConvertBeans.toDisciplina(disciplinaDTO);
		}
		
		List< Object[] > listCurriculoDisciplinaRequisito = findCurriculosDisciplinasAssociacao( 
				cenario, disciplina, curriculo, null, null, periodo, "coRequisitos" ,config);
		
		List< DisciplinaRequisitoDTO > disciplinasRequisitoDTO = new ArrayList<DisciplinaRequisitoDTO>();

		for ( Object[] cdr : listCurriculoDisciplinaRequisito )
		{
			disciplinasRequisitoDTO.add(buildDisciplinaRequisito( (CurriculoDisciplina)cdr[0], (Disciplina)cdr[1]));
		}

		BasePagingLoadResult< DisciplinaRequisitoDTO > result
		= new BasePagingLoadResult< DisciplinaRequisitoDTO >( disciplinasRequisitoDTO );

		result.setOffset( config.getOffset() );

		result.setTotalLength( 
				CurriculoDisciplina.count(getInstituicaoEnsinoUser(), cenario, curriculo, disciplina, null, null, periodo, "coRequisitos") );
		
		return result;
	}
	
	@Override
	public PagingLoadResult< AlunoDisciplinaCursadaDTO > getAlunosDisciplinasCursadasList( CenarioDTO cenarioDTO,
			DisciplinaDTO disciplinaDTO, CurriculoDTO curriculoDTO, 
			CursoDTO cursoDTO, String matricula, Integer periodo, PagingLoadConfig config )
	{	
		Cenario cenario = ConvertBeans.toCenario(cenarioDTO);
		
		Curriculo curriculo = null;

		if ( curriculoDTO != null )
		{
			curriculo = ConvertBeans.toCurriculo(curriculoDTO);
		}
		
		Disciplina disciplina = null;

		if ( disciplinaDTO != null )
		{
			disciplina = ConvertBeans.toDisciplina(disciplinaDTO);
		}
		
		Curso curso = null;
		if(cursoDTO != null){
			curso = ConvertBeans.toCurso(cursoDTO);
		}
		
		List< Object[] > listCurriculoDisciplinaAluno = findCurriculosDisciplinasAssociacao( 
				cenario, disciplina, curriculo, curso, matricula, periodo, "cursadoPor", config);
		
		List< AlunoDisciplinaCursadaDTO > alunosDisciplinasCursadasDTO = new ArrayList<AlunoDisciplinaCursadaDTO>();

		for ( Object[] cda : listCurriculoDisciplinaAluno )
		{
			alunosDisciplinasCursadasDTO.add(buildAlunoDisciplinaCursada( (CurriculoDisciplina)cda[0], (Aluno)cda[1]) );
		}

		BasePagingLoadResult< AlunoDisciplinaCursadaDTO > result
		= new BasePagingLoadResult< AlunoDisciplinaCursadaDTO >( alunosDisciplinasCursadasDTO );

		result.setOffset( config.getOffset() );

		result.setTotalLength( 
				CurriculoDisciplina.count(getInstituicaoEnsinoUser(), cenario, curriculo, disciplina, curso, matricula, periodo, "cursadoPor") );
		
		return result;
	}
	
	@Override
	public ListLoadResult< CurriculoDisciplinaDTO > getAlunosDisciplinasNaoCursadasList( CenarioDTO cenarioDTO,
			AlunoDTO alunoDTO, CurriculoDTO curriculoDTO)
	{
		List< CurriculoDisciplina > listCurriculoDisciplina = CurriculoDisciplina.findAllByCurriculoAndPeriodo(
				getInstituicaoEnsinoUser(),	ConvertBeans.toCenario(cenarioDTO), ConvertBeans.toCurriculo(curriculoDTO), null);
		
		Aluno aluno = ConvertBeans.toAluno(alunoDTO);
		
		List< CurriculoDisciplinaDTO > curriculoDisciplinasDTO = new ArrayList<CurriculoDisciplinaDTO>();
		for ( CurriculoDisciplina cd : listCurriculoDisciplina )
		{
			if ( !cd.getCursadoPor().contains(aluno) )
			{
				curriculoDisciplinasDTO.add( ConvertBeans.toCurriculoDisciplinaDTO(cd) );
			}
		}

		BaseListLoadResult< CurriculoDisciplinaDTO > result
		= new BaseListLoadResult< CurriculoDisciplinaDTO >( curriculoDisciplinasDTO );
		
		return result;
	}
	
	private List< Object[] > findCurriculosDisciplinasAssociacao( Cenario cenario,
			Disciplina disciplina, Curriculo curriculo, Curso curso, String matricula, Integer periodo, String associacao,
			PagingLoadConfig config  )
	{
		String orderBy = config == null ? null : config.getSortField();

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

		List< Object[] > listCurriculoDisciplinaAssociacao
			= CurriculoDisciplina.findBy(getInstituicaoEnsinoUser(), cenario, curriculo, disciplina,
					curso, matricula,
					periodo, associacao, orderBy, config.getOffset(), config.getLimit() );
		
		return listCurriculoDisciplinaAssociacao;
	}
	
	private DisciplinaRequisitoDTO buildDisciplinaRequisito( CurriculoDisciplina curriculoDisciplina, Disciplina requisito )
	{
		DisciplinaRequisitoDTO disciplinaRequisitoDTO = new DisciplinaRequisitoDTO();
		
		disciplinaRequisitoDTO.setCurriculoDisciplinaId( curriculoDisciplina.getId() );
		disciplinaRequisitoDTO.setCurriculoId( curriculoDisciplina.getCurriculo().getId() );
		disciplinaRequisitoDTO.setCurriculoString( curriculoDisciplina.getCurriculo().getCodigo() );
		disciplinaRequisitoDTO.setDisciplinaId( curriculoDisciplina.getDisciplina().getId() );
		disciplinaRequisitoDTO.setDisciplinaString( curriculoDisciplina.getDisciplina().getCodigo() );
		disciplinaRequisitoDTO.setPeriodo( curriculoDisciplina.getPeriodo() );
		disciplinaRequisitoDTO.setDisciplinaRequisitoId( requisito.getId() );
		disciplinaRequisitoDTO.setDisciplinaRequisitoString( requisito.getCodigo() );
			
		return disciplinaRequisitoDTO;
	}
	
	private AlunoDisciplinaCursadaDTO buildAlunoDisciplinaCursada( CurriculoDisciplina curriculoDisciplina, Aluno cursadoPor )
	{
		AlunoDisciplinaCursadaDTO alunoDisciplinaCusadaDTO = new AlunoDisciplinaCursadaDTO();
		
		alunoDisciplinaCusadaDTO.setCurriculoDisciplinaId( curriculoDisciplina.getId() );
		alunoDisciplinaCusadaDTO.setCursoId( curriculoDisciplina.getCurriculo().getCurso().getId() );
		alunoDisciplinaCusadaDTO.setCurriculoId( curriculoDisciplina.getCurriculo().getId() );
		alunoDisciplinaCusadaDTO.setCurriculoString( curriculoDisciplina.getCurriculo().getCodigo() );
		alunoDisciplinaCusadaDTO.setDisciplinaId( curriculoDisciplina.getDisciplina().getId() );
		alunoDisciplinaCusadaDTO.setCursoString( curriculoDisciplina.getCurriculo().getCurso().getCodigo() );
		alunoDisciplinaCusadaDTO.setDisciplinaString( curriculoDisciplina.getDisciplina().getCodigo() );
		alunoDisciplinaCusadaDTO.setPeriodo( curriculoDisciplina.getPeriodo() );
		alunoDisciplinaCusadaDTO.setAlunoId( cursadoPor.getId() );
		alunoDisciplinaCusadaDTO.setAlunoString( cursadoPor.getMatricula() );
			
		return alunoDisciplinaCusadaDTO;
	}

}
