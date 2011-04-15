package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.TreeMap;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Unidade;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.services.SemanasLetivaService;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class SemanasLetivaServiceImpl extends RemoteServiceServlet implements SemanasLetivaService {

	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public SemanaLetivaDTO getSemanaLetiva(CenarioDTO cenario) {
		// TODO Pegar a semana letiva do cenário
		return ConvertBeans.toSemanaLetivaDTO(SemanaLetiva.findAll().get(0));
	}
	
	@Override
	public PagingLoadResult<HorarioDisponivelCenarioDTO> getHorariosDisponiveisByCenario(CenarioDTO cenario) {
		// TODO Pegar a semana letiva do cenário
		SemanaLetiva semanaLetiva = SemanaLetiva.findAll().get(0);
		SemanaLetivaDTO semanaLetivaDTO = ConvertBeans.toSemanaLetivaDTO(semanaLetiva);
		return getHorariosDisponiveisCenario(semanaLetivaDTO);
	}
	
	@Override
	public ListLoadResult<SemanaLetivaDTO> getList(BasePagingLoadConfig loadConfig) {
		return getBuscaList(loadConfig.get("query").toString(), null, loadConfig);
	}
	
	@Override
	public PagingLoadResult<SemanaLetivaDTO> getBuscaList(String codigo, String descricao, PagingLoadConfig config) {
		List<SemanaLetivaDTO> list = new ArrayList<SemanaLetivaDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		for(SemanaLetiva semanaLetiva : SemanaLetiva.findBy(codigo, descricao, config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toSemanaLetivaDTO(semanaLetiva));
		}
		BasePagingLoadResult<SemanaLetivaDTO> result = new BasePagingLoadResult<SemanaLetivaDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(SemanaLetiva.count(codigo, descricao));
		return result;
	}
	
	@Override
	public ListLoadResult<SemanaLetivaDTO> getList() {
		List<SemanaLetivaDTO> list = new ArrayList<SemanaLetivaDTO>();
		for(SemanaLetiva semanaLetiva : SemanaLetiva.findAll()) {
			list.add(ConvertBeans.toSemanaLetivaDTO(semanaLetiva));
		}
		return new BaseListLoadResult<SemanaLetivaDTO>(list);
	}

	@Override
	public void save(SemanaLetivaDTO semanaLetivaDTO) {
		SemanaLetiva semanaLetiva = ConvertBeans.toSemanaLetiva(semanaLetivaDTO);
		if(semanaLetiva.getId() != null && semanaLetiva.getId() > 0) {
			semanaLetiva.merge();
		} else {
			semanaLetiva.persist();
		}
	}
	
	@Override
	public void remove(List<SemanaLetivaDTO> semanaLetivaDTOList) {
		for(SemanaLetivaDTO semanaLetivaDTO : semanaLetivaDTOList) {
			ConvertBeans.toSemanaLetiva(semanaLetivaDTO).remove();
		}
	}
	
	@Override
	public PagingLoadResult<HorarioDisponivelCenarioDTO> getHorariosDisponiveisCenario(SemanaLetivaDTO semanaLetivaDTO) {
		SemanaLetiva semanaLetiva = SemanaLetiva.find(semanaLetivaDTO.getId());
		List<HorarioDisponivelCenarioDTO> list = new ArrayList<HorarioDisponivelCenarioDTO>();
		for(HorarioAula o : semanaLetiva.getHorariosAula()) {
			list.add(ConvertBeans.toHorarioDisponivelCenarioDTO(o));
		}
		
		Map<String, List<HorarioDisponivelCenarioDTO>> horariosTurnos = new HashMap<String, List<HorarioDisponivelCenarioDTO>>();
		for(HorarioDisponivelCenarioDTO o : list) {
			List<HorarioDisponivelCenarioDTO> horarios = horariosTurnos.get(o.getTurnoString());
			if(horarios == null) {
				horarios = new ArrayList<HorarioDisponivelCenarioDTO>();
				horariosTurnos.put(o.getTurnoString(), horarios);
			}
			horarios.add(o);
		}
		
		for(Entry<String, List<HorarioDisponivelCenarioDTO>> entry : horariosTurnos.entrySet()) {
			Collections.sort(entry.getValue());
		}
		
		Map<Date, List<String>> horariosFinalTurnos = new TreeMap<Date, List<String>>();
		for(Entry<String, List<HorarioDisponivelCenarioDTO>> entry : horariosTurnos.entrySet()) {
			Date ultimoHorario = entry.getValue().get(entry.getValue().size()-1).getHorario();
			List<String> turnos = horariosFinalTurnos.get(ultimoHorario);
			if (turnos == null) {
				turnos = new ArrayList<String>();
				horariosFinalTurnos.put(ultimoHorario,turnos);
			}
			turnos.add(entry.getKey());
		}
		
		list.clear();
		for(Entry<Date, List<String>> entry : horariosFinalTurnos.entrySet()) {
			for (String turno : entry.getValue()) {
				list.addAll(horariosTurnos.get(turno));
			}
		}
		
		BasePagingLoadResult<HorarioDisponivelCenarioDTO> result = new BasePagingLoadResult<HorarioDisponivelCenarioDTO>(list);
		result.setOffset(0);
		result.setTotalLength(list.size());
		return result;
	}
	
	@Override
	public void saveHorariosDisponiveisCenario(SemanaLetivaDTO semanaLetivaDTO, List<HorarioDisponivelCenarioDTO> listDTO) {
		List<HorarioDisponivelCenario> listSelecionados = ConvertBeans.toHorarioDisponivelCenario(listDTO);
		SemanaLetiva semanaLetiva = SemanaLetiva.find(semanaLetivaDTO.getId());
		
		List<HorarioDisponivelCenario> todosQueJaTinhamList = new ArrayList<HorarioDisponivelCenario>();
		for(HorarioAula horarioAula : semanaLetiva.getHorariosAula()) {
			todosQueJaTinhamList.addAll(horarioAula.getHorariosDisponiveisCenario());
		}
		
		List<HorarioDisponivelCenario> removerList = new ArrayList<HorarioDisponivelCenario>(todosQueJaTinhamList);
		removerList.removeAll(listSelecionados);

		for(HorarioAula horariosAula : semanaLetiva.getHorariosAula()) {
			horariosAula.getHorariosDisponiveisCenario().removeAll(removerList);
			horariosAula.merge();
		}
		
		List<HorarioDisponivelCenario> adicionarList = new ArrayList<HorarioDisponivelCenario> (listSelecionados);
		adicionarList.removeAll(todosQueJaTinhamList);
		
		List<Campus> campi = Campus.findAll();
		List<Unidade> unidades = Unidade.findAll();
		List<Sala> salas = Sala.findAll();
		List<Disciplina> disciplinas = Disciplina.findAll();
		List<Professor> professores = Professor.findAll();
		for(HorarioDisponivelCenario o : adicionarList) {
			o.getCampi().addAll(campi);
			o.getUnidades().addAll(unidades);
			o.getSalas().addAll(salas);
			o.getDisciplinas().addAll(disciplinas);
			o.getProfessores().addAll(professores);
			o.merge();
		}
	}
	
}
