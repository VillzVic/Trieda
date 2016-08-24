package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;
import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.Equivalencia;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.EquivalenciaDTO;
import com.gapso.web.trieda.shared.services.EquivalenciasService;
import com.gapso.web.trieda.shared.util.view.TriedaException;

public class EquivalenciasServiceImpl
	extends RemoteService implements EquivalenciasService
{
	private static final long serialVersionUID = 345113452407626806L;

	@Override
	public EquivalenciaDTO getEquivalencia( Long id )
	{
		if ( id == null )
		{
			return null;
		}

		Equivalencia equivalencia
			= Equivalencia.find( id, getInstituicaoEnsinoUser() );  
		
		if ( equivalencia == null )
		{
			return null;
		}

		return ConvertBeans.toEquivalenciaDTO( equivalencia );
	}
	
	@Override
	public PagingLoadResult<EquivalenciaDTO> getBuscaList(CenarioDTO cenarioDTO, 
			DisciplinaDTO disciplinaDTO, CursoDTO cursoDTO, PagingLoadConfig config) {
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}

		Disciplina disciplina = (disciplinaDTO == null) ? null :
			Disciplina.find( disciplinaDTO.getId(), getInstituicaoEnsinoUser() );
		
		Curso curso = (cursoDTO == null) ? null :
			ConvertBeans.toCurso(cursoDTO);

		List< Equivalencia > list = Equivalencia.findBy( getInstituicaoEnsinoUser(),
			cenario, disciplina, curso, config.getOffset(), config.getLimit(), orderBy );

		List< EquivalenciaDTO > listDTO
			= new ArrayList< EquivalenciaDTO >();

		for ( Equivalencia equivalencia : list )
		{
			listDTO.add( ConvertBeans.toEquivalenciaDTO( equivalencia ) );
		}

		BasePagingLoadResult< EquivalenciaDTO > result
			= new BasePagingLoadResult< EquivalenciaDTO >( listDTO );

		result.setOffset( config.getOffset() );
		result.setTotalLength( Equivalencia.count(
			getInstituicaoEnsinoUser(), cenario, disciplina, curso ) );

		return result;
	}
	
	@Override
	public void save( EquivalenciaDTO equivalenciaDTO, List<CursoDTO> cursosSelecionados ) throws TriedaException
	{
		Equivalencia equivalencia
			= ConvertBeans.toEquivalencia( equivalenciaDTO );
		
		if ( equivalencia.getId() != null && equivalencia.getId() >0 )
		{
			if ( !equivalenciaDTO.getEquivalenciaGeral() || cursosSelecionados != null )
			{
				for ( CursoDTO cursoDTO : cursosSelecionados )
				{
						equivalencia.getCursos().add( Curso.find(cursoDTO.getId(), getInstituicaoEnsinoUser()) );
				}
			}
			equivalencia.merge();
		}
		else
		{
			if ( Equivalencia.findBy(getInstituicaoEnsinoUser(), equivalencia.getCursou(), equivalencia.getElimina()).size() > 0)
			{
				throw new TriedaException("Conjunto de disciplinas cursou-elimina já existe");
			}
			if ( !equivalenciaDTO.getEquivalenciaGeral() || cursosSelecionados != null )
			{
				for ( CursoDTO cursoDTO : cursosSelecionados )
				{
					equivalencia.getCursos().add( Curso.find(cursoDTO.getId(), getInstituicaoEnsinoUser()) );
				}
			}
			equivalencia.persist();
		}
	}
	
	@Override
	public void remove( List< EquivalenciaDTO > equivalenciaDTOList )
	{
		for ( EquivalenciaDTO turnoDTO : equivalenciaDTOList )
		{
			Equivalencia equivalencia
				= Equivalencia.find( turnoDTO.getId(), getInstituicaoEnsinoUser() ); 

			if ( equivalencia != null )
			{
				equivalencia.remove();
			}
		}
	}
	
	@Override
	public void removeAll( CenarioDTO cenarioDTO )
	{
		Equivalencia.removeAll(ConvertBeans.toCenario(cenarioDTO));
		
		/*List<Equivalencia> equivalencia = Equivalencia.findByCenario(getInstituicaoEnsinoUser(), ConvertBeans.toCenario(cenarioDTO)); 

		for(Equivalencia equiv : equivalencia){
			
			if ( equiv != null )
			{
				equiv.remove();
			}
		}*/
	}
	
	@Override
	public List< CursoDTO > getCursosEquivalencia( EquivalenciaDTO equivalenciaDTO )
	{
		List< CursoDTO > cursosEquivalencia= new ArrayList< CursoDTO >();
		
		for( Curso curso : Equivalencia.find( equivalenciaDTO.getId(), getInstituicaoEnsinoUser() ).getCursos() )
		{
			cursosEquivalencia.add( ConvertBeans.toCursoDTO(curso) );
		}
		
		return cursosEquivalencia;
	}

}
