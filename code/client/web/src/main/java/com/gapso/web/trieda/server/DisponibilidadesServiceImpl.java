package com.gapso.web.trieda.server;

import java.util.Calendar;
import java.util.Date;
import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Cenario;
import com.gapso.trieda.domain.Disponibilidade;
import com.gapso.trieda.domain.DisponibilidadeCampus;
import com.gapso.trieda.domain.DisponibilidadeDisciplina;
import com.gapso.trieda.domain.DisponibilidadeProfessor;
import com.gapso.trieda.domain.DisponibilidadeSala;
import com.gapso.trieda.domain.DisponibilidadeUnidade;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisponibilidadeDTO;
import com.gapso.web.trieda.shared.services.DisponibilidadesService;
import com.gapso.web.trieda.shared.util.view.TriedaException;

public class DisponibilidadesServiceImpl
	extends RemoteService implements DisponibilidadesService
{
	private static final long serialVersionUID = 6858405522567342346L;

	
	@Override
	public PagingLoadResult< DisponibilidadeDTO > getDisponibilidades(CenarioDTO cenarioDTO,
		Long entidadeId, String tipoEntidade )
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		
		List< Disponibilidade > list = Disponibilidade.findBy(cenario, entidadeId, tipoEntidade);

		BasePagingLoadResult< DisponibilidadeDTO > result
			= new BasePagingLoadResult< DisponibilidadeDTO >( ConvertBeans.toDisponibilidadeDTO(list) );

		result.setOffset( 0 );
		result.setTotalLength( list.size() );

		return result;
	}
	
	@Override
	public void saveDisponibilidadesDias(CenarioDTO cenarioDTO, List<DisponibilidadeDTO> disponibilidadesDTO, String tipoEntidade)
	{
		for (DisponibilidadeDTO disponibilidadeDTO : disponibilidadesDTO)
		{
			Disponibilidade disponibilidade = null;
			if (tipoEntidade.equals(DisponibilidadeDTO.DISCIPLINA))
			{
				disponibilidade = DisponibilidadeDisciplina.find(disponibilidadeDTO.getDisponibilidadeId());
			}
			if (tipoEntidade.equals(DisponibilidadeDTO.PROFESSOR))
			{
				disponibilidade = DisponibilidadeProfessor.find(disponibilidadeDTO.getDisponibilidadeId());
			}
			if (tipoEntidade.equals(DisponibilidadeDTO.CAMPUS))
			{
				disponibilidade = DisponibilidadeCampus.find(disponibilidadeDTO.getDisponibilidadeId());
			}
			if (tipoEntidade.equals(DisponibilidadeDTO.UNIDADE))
			{
				disponibilidade = DisponibilidadeUnidade.find(disponibilidadeDTO.getDisponibilidadeId());
			}
			if (tipoEntidade.equals(DisponibilidadeDTO.SALA))
			{
				disponibilidade = DisponibilidadeSala.find(disponibilidadeDTO.getDisponibilidadeId());
			}
			
			disponibilidade.setSegunda(disponibilidadeDTO.getSegunda());
			disponibilidade.setTerca(disponibilidadeDTO.getTerca());
			disponibilidade.setQuarta(disponibilidadeDTO.getQuarta());
			disponibilidade.setQuinta(disponibilidadeDTO.getQuinta());
			disponibilidade.setSexta(disponibilidadeDTO.getSexta());
			disponibilidade.setSabado(disponibilidadeDTO.getSabado());
			disponibilidade.setDomingo(disponibilidadeDTO.getDomingo());
			
			disponibilidade.merge();
		}
	}
	
	@Override
	public void saveDisponibilidade(CenarioDTO cenarioDTO, DisponibilidadeDTO disponibilidadeDTO, String tipoEntidade) throws TriedaException
	{
		Cenario cenario = Cenario.find(cenarioDTO.getId(), getInstituicaoEnsinoUser());
		Disponibilidade disponibilidade = ConvertBeans.toDisponibilidade(disponibilidadeDTO, tipoEntidade);
		
		//Checando se ja existe interecessao
		List<Disponibilidade> disponibilidadesExistentes = Disponibilidade.findBy(cenario, disponibilidadeDTO.getEntidadeId(), tipoEntidade);
		for (Disponibilidade disponibilidadeExistente : disponibilidadesExistentes)
		{
			if (checkIntercessaoHorarios(disponibilidadeExistente, disponibilidade.getHorarioInicio(), disponibilidade.getHorarioFim()))
				throw new TriedaException("Ja existe(m) horário(s) que intercede(m) o horário a ser criado");
		}
		
		disponibilidade.persist();
	}
	
	@Override
	public void remove(DisponibilidadeDTO disponibilidadeDTO, String tipoEntidade)
	{
		tipoEntidade = disponibilidadeDTO.getEntidadeTipo();
		Disponibilidade disponibilidade = null;
		if (tipoEntidade.equals(DisponibilidadeDTO.DISCIPLINA))
		{
			disponibilidade = DisponibilidadeDisciplina.find(disponibilidadeDTO.getDisponibilidadeId());
		}
		if (tipoEntidade.equals(DisponibilidadeDTO.PROFESSOR))
		{
			disponibilidade = DisponibilidadeProfessor.find(disponibilidadeDTO.getDisponibilidadeId());
		}
		if (tipoEntidade.equals(DisponibilidadeDTO.CAMPUS))
		{
			disponibilidade = DisponibilidadeCampus.find(disponibilidadeDTO.getDisponibilidadeId());
		}
		if (tipoEntidade.equals(DisponibilidadeDTO.UNIDADE))
		{
			disponibilidade = DisponibilidadeUnidade.find(disponibilidadeDTO.getDisponibilidadeId());
		}
		if (tipoEntidade.equals(DisponibilidadeDTO.SALA))
		{
			disponibilidade = DisponibilidadeSala.find(disponibilidadeDTO.getDisponibilidadeId());
		}
		
		disponibilidade.remove();
	}

	private boolean checkIntercessaoHorarios(Disponibilidade disponibilidade,
			Date horarioInicio, Date horarioFim) {

		Calendar horaInicio = Calendar.getInstance();
		horaInicio.setTime(disponibilidade.getHorarioInicio());
		horaInicio.set(1979,Calendar.NOVEMBER,6);
		
		Calendar horaFim = Calendar.getInstance();
		horaFim.setTime(disponibilidade.getHorarioFim());
		horaFim.set(1979,Calendar.NOVEMBER,6);
		
		Calendar oHoraInicio = Calendar.getInstance();
		oHoraInicio.setTime(horarioInicio);
		oHoraInicio.set(1979,Calendar.NOVEMBER,6);
		
		Calendar oHoraFim = Calendar.getInstance();
		oHoraFim.setTime(horarioFim);
		oHoraFim.set(1979,Calendar.NOVEMBER,6);
		
		return (horaInicio.compareTo(oHoraInicio) < 0 && oHoraInicio.compareTo(horaFim) < 0 ) || // hi1 < hi2 < hf1
				   (horaInicio.compareTo(oHoraFim) < 0  && oHoraFim.compareTo(horaFim) < 0 ) || // hi1 < hf2 < hf1
				   (oHoraInicio.compareTo(horaInicio) < 0 && horaInicio.compareTo(oHoraFim) < 0 ) || // hi2 < hi1 < hf2
				   (oHoraInicio.compareTo(horaFim) < 0 && horaFim.compareTo(oHoraFim) < 0 ) ||
				   (horaInicio.compareTo(oHoraInicio) == 0 && horaFim.compareTo(oHoraFim) == 0 ); // hi1 == hi2 && hf1 = hf2
	}
}
