package com.gapso.web.trieda.server;

import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;
import java.util.Set;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Demanda;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.domain.Unidade;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.HorarioAulaDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.TrioDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.services.HorariosAulaService;
import com.gapso.web.trieda.shared.util.view.TriedaException;

public class HorariosAulaServiceImpl
	extends RemoteService implements HorariosAulaService
{
	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public PagingLoadResult< HorarioAulaDTO > getBuscaList( SemanaLetivaDTO semanaLetivaDTO,
		TurnoDTO turnoDTO, Date horario, PagingLoadConfig config )
	{
		List< HorarioAulaDTO > list = new ArrayList< HorarioAulaDTO >();
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

		SemanaLetiva semanaLetiva = ( semanaLetivaDTO == null ) ? null :
			ConvertBeans.toSemanaLetiva( semanaLetivaDTO );

		Turno turno = ( turnoDTO == null ) ? null : ConvertBeans.toTurno( turnoDTO );

		List< HorarioAula > listHorariosAula = HorarioAula.findBy( getInstituicaoEnsinoUser(),
			semanaLetiva, turno, horario, config.getOffset(), config.getLimit(), orderBy );

		for ( HorarioAula horarioAula : listHorariosAula )
		{
			list.add( ConvertBeans.toHorarioAulaDTO( horarioAula ) );
		}

		BasePagingLoadResult< HorarioAulaDTO > result
			= new BasePagingLoadResult< HorarioAulaDTO >( list );

		result.setOffset( config.getOffset() );
		result.setTotalLength( HorarioAula.count(
			getInstituicaoEnsinoUser(), semanaLetiva, turno, horario ) );

		return result;
	}

	@Override
	public void save( CenarioDTO cenarioDTO, HorarioAulaDTO horarioAulaDTO )
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		HorarioAula horarioDeAula
			= ConvertBeans.toHorarioAula( horarioAulaDTO );

		if ( horarioDeAula.getId() != null
			&& horarioDeAula.getId() > 0 )
		{
			horarioDeAula.merge();
		}
		else
		{
			horarioDeAula.persist();

			List< Campus > campi = Campus.findByCenario( this.getInstituicaoEnsinoUser(), cenario );
			List< Unidade > unidades = Unidade.findByCenario( getInstituicaoEnsinoUser(), cenario );
			List< Sala > salas = Sala.findByCenario( getInstituicaoEnsinoUser(), cenario );
			List< Disciplina > disciplinas = Disciplina.findByCenario( getInstituicaoEnsinoUser(), cenario );
			List< Professor > professores = Professor.findByCenario( getInstituicaoEnsinoUser(), cenario );

			for ( Semanas semana : Semanas.values() )
			{
				if ( semana == Semanas.SAB
					|| semana == Semanas.DOM )
				{
					continue;
				}

				HorarioDisponivelCenario hdc = new HorarioDisponivelCenario();

				hdc.setDiaSemana( semana );
				hdc.setHorarioAula( horarioDeAula );

/*				hdc.getCampi().addAll( campi );
				hdc.getUnidades().addAll( unidades );
				hdc.getSalas().addAll( salas );
				hdc.getDisciplinas().addAll( disciplinas );
				hdc.getProfessores().addAll( professores );*/

				hdc.persist();
			}
		}
	}
	
	@Override
	public void remove( List< HorarioAulaDTO > horariosAulaDTOList )
	{
		for ( HorarioAulaDTO horarioAulaDTO : horariosAulaDTOList )
		{
			ConvertBeans.toHorarioAula( horarioAulaDTO ).remove();
		}
	}

	@Override
	public void removeWithHorario( HorarioAulaDTO horarioAulaDTO )
	{
		SemanaLetiva semanaLetiva = SemanaLetiva.find(
			horarioAulaDTO.getSemanaLetivaId(), getInstituicaoEnsinoUser() );

		Turno turno = Turno.find( horarioAulaDTO.getTurnoId(),
			getInstituicaoEnsinoUser() );

		Calendar cal1 = Calendar.getInstance();
		cal1.setTime( horarioAulaDTO.getInicio() );

		List< HorarioAula > listHorariosAula
			= HorarioAula.findHorarioAulasBySemanaLetivaAndTurno(
				getInstituicaoEnsinoUser(), semanaLetiva, turno );

		for ( HorarioAula horarioAula : listHorariosAula )
		{
			Calendar cal2 = Calendar.getInstance();
			cal2.setTime( horarioAula.getHorario() );

			if ( cal1.get( Calendar.HOUR ) == cal2.get( Calendar.HOUR )
				&& cal1.get( Calendar.MINUTE ) == cal2.get( Calendar.MINUTE ) )
			{
				horarioAula.remove();
			}
		}
	}
	
	@Override
	public ParDTO<HorarioDisponivelCenarioDTO, SemanaLetivaDTO> getHorarioDisponivelCenario(
		Long id )
	{
		HorarioDisponivelCenario hdc = HorarioDisponivelCenario.find(id, getInstituicaoEnsinoUser());
		List<HorarioDisponivelCenario> list = new ArrayList<HorarioDisponivelCenario>();
		list.add(hdc);
		return ParDTO.create(ConvertBeans.toHorarioDisponivelCenarioDTO(list).get(0),
				ConvertBeans.toSemanaLetivaDTO(hdc.getHorarioAula().getSemanaLetiva()));
	}
	
	@Override
	public TrioDTO<HorarioDisponivelCenarioDTO, String, SemanaLetivaDTO> findHorarioDisponivelCenario(String horarioInicialString, Integer semana, DemandaDTO demandaDTO) throws TriedaException
	{
		Demanda demanda = Demanda.find(demandaDTO.getId(), getInstituicaoEnsinoUser());
		DateFormat formatter = new SimpleDateFormat("HH:mm");
		Date date;
		try {
			date = formatter.parse(horarioInicialString);
		} catch (ParseException e) {
			e.printStackTrace();
			throw new TriedaException("Erro ao fazer parsing do horário");
		}
		
		Set<HorarioAula> horarios = demanda.getOferta().getCurriculo().getSemanaLetiva().getHorariosAula();
		List<HorarioDisponivelCenario> list = new ArrayList<HorarioDisponivelCenario>();
		for (HorarioAula horario : horarios)
		{
			if (formatter.format(horario.getHorario()).equals(formatter.format(date)))
			{
				try {
					list.add(HorarioDisponivelCenario.findBy(getInstituicaoEnsinoUser(), horario, Semanas.get(semana)));
				} catch (Exception e) {
					throw new TriedaException("Nao existe este horário cadastrado na Semana Letiva");
				}
			}
		}
		
		if (list.isEmpty())
		{
			throw new TriedaException("Nao existe este horário cadastrado na Semana Letiva");
		}
		
		return TrioDTO.create(ConvertBeans.toHorarioDisponivelCenarioDTO(list).get(0), Semanas.get(semana).toString(),
				ConvertBeans.toSemanaLetivaDTO(demanda.getOferta().getCurriculo().getSemanaLetiva()));
	}
}
