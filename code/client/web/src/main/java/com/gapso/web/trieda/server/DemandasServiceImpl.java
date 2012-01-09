package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.InstituicaoEnsino;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.services.DemandasService;
import com.gapso.web.trieda.shared.util.view.TriedaException;

public class DemandasServiceImpl
	extends RemoteService implements DemandasService
{
	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public PagingLoadResult< DemandaDTO > getBuscaList( CampusDTO campusDTO,
		CursoDTO cursoDTO, CurriculoDTO curriculoDTO,
		TurnoDTO turnoDTO, DisciplinaDTO disciplinaDTO, PagingLoadConfig config )
	{
		List< DemandaDTO > list = new ArrayList< DemandaDTO >();
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
			campus = ConvertBeans.toCampus( campusDTO );
		}
		
		Curso curso = null;
		if ( cursoDTO != null )
		{
			curso = ConvertBeans.toCurso( cursoDTO );
		}

		Curriculo curriculo	= null;
		if ( curriculoDTO != null )
		{
			curriculo = ConvertBeans.toCurriculo( curriculoDTO );
		}

		Turno turno = null;
		if ( turnoDTO != null )
		{
			turno = ConvertBeans.toTurno( turnoDTO );
		}

		Disciplina disciplina = null;
		if ( disciplinaDTO != null )
		{
			disciplina = ConvertBeans.toDisciplina( disciplinaDTO );
		}

		List< Demanda > listDomains = Demanda.findBy( getInstituicaoEnsinoUser(),
			campus, curso, curriculo, turno, disciplina,
			config.getOffset(), config.getLimit(), orderBy );

		for ( Demanda demanda : listDomains )
		{
			list.add( ConvertBeans.toDemandaDTO( demanda ) );
		}

		BasePagingLoadResult< DemandaDTO > result
			= new BasePagingLoadResult< DemandaDTO >( list );

		result.setOffset( config.getOffset() );
		result.setTotalLength( Demanda.count( getInstituicaoEnsinoUser(),
			campus, curso, curriculo, turno, disciplina ) );

		return result;
	}

	@Override
	public void save( DemandaDTO demandaDTO ) throws TriedaException
	{
		try {
			Demanda d = ConvertBeans.toDemanda( demandaDTO );
	
			if ( d.getId() != null && d.getId() > 0 )
			{
				d.merge();
			}
			else
			{
				List< Demanda > demandas = Demanda.findBy( getInstituicaoEnsinoUser(),
					d.getOferta().getCampus(), d.getOferta().getCurriculo().getCurso(),
					d.getOferta().getCurriculo(), d.getOferta().getTurno(), d.getDisciplina(), 0, 1, null );
	
				if ( !demandas.isEmpty() )
				{
					Integer qtd = d.getQuantidade();
	
					d = demandas.get( 0 );
					d.setQuantidade( qtd );
	
					d.merge();
				}
				else
				{
					d.persist();
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
			throw new TriedaException(e);
		}
	}

	@Override
	public void remove( List< DemandaDTO > demandaDTOList )
	{
		for ( DemandaDTO demandaDTO : demandaDTOList )
		{
			Demanda demanda = ConvertBeans.toDemanda( demandaDTO );

			if ( demanda != null )
			{
				demanda.remove();
			}
		}
	}
	
	@Override
	public Integer findPeriodo(DemandaDTO demandaDTO) throws TriedaException {
		Integer periodo = null;
		
		try {
			InstituicaoEnsino instituicaoEnsino = InstituicaoEnsino.find(demandaDTO.getInstituicaoEnsinoId());
			Curriculo curriculo = Curriculo.find(demandaDTO.getCurriculoId(),instituicaoEnsino);
			Disciplina disciplina = Disciplina.find(demandaDTO.getDisciplinaId(),instituicaoEnsino);
			Oferta oferta = Oferta.find(demandaDTO.getOfertaId(),instituicaoEnsino);
			
			periodo = curriculo.getPeriodo(instituicaoEnsino,disciplina,oferta);
		} catch (Exception e) {
			e.printStackTrace();
			throw new TriedaException(e);
		}
		
		return periodo; 
	}
}
