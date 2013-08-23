package com.gapso.web.trieda.server;

import java.util.ArrayList;
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
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
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
	public ListLoadResult< CurriculoDTO > getList( CenarioDTO cenarioDTO, BasePagingLoadConfig config )
	{
		CursoDTO cursoDTO = config.get( "cursoDTO" );

		return getBuscaList( cenarioDTO, cursoDTO, config.get( "query" ).toString(), null, config );
	}

	@Override
	public PagingLoadResult< CurriculoDTO > getBuscaList( CenarioDTO cenarioDTO, 
		CursoDTO cursoDTO, String codigo, String descricao, PagingLoadConfig config )
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

		List< Curriculo > listCurriculos = Curriculo.findBy( getInstituicaoEnsinoUser(),
			cenario, curso, codigo, descricao, config.getOffset(), config.getLimit(), orderBy );

		for ( Curriculo curriculo : listCurriculos )
		{
			list.add( ConvertBeans.toCurriculoDTO( curriculo ) );
		}

		BasePagingLoadResult< CurriculoDTO > result
			= new BasePagingLoadResult< CurriculoDTO >( list );

		result.setOffset( config.getOffset() );
		result.setTotalLength( Curriculo.count(
			getInstituicaoEnsinoUser(), cenario, curso, codigo, descricao ) );

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
	public List< Integer > getPeriodos( CurriculoDTO curriculoDTO )
	{
		Curriculo curriculo = Curriculo.find(
			curriculoDTO.getId(), getInstituicaoEnsinoUser() );

		if ( curriculo == null )
		{
			return new ArrayList< Integer >();
		}

		return curriculo.getPeriodos();
	}

	@Override
	public void removeDisciplina( List< CurriculoDisciplinaDTO > curriculoDisciplinaDTOList )
	{
		for ( CurriculoDisciplinaDTO curriculoDisciplinaDTO : curriculoDisciplinaDTOList )
		{
			ConvertBeans.toCurriculoDisciplina( curriculoDisciplinaDTO ).remove();
		}
	}
}
