package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.TreeMap;

import org.springframework.transaction.annotation.Transactional;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.GrupoSala;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.TipoSala;
import com.gapso.trieda.domain.Unidade;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.GrupoSalaDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.TipoSalaDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.services.SalasService;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
@Transactional
public class SalasServiceImpl extends RemoteServiceServlet implements SalasService {

	private static final long serialVersionUID = -5850050305078103981L;

	@Override
	public SalaDTO getSala(Long id) {
		if(id == null) return null;
		return ConvertBeans.toSalaDTO(Sala.find(id));
	}
	
	@Override
	public List<HorarioDisponivelCenarioDTO> getHorariosDisponiveis(SalaDTO salaDTO, SemanaLetivaDTO semanaLetivaDTO) {
		SemanaLetiva semanaLetiva = SemanaLetiva.find(semanaLetivaDTO.getId());
		List<HorarioDisponivelCenario> list = new ArrayList<HorarioDisponivelCenario>(Sala.find(salaDTO.getId()).getHorarios(semanaLetiva));
		List<HorarioDisponivelCenarioDTO> listDTO = ConvertBeans.toHorarioDisponivelCenarioDTO(list);

		// ORDENANDO HORARIOS POR ORDEM DE TURNOS E HORARIOS
		
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
		
		
		return listDTO;
	}
	
	@Override
	public void saveHorariosDisponiveis(SalaDTO salaDTO, List<HorarioDisponivelCenarioDTO> listDTO) {
		List<HorarioDisponivelCenario> listSelecionados = ConvertBeans.toHorarioDisponivelCenario(listDTO);
		Sala sala = Sala.find(salaDTO.getId());
		List<HorarioDisponivelCenario> adicionarList = new ArrayList<HorarioDisponivelCenario> (listSelecionados);
		adicionarList.removeAll(sala.getHorarios());
		List<HorarioDisponivelCenario> removerList = new ArrayList<HorarioDisponivelCenario> (sala.getHorarios());
		removerList.removeAll(listSelecionados);
		for(HorarioDisponivelCenario o : removerList) {
			o.getSalas().remove(sala);
			o.merge();
		}
		for(HorarioDisponivelCenario o : adicionarList) {
			o.getSalas().add(sala);
			o.merge();
		}
	}
	
	@Override
	public TipoSalaDTO getTipoSala(Long id) {
		return ConvertBeans.toTipoSalaDTO(TipoSala.find(id));
	}
	
	@Override
	public ListLoadResult<SalaDTO> getBuscaList(UnidadeDTO unidadeDTO) {
		Unidade unidade = Unidade.find(unidadeDTO.getId());
		List<SalaDTO> listDTO = new ArrayList<SalaDTO>();
		List<Sala> list = Sala.findByUnidade(unidade);
		for(Sala sala : list) {
			listDTO.add(ConvertBeans.toSalaDTO(sala));
		}
		BaseListLoadResult<SalaDTO> result = new BaseListLoadResult<SalaDTO>(listDTO);
		return result;
	}
	
