package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import org.springframework.transaction.annotation.Transactional;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.AlunoDemanda;
import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Curriculo;
import com.gapso.trieda.domain.CurriculoDisciplina;
import com.gapso.trieda.domain.Curso;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.Oferta;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDemandaDTO;
import com.gapso.web.trieda.shared.dtos.OfertaDTO;
import com.gapso.web.trieda.shared.dtos.TreeNodeDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.services.OfertasService;
import com.gapso.web.trieda.shared.util.view.TriedaException;

@Transactional
public class OfertasServiceImpl
	extends RemoteService implements OfertasService
{
	private static final long serialVersionUID = -3010939181486905949L;
	
	@Override
	public ListLoadResult<OfertaDTO> getListAll( CenarioDTO cenarioDTO ) {
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		List<OfertaDTO> ofertasDTOs = new ArrayList<OfertaDTO>();
		List<Oferta> ofertas = Oferta.findByCenario(this.getInstituicaoEnsinoUser(), cenario);

		for (Oferta oferta : ofertas) {
			ofertasDTOs.add(ConvertBeans.toOfertaDTO(oferta));
		}

		return new BaseListLoadResult<OfertaDTO>(ofertasDTOs);
	}

	@Override
	public OfertaDTO getOferta( Long id )
	{
		Oferta oferta = Oferta.find(
			id, getInstituicaoEnsinoUser() );
		
		if ( oferta == null )
		{
			return null;
		}

		return ConvertBeans.toOfertaDTO( oferta );
	}

	@Override
	public PagingLoadResult< OfertaDTO > getBuscaList( CenarioDTO cenarioDTO, 
		TurnoDTO turnoDTO, CampusDTO campusDTO, CursoDTO cursoDTO,
		CurriculoDTO curriculoDTO, PagingLoadConfig config )
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		List< OfertaDTO > list = new ArrayList< OfertaDTO >();
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

		Turno turno = ( ( turnoDTO != null ) ? ConvertBeans.toTurno( turnoDTO ) : null );
		Campus campus = ( ( campusDTO != null ) ? ConvertBeans.toCampus( campusDTO ) : null );
		Curso curso = ( ( cursoDTO != null ) ? ConvertBeans.toCurso( cursoDTO ) : null );
		Curriculo curriculo = ( ( curriculoDTO != null ) ? ConvertBeans.toCurriculo( curriculoDTO ) : null );

		List< Oferta > listOfertas = Oferta.findBy( getInstituicaoEnsinoUser(), cenario,
			turno, campus, curso, curriculo, config.getOffset(), config.getLimit(), orderBy );

		for ( Oferta oferta : listOfertas )
		{
			list.add( ConvertBeans.toOfertaDTO( oferta ) );
		}

		BasePagingLoadResult< OfertaDTO > result
			= new BasePagingLoadResult< OfertaDTO >( list );

		result.setOffset( config.getOffset() );
		result.setTotalLength( Oferta.count(
			getInstituicaoEnsinoUser(), cenario, turno, campus, curso, curriculo ) );

		return result;
	}

	@Override
	public ListLoadResult< TreeNodeDTO > getListByCampusAndTurno(
		CampusDTO campusDTO, TurnoDTO turnoDTO )
	{
		List< TreeNodeDTO > treeNodesList = new ArrayList< TreeNodeDTO >();

		Campus campus = ( campusDTO == null ) ? null : ConvertBeans.toCampus( campusDTO );
		Turno turno = ( turnoDTO == null ) ? null : ConvertBeans.toTurno( turnoDTO );

		List< Oferta > listDomains = Oferta.findByCampusAndTurno(
			getInstituicaoEnsinoUser(), campus, turno );

		for ( Oferta oferta : listDomains )
		{
			OfertaDTO ofertaDTO = ConvertBeans.toOfertaDTO( oferta );
			treeNodesList.add( new TreeNodeDTO( ofertaDTO ) );
		}

		Collections.sort( treeNodesList );
		ListLoadResult< TreeNodeDTO > result
			= new BaseListLoadResult< TreeNodeDTO >( treeNodesList );

		return result;
	}
	
	@Override
	public ListLoadResult<DisciplinaDemandaDTO> getDisciplinas(DemandaDTO demandaDTO, OfertaDTO ofertaDTO, Integer periodo)
	{
		Oferta oferta = ConvertBeans.toOferta(ofertaDTO);
		
		List<DisciplinaDemandaDTO> disciplinasDemandaDTO = new ArrayList<DisciplinaDemandaDTO>();

		for (CurriculoDisciplina curriculoDisciplina : oferta.getCurriculo().getCurriculoDisciplinas())
		{
			if (curriculoDisciplina.getPeriodo().equals(periodo) )
			{
				Demanda demanda = Demanda.findbyOfertaAndDisciplina(getInstituicaoEnsinoUser(), oferta, curriculoDisciplina.getDisciplina());
				DisciplinaDemandaDTO novaDisciplina = new DisciplinaDemandaDTO();
				novaDisciplina.setDisciplina(curriculoDisciplina.getDisciplina().getCodigo());
				novaDisciplina.setDisciplinaId(curriculoDisciplina.getDisciplina().getId());
				
				Integer demandaReal = demanda == null ? 0 : AlunoDemanda.findByDemandaReal(getInstituicaoEnsinoUser(), demanda).size();
				Integer demandaVirtual = demanda == null ? 0 : AlunoDemanda.findByDemandaVirtual(getInstituicaoEnsinoUser(), demanda).size();
				
				
				novaDisciplina.setDemandaReal(demandaReal);
				novaDisciplina.setDemandaVirtual(demandaVirtual);
				novaDisciplina.setDemandaTotal(demandaReal+demandaVirtual);
				
				novaDisciplina.setNovaDemanda(0);
				
				disciplinasDemandaDTO.add(novaDisciplina);
			}
		}
		
		ListLoadResult< DisciplinaDemandaDTO > result
			= new BaseListLoadResult< DisciplinaDemandaDTO >( disciplinasDemandaDTO );
		return result;
	}

	@Override
	public void save( OfertaDTO ofertaDTO )
	{
		Oferta oferta = ConvertBeans.toOferta( ofertaDTO );

		if ( oferta.getId() != null
			&& oferta.getId() > 0 )
		{
			oferta.merge();
		}
		else
		{
			oferta.persist();
		}
	}

	@Override
	public void remove( List< OfertaDTO > ofertaDTOList ) throws TriedaException
	{
		for ( OfertaDTO ofertaDTO : ofertaDTOList )
		{
			Oferta oferta = ConvertBeans.toOferta( ofertaDTO );
			List<AtendimentoOperacional> aos = AtendimentoOperacional.findAllBy(oferta, getInstituicaoEnsinoUser());
			List<AtendimentoTatico> aot = AtendimentoTatico.findAllBy(getInstituicaoEnsinoUser(), oferta);
			if(aos.isEmpty() && aot.isEmpty()){
				oferta.remove();
			} else {
				throw new TriedaException("Não foi possível remover a Oferta. 2");
			}
		}
	}
}
