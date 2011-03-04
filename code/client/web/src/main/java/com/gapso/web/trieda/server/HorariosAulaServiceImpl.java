package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Turno;
import com.gapso.trieda.domain.Unidade;
import com.gapso.trieda.misc.Semanas;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.gapso.web.trieda.client.services.HorariosAulaService;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.HorarioAulaDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class HorariosAulaServiceImpl extends RemoteServiceServlet implements HorariosAulaService {

	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public PagingLoadResult<HorarioAulaDTO> getBuscaList(SemanaLetivaDTO semanaLetivaDTO, TurnoDTO turnoDTO, Date horario, PagingLoadConfig config) {
		List<HorarioAulaDTO> list = new ArrayList<HorarioAulaDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		SemanaLetiva semanaLetiva = (semanaLetivaDTO == null)? null : ConvertBeans.toSemanaLetiva(semanaLetivaDTO);
		Turno turno = (turnoDTO == null)? null : ConvertBeans.toTurno(turnoDTO);
		for(HorarioAula horarioAula : HorarioAula.findBy(semanaLetiva, turno, horario, config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toHorarioAulaDTO(horarioAula));
		}
		BasePagingLoadResult<HorarioAulaDTO> result = new BasePagingLoadResult<HorarioAulaDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(HorarioAula.count(semanaLetiva, turno, horario));
		return result;
	}

	@Override
	public void save(HorarioAulaDTO horarioAulaDTO) {
		HorarioAula horarioDeAula = ConvertBeans.toHorarioAula(horarioAulaDTO);
		if(horarioDeAula.getId() != null && horarioDeAula.getId() > 0) {
			horarioDeAula.merge();
		} else {
			horarioDeAula.persist();

		    List<Campus> campi = Campus.findAll();
		    List<Unidade> unidades = Unidade.findAll();
		    List<Sala> salas = Sala.findAll();
		    List<Disciplina> disciplinas = Disciplina.findAll();
		    List<Professor> professores = Professor.findAll();
			
			for(Semanas semana : Semanas.values()) {
				if(semana == Semanas.SAB || semana == Semanas.DOM) continue;
				HorarioDisponivelCenario hdc = new HorarioDisponivelCenario();
				hdc.setSemana(semana);
				hdc.setHorarioAula(horarioDeAula);
				
				hdc.getCampi().addAll(campi);
				hdc.getUnidades().addAll(unidades);
				hdc.getSalas().addAll(salas);
				hdc.getDisciplinas().addAll(disciplinas);
				hdc.getProfessores().addAll(professores);
				
				hdc.persist();
			}
		}
	}
	
	@Override
	public void remove(List<HorarioAulaDTO> horariosAulaDTOList) {
		for(HorarioAulaDTO horarioAulaDTO : horariosAulaDTOList) {
			ConvertBeans.toHorarioAula(horarioAulaDTO).remove();
		}
	}
	
	@Override
	public void removeWithHorario(HorarioAulaDTO horarioAulaDTO) {
		SemanaLetiva semanaLetiva = SemanaLetiva.find(horarioAulaDTO.getSemanaLetivaId());
		Turno turno = Turno.find(horarioAulaDTO.getTurnoId());
		Calendar cal1 = Calendar.getInstance();
		cal1.setTime(horarioAulaDTO.getInicio());
		
		for(HorarioAula horarioAula : HorarioAula.findHorarioAulasBySemanaLetivaAndTurno(semanaLetiva, turno)) {
			Calendar cal2 = Calendar.getInstance();
			cal2.setTime(horarioAula.getHorario());
			if(cal1.get(Calendar.HOUR) == cal2.get(Calendar.HOUR) && cal1.get(Calendar.MINUTE) == cal2.get(Calendar.MINUTE)) {
				horarioAula.remove();
			}
		}
	}

}