	@Override
	public PagingLoadResult<SalaDTO> getList(CampusDTO campusDTO, UnidadeDTO unidadeDTO, PagingLoadConfig config) {
		List<SalaDTO> list = new ArrayList<SalaDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		Campus campus = campusDTO == null ? null : Campus.find(campusDTO.getId());
		Unidade unidade = unidadeDTO == null ? null : Unidade.find(unidadeDTO.getId());
		for(Sala sala : Sala.find(campus, unidade, config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toSalaDTO(sala));
		}
		BasePagingLoadResult<SalaDTO> result = new BasePagingLoadResult<SalaDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Sala.count(campus, unidade));
		return result;
	}
	
	@Override
	public ListLoadResult<SalaDTO> getAndaresList() {
		return getAndaresList(null);
	}
	@Override
	public ListLoadResult<SalaDTO> getAndaresList(Long unidadeId) {
		List<SalaDTO> list = new ArrayList<SalaDTO>();
		Unidade unidade = Unidade.find(unidadeId);
		for(Sala sala : Sala.findAndaresAll(unidade)) {
			list.add(ConvertBeans.toSalaDTO(sala));
		}
		return new BaseListLoadResult<SalaDTO>(list);
	}
	
	@Override
	public ListLoadResult<SalaDTO> getSalasDoAndareList(UnidadeDTO unidadeDTO, List<String> andares) {
		Unidade unidade = Unidade.find(unidadeDTO.getId());
		List<SalaDTO> list = new ArrayList<SalaDTO>();
		for(Sala sala : Sala.findSalasDoAndarAll(unidade, andares)) {
			list.add(ConvertBeans.toSalaDTO(sala));
		}
		return new BaseListLoadResult<SalaDTO>(list);
	}
	
	@Override
	public Map<String, List<SalaDTO>> getSalasEAndareMap(Long unidadeId) {
		Unidade unidade = Unidade.find(unidadeId);
		List<Sala> salas = Sala.findByUnidade(unidade);
		Map<String, List<SalaDTO>> map = new HashMap<String, List<SalaDTO>>(); 
		for(Sala sala : salas) {
			if(!map.containsKey(sala.getAndar())) {
				map.put(sala.getAndar(), new ArrayList<SalaDTO>());
			}
			SalaDTO salaDTO = ConvertBeans.toSalaDTO(sala);
			map.get(sala.getAndar()).add(salaDTO);
		}
		return map;
	}
	
	@Override
	public ListLoadResult<SalaDTO> getList() {
		List<SalaDTO> list = new ArrayList<SalaDTO>();
		for(Sala sala : Sala.findAll()) {
			list.add(ConvertBeans.toSalaDTO(sala));
		}
		return new BaseListLoadResult<SalaDTO>(list);
	}
	

	@Override
	public List<GrupoSalaDTO> getGruposDeSalas(Long unidadeId) {
		Unidade unidade = Unidade.find(unidadeId);
		List<GrupoSala> grupoSalas = GrupoSala.findByUnidade(unidade);
		List<GrupoSalaDTO> grupoSalasDTO = new ArrayList<GrupoSalaDTO>();
		for(GrupoSala gs : grupoSalas) {
			GrupoSalaDTO gsDTO = ConvertBeans.toGrupoSalaDTO(gs);
			//gsDTO.setName(gsDTO.getCodigo()); TODO: ***********
			//gsDTO.setPath(gsDTO.getCodigo() + "/"); TODO: ***********
			grupoSalasDTO.add(gsDTO);
		}
		return grupoSalasDTO;
	}
	
	@Override
	public void save(SalaDTO salaDTO) {
		Sala sala = ConvertBeans.toSala(salaDTO);
		if(sala.getId() != null && sala.getId() > 0) {
			sala.merge();
		} else {
			sala.persist();
		}
	}
	
	@Override
	public void remove(List<SalaDTO> salaDTOList) {
		for(SalaDTO salaDTO : salaDTOList) {
			Sala.find(salaDTO.getId()).remove();
		}
	}

	@Override
	public ListLoadResult<TipoSalaDTO> getTipoSalaList() {
		// TODO REMOVER AS LINHAS DE BAIXO
		if(TipoSala.count() == 0) {
			TipoSala tipo1 = new TipoSala();
			tipo1.setNome("Sala de Aula");
			tipo1.setDescricao("Sala de Aula");
			tipo1.persist();
			
			TipoSala tipo2 = new TipoSala();
			tipo2.setNome("Laboratório");
			tipo2.setDescricao("Laboratório");
			tipo2.persist();
			
			TipoSala tipo3 = new TipoSala();
			tipo3.setNome("Auditório");
			tipo3.setDescricao("Auditório");
			tipo3.persist();
		}
		
		
		List<TipoSalaDTO> list = new ArrayList<TipoSalaDTO>();
		for(TipoSala tipo : TipoSala.findAll()) {
			list.add(ConvertBeans.toTipoSalaDTO(tipo));
		}
		return new BaseListLoadResult<TipoSalaDTO>(list);
	}
	
}
