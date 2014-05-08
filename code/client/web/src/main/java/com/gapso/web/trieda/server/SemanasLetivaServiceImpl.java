package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.AtendimentoOperacional;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Unidade;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.server.util.TriedaServerUtil;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.services.SemanasLetivaService;

public class SemanasLetivaServiceImpl
	extends RemoteService
	implements SemanasLetivaService
{
	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public ListLoadResult< SemanaLetivaDTO > getList( CenarioDTO cenarioDTO, BasePagingLoadConfig loadConfig )
	{
		return getBuscaList( cenarioDTO, loadConfig.get( "query" ).toString(), null, null, null, null, loadConfig );
	}

	@Override
	public PagingLoadResult< SemanaLetivaDTO > getBuscaList(
		CenarioDTO cenarioDTO, String codigo, String descricao,
		String operadorTempo, Integer tempo, Boolean permite, PagingLoadConfig config )
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		List< SemanaLetivaDTO > list
			= new ArrayList< SemanaLetivaDTO >();

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

		List< SemanaLetiva > listDomains = SemanaLetiva.findBy(
			getInstituicaoEnsinoUser(), cenario, codigo, descricao, operadorTempo, tempo, permite,
			config.getOffset(), config.getLimit(), orderBy );

		for ( SemanaLetiva semanaLetiva : listDomains )
		{
			list.add( ConvertBeans.toSemanaLetivaDTO( semanaLetiva ) );
		}

		BasePagingLoadResult< SemanaLetivaDTO > result
			= new BasePagingLoadResult< SemanaLetivaDTO >( list );

		result.setOffset( config.getOffset() );
		result.setTotalLength( SemanaLetiva.count(
			getInstituicaoEnsinoUser(), cenario, codigo, descricao,
			operadorTempo, tempo, permite) );

		return result;
	}

	@Override
	public ListLoadResult< SemanaLetivaDTO > getList( CenarioDTO cenarioDTO)
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		List< SemanaLetivaDTO > list
			= new ArrayList< SemanaLetivaDTO >();

		List< SemanaLetiva > listDomains
			= SemanaLetiva.findByCenario( getInstituicaoEnsinoUser(), cenario );

		for ( SemanaLetiva semanaLetiva : listDomains )
		{
			list.add( ConvertBeans.toSemanaLetivaDTO( semanaLetiva ) );
		}

		return new BaseListLoadResult< SemanaLetivaDTO >( list );
	}

	@Override
	public void save( SemanaLetivaDTO semanaLetivaDTO )
	{
		SemanaLetiva semanaLetiva
			= ConvertBeans.toSemanaLetiva( semanaLetivaDTO );

		if ( semanaLetiva.getId() != null
			&& semanaLetiva.getId() > 0 )
		{
			semanaLetiva.merge();
		}
		else
		{
			semanaLetiva.persist();
		}
	}

	@Override
	public void remove( List< SemanaLetivaDTO > semanaLetivaDTOList )
	{
		for ( SemanaLetivaDTO semanaLetivaDTO : semanaLetivaDTOList )
		{
			ConvertBeans.toSemanaLetiva( semanaLetivaDTO ).remove();
		}
	}

	private List<HorarioDisponivelCenarioDTO> getHorariosDisponiveisCenarioDTOs(SemanaLetivaDTO semanaLetivaDTO) {
		SemanaLetiva semanaLetiva = SemanaLetiva.find(semanaLetivaDTO.getId(),getInstituicaoEnsinoUser());
		Set<HorarioAula> horariosAula = new HashSet<HorarioAula>(semanaLetiva.getHorariosAula());

		List<HorarioDisponivelCenarioDTO> list = new ArrayList<HorarioDisponivelCenarioDTO>();
		for (HorarioAula o : horariosAula) {
			list.add(ConvertBeans.toHorarioDisponivelCenarioDTO(o));
		}

		TriedaServerUtil.ordenaHorariosDTOPorSemanaLetivaETurno(list);
		
		return list;
	}
	
	@Override
	public PagingLoadResult< HorarioDisponivelCenarioDTO > getHorariosDisponiveisCenario(
		SemanaLetivaDTO semanaLetivaDTO )
	{
		List< HorarioDisponivelCenarioDTO > list
			= getHorariosDisponiveisCenarioDTOs( semanaLetivaDTO );

		BasePagingLoadResult< HorarioDisponivelCenarioDTO > result
			= new BasePagingLoadResult< HorarioDisponivelCenarioDTO >( list );

		result.setOffset( 0 );
		result.setTotalLength( list.size() );

		return result;
	}
	
	public void removeHorariosDisponiveisCenario(SemanaLetivaDTO semanaLetivaDTO,
		List< HorarioDisponivelCenarioDTO > listDTO){
		List< HorarioDisponivelCenario > listSelecionados
		= ConvertBeans.toHorarioDisponivelCenario( listDTO );

		SemanaLetiva semanaLetiva = SemanaLetiva.find(
			semanaLetivaDTO.getId(), getInstituicaoEnsinoUser() );
	
		List< HorarioDisponivelCenario > todosQueJaTinhamList
			= new ArrayList< HorarioDisponivelCenario >();
	
		for ( HorarioAula horarioAula : semanaLetiva.getHorariosAula() )
		{
			todosQueJaTinhamList.addAll(
				horarioAula.getHorariosDisponiveisCenario() );
		}
	
		List< HorarioDisponivelCenario > removerList
			= new ArrayList< HorarioDisponivelCenario >( todosQueJaTinhamList );
	
		removerList.removeAll( listSelecionados );
		
		for(HorarioDisponivelCenario hdc: removerList){
			List<AtendimentoOperacional> atendimentoOperacionalList = AtendimentoOperacional.findAllByDiasHorariosAula(getInstituicaoEnsinoUser(), hdc);
			for(AtendimentoOperacional atendimentoOperacional : atendimentoOperacionalList){
				atendimentoOperacional.remove();
			}
		}
		
		for(HorarioAula horariosAula : semanaLetiva.getHorariosAula()){
			horariosAula.getHorariosDisponiveisCenario().removeAll(removerList);
			horariosAula.merge();
		}
	
	}

	@Override
	public void saveHorariosDisponiveisCenario(
		CenarioDTO cenarioDTO, SemanaLetivaDTO semanaLetivaDTO,
		List< HorarioDisponivelCenarioDTO > listDTO )
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		List< HorarioDisponivelCenario > listSelecionados
			= ConvertBeans.toHorarioDisponivelCenario( listDTO );

		SemanaLetiva semanaLetiva = SemanaLetiva.find(
			semanaLetivaDTO.getId(), getInstituicaoEnsinoUser() );

		List< HorarioDisponivelCenario > todosQueJaTinhamList
			= new ArrayList< HorarioDisponivelCenario >();

		for ( HorarioAula horarioAula : semanaLetiva.getHorariosAula() )
		{
			todosQueJaTinhamList.addAll(
				horarioAula.getHorariosDisponiveisCenario() );
		}

		List< HorarioDisponivelCenario > removerList
			= new ArrayList< HorarioDisponivelCenario >( todosQueJaTinhamList );

		removerList.removeAll( listSelecionados );

		for ( HorarioAula horariosAula : semanaLetiva.getHorariosAula() )
		{
			horariosAula.getHorariosDisponiveisCenario().removeAll( removerList );
			horariosAula.merge();
		}

		List< HorarioDisponivelCenario > adicionarList
			= new ArrayList< HorarioDisponivelCenario >( listSelecionados );

		adicionarList.removeAll( todosQueJaTinhamList );

		if ( adicionarList != null && adicionarList.size() > 0 )
		{
			List< Campus > campi = Campus.findByCenario( this.getInstituicaoEnsinoUser(), cenario );
			List< Unidade > unidades = Unidade.findByCenario( getInstituicaoEnsinoUser(), cenario );
			List< Sala > salas = Sala.findByCenario( getInstituicaoEnsinoUser(), cenario );
			List< Disciplina > disciplinas = Disciplina.findByCenario( getInstituicaoEnsinoUser(), cenario );
			List< Professor > professores = Professor.findByCenario( getInstituicaoEnsinoUser(), cenario );
	
			for ( HorarioDisponivelCenario o : adicionarList )
			{
				o.getCampi().addAll( campi );
				o.getUnidades().addAll( unidades );
				o.getSalas().addAll( salas );
				for (Disciplina disciplina : disciplinas)
				{
					if (o.getDiaSemana() == Semanas.SAB)
					{
						if (disciplina.getUsaSabado())
							o.getDisciplinas().add(disciplina);
					}
					else if (o.getDiaSemana() == Semanas.DOM)
					{
						
						if (disciplina.getUsaDomingo())
							o.getDisciplinas().add(disciplina);
					}
					else
					{
						o.getDisciplinas().add(disciplina);
					}
				}
				o.getProfessores().addAll( professores );
	
				o.merge();
			}
		}
	}

	@Override
	public PagingLoadResult< HorarioDisponivelCenarioDTO > getAllHorariosDisponiveisCenario( CenarioDTO cenarioDTO ) {
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		List<SemanaLetiva> semanasLetivas = SemanaLetiva.findByCenario(getInstituicaoEnsinoUser(), cenario);

		List<SemanaLetivaDTO> semanasLetivasDTO = new ArrayList<SemanaLetivaDTO>();
		for (SemanaLetiva semanaLetiva : semanasLetivas) {
			semanasLetivasDTO.add(ConvertBeans.toSemanaLetivaDTO(semanaLetiva));
		}

		List<HorarioDisponivelCenarioDTO> horarios = new ArrayList<HorarioDisponivelCenarioDTO>();
		for (SemanaLetivaDTO semanaLetivaDTO : semanasLetivasDTO) {
			horarios.addAll(getHorariosDisponiveisCenarioDTOs(semanaLetivaDTO));
		}
		
		TriedaServerUtil.ordenaHorariosDTOPorSemanaLetivaETurno(horarios);

		BasePagingLoadResult<HorarioDisponivelCenarioDTO> result = new BasePagingLoadResult<HorarioDisponivelCenarioDTO>(horarios);

		result.setOffset(0);
		result.setTotalLength(horarios.size());

		return result;
	}

	@Override
	public SemanaLetivaDTO findSemanaLetiva( Long id )
	{
		if ( id == null )
		{
			return null;
		}
		
		SemanaLetiva semanaLetiva
			= SemanaLetiva.find( id, getInstituicaoEnsinoUser() );

		if ( semanaLetiva == null )
		{
			return null;
		}

		return ConvertBeans.toSemanaLetivaDTO( semanaLetiva );
	}
}
