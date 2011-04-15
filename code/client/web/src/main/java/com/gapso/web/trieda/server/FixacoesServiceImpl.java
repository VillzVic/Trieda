package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.TreeMap;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.Fixacao;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.Sala;
import com.gapso.web.trieda.main.client.services.FixacoesService;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.FixacaoDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class FixacoesServiceImpl extends RemoteServiceServlet implements FixacoesService {

	private static final long serialVersionUID = -594991176048559553L;

	@Override
	public FixacaoDTO getFixacao(Long id) {
		return ConvertBeans.toFixacaoDTO(Fixacao.find(id));
	}
	
	@Override
	public PagingLoadResult<FixacaoDTO> getBuscaList(String codigo, PagingLoadConfig config) {
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		List<FixacaoDTO> list = new ArrayList<FixacaoDTO>();
		for(Fixacao fixacao : Fixacao.findBy(codigo, config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toFixacaoDTO(fixacao));
		}
		BasePagingLoadResult<FixacaoDTO> result = new BasePagingLoadResult<FixacaoDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Fixacao.count());
		return result;
	}
	
	@Override
	public void save(FixacaoDTO fixacaoDTO, List<HorarioDisponivelCenarioDTO> hdcDTOList) {
		Fixacao.entityManager().clear();
		Fixacao fixacao = ConvertBeans.toFixacao(fixacaoDTO);
		List<HorarioDisponivelCenario> listSelecionados = ConvertBeans.toHorarioDisponivelCenario(hdcDTOList);
		List<HorarioDisponivelCenario> adicionarList = new ArrayList<HorarioDisponivelCenario>(listSelecionados);
		
		if(fixacao.getId() != null && fixacao.getId() > 0) {
			fixacao.merge();
		} else {
			fixacao.persist();
		}
		Long id = fixacao.getId();
		Fixacao.entityManager().clear();
		
		fixacao = Fixacao.find(id);
		
		adicionarList.removeAll(fixacao.getHorarios());
		for(HorarioDisponivelCenario o : adicionarList) {
			HorarioDisponivelCenario hdc = HorarioDisponivelCenario.find(o.getId());
			hdc.getFixacoes().add(fixacao);
			hdc.merge();
		}
		
		List<HorarioDisponivelCenario> removerList = new ArrayList<HorarioDisponivelCenario>(fixacao.getHorarios());
		removerList.removeAll(listSelecionados);
		for(HorarioDisponivelCenario o : removerList) {
			o.getFixacoes().remove(fixacao);
			o.merge();
		}
	}

	@Override
	public void remove(List<FixacaoDTO> fixacaoDTOList) {
		for(FixacaoDTO fixacaoDTO : fixacaoDTOList) {
			Fixacao.find(fixacaoDTO.getId()).remove();
		}
	}
	
	@Override
	public List<HorarioDisponivelCenarioDTO> getHorariosSelecionados(FixacaoDTO fixacaoDTO) {
		Fixacao fixacao = Fixacao.find(fixacaoDTO.getId());
		return ConvertBeans.toHorarioDisponivelCenarioDTO(new ArrayList<HorarioDisponivelCenario>(fixacao.getHorarios()));
	}

	@Override
	public PagingLoadResult<HorarioDisponivelCenarioDTO> getHorariosDisponiveis(ProfessorDTO professorDTO, DisciplinaDTO disciplinaDTO, SalaDTO salaDTO) {
		if(disciplinaDTO == null && salaDTO == null && professorDTO == null) {
			return new BasePagingLoadResult<HorarioDisponivelCenarioDTO>(new ArrayList<HorarioDisponivelCenarioDTO>());
		}
		Set<HorarioDisponivelCenario> professorHorarios = null;
		Set<HorarioDisponivelCenario> disciplinaHorarios = null;
		Set<HorarioDisponivelCenario> salaHorarios = null;
		
		if(professorDTO != null) {
			Professor professor = Professor.find(professorDTO.getId());
			professorHorarios = professor.getHorarios();
		}
		
		if(disciplinaDTO != null) {
			Disciplina disciplina = Disciplina.find(disciplinaDTO.getId());
			disciplinaHorarios = disciplina.getHorarios();
		}
		
		if(salaDTO != null) {
			Sala sala = Sala.find(salaDTO.getId());
			salaHorarios = sala.getHorarios();
		}
		
		List<HorarioDisponivelCenario> list = intercessaoHorarios(professorHorarios, disciplinaHorarios, salaHorarios);
		List<HorarioDisponivelCenarioDTO> listDTO = ConvertBeans.toHorarioDisponivelCenarioDTO(list);

		Map<String, List<HorarioDisponivelCenarioDTO>> horariosTurnos = new HashMap<String, List<HorarioDisponivelCenarioDTO>>();
		for(HorarioDisponivelCenarioDTO o : listDTO) {
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
		
		listDTO.clear();
		for(Entry<Date, List<String>> entry : horariosFinalTurnos.entrySet()) {
			for (String turno : entry.getValue()) {
				listDTO.addAll(horariosTurnos.get(turno));
			}
		}
		
		return new BasePagingLoadResult<HorarioDisponivelCenarioDTO>(listDTO);
	}

	private List<HorarioDisponivelCenario> intercessaoHorarios(Set<HorarioDisponivelCenario> horario1, Set<HorarioDisponivelCenario> horario2, Set<HorarioDisponivelCenario> horario3) {
		List<HorarioDisponivelCenario> horarios = new ArrayList<HorarioDisponivelCenario>();
		
		if(horarios.size() == 0 && horario1 != null) horarios.addAll(horario1);
		if(horarios.size() == 0 && horario2 != null) horarios.addAll(horario2);
		if(horarios.size() == 0 && horario3 != null) horarios.addAll(horario3);
		
		if(horario1 != null) horarios.retainAll(horario1);
		if(horario2 != null) horarios.retainAll(horario2);
		if(horario3 != null) horarios.retainAll(horario3);
		return horarios;
	}
	
}
