package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.Map.Entry;

import org.springframework.transaction.annotation.Transactional;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.DeslocamentoUnidade;
import com.gapso.trieda.domain.HorarioDisponivelCenario;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Unidade;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.DeslocamentoUnidadeDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.services.UnidadesService;
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
@Transactional
public class UnidadesServiceImpl extends RemoteServiceServlet implements UnidadesService {

	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public UnidadeDTO getUnidade(Long id) {
		if(id == null) return null;
		return ConvertBeans.toUnidadeDTO(Unidade.find(id));
	}
	
	@Override
	public PagingLoadResult<HorarioDisponivelCenarioDTO> getHorariosDisponiveis(UnidadeDTO unidadeDTO, SemanaLetivaDTO semanaLetivaDTO) {
		SemanaLetiva semanaLetiva = SemanaLetiva.getByOficial();
		List<HorarioDisponivelCenario> list = new ArrayList<HorarioDisponivelCenario>(Unidade.find(unidadeDTO.getId()).getHorarios(semanaLetiva));
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
	
	@Override
	public void saveHorariosDisponiveis(UnidadeDTO unidadeDTO, List<HorarioDisponivelCenarioDTO> listDTO) {
		List<HorarioDisponivelCenario> listSelecionados = ConvertBeans.toHorarioDisponivelCenario(listDTO);
		Unidade unidade = Unidade.find(unidadeDTO.getId());
		List<Sala> salas = Sala.findByUnidade(unidade);
		SemanaLetiva semanaLetiva = SemanaLetiva.getByOficial();
		
		List<HorarioDisponivelCenario> removerList = new ArrayList<HorarioDisponivelCenario> (unidade.getHorarios(semanaLetiva));
		removerList.removeAll(listSelecionados);
		for(HorarioDisponivelCenario o : removerList) {
			o.getUnidades().remove(unidade);
			o.getSalas().removeAll(salas);
			o.merge();
		}
		
		List<HorarioDisponivelCenario> adicionarList = new ArrayList<HorarioDisponivelCenario> (listSelecionados);
		adicionarList.removeAll(unidade.getHorarios(semanaLetiva));
		for(HorarioDisponivelCenario o : adicionarList) {
			o.getUnidades().add(unidade);
			o.getSalas().addAll(salas);
			o.merge();
		}
	}
	
	@Override
	public ListLoadResult<UnidadeDTO> getList(BasePagingLoadConfig loadConfig) {
		Long campusID = loadConfig.get("campusId");
		System.out.println("Buscando: "+ campusID);
		CampusDTO campusDTO = null;
		if(campusID != null) {
			campusDTO = ConvertBeans.toCampusDTO(Campus.find(campusID));
		}
		return getBuscaList(campusDTO, null, loadConfig.get("query").toString(), loadConfig);
	}
	
	@Override
	public PagingLoadResult<UnidadeDTO> getBuscaList(CampusDTO campusDTO, String nome, String codigo, PagingLoadConfig config) {
		List<UnidadeDTO> list = new ArrayList<UnidadeDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		Campus campus = null;
		if(campusDTO != null) {
			campus = ConvertBeans.toCampus(campusDTO);
		}
		for(Unidade unidade : Unidade.findBy(campus, nome, codigo, config.getOffset(), config.getLimit(), orderBy)) {
			UnidadeDTO unidadeDTO = ConvertBeans.toUnidadeDTO(unidade);
			unidadeDTO.setCapSalas(Unidade.getCapacidadeMedia(unidade));
			list.add(unidadeDTO);
		}
		BasePagingLoadResult<UnidadeDTO> result = new BasePagingLoadResult<UnidadeDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Unidade.count(campus, nome, codigo));
		return result;
	}
	
	@Override
	public ListLoadResult<UnidadeDTO> getListByCampus(CampusDTO campusDTO) {
		List<UnidadeDTO> list = new ArrayList<UnidadeDTO>();
		Campus campus = Campus.find(campusDTO.getId());
		List<Unidade> unidades = Unidade.findByCampus(campus);
		for(Unidade unidade : unidades) {
			list.add(ConvertBeans.toUnidadeDTO(unidade));
		}
		return new BaseListLoadResult<UnidadeDTO>(list);
	}
	
	@Override
	public ListLoadResult<UnidadeDTO> getList() {
		List<UnidadeDTO> list = new ArrayList<UnidadeDTO>();
		for(Unidade unidade : Unidade.findAll()) {
			list.add(ConvertBeans.toUnidadeDTO(unidade));
		}
		return new BaseListLoadResult<UnidadeDTO>(list);
	}
	
	@Override
	public void save(UnidadeDTO unidadeDTO) {
		Unidade unidade = ConvertBeans.toUnidade(unidadeDTO);
		if(unidade.getId() != null && unidade.getId() > 0) {
			unidade.merge();
		} else {
			unidade.persist();
		}
	}
	
	@Override
	public void remove(List<UnidadeDTO> unidadeDTOList) throws TriedaException {
		try {
			for(UnidadeDTO unidadeDTO : unidadeDTOList) {
				ConvertBeans.toUnidade(unidadeDTO).remove();
			}
		} catch (Exception e) {
			throw new TriedaException(e);
		}
	}
	
	@Override
	public List<DeslocamentoUnidadeDTO> getDeslocamento(CampusDTO campusDTO) {
		List<DeslocamentoUnidadeDTO> list = new ArrayList<DeslocamentoUnidadeDTO>();
		if(campusDTO != null) {
			Campus campus = Campus.find(campusDTO.getId());
			Set<Unidade> listUnidades = campus.getUnidades();
			for(Unidade unidade : listUnidades) {
				list.add(ConvertBeans.toDeslocamentoUnidadeDTO(unidade, listUnidades));
			}
		}
		Collections.sort(list, new Comparator<DeslocamentoUnidadeDTO>() {
			@Override
			public int compare(DeslocamentoUnidadeDTO o1, DeslocamentoUnidadeDTO o2) {
				return o1.get("origemString").toString().compareToIgnoreCase(o2.get("origemString").toString());
			}
		});
		return list;
	}
	
	@Override
	public void saveDeslocamento(CampusDTO campusDTO, List<DeslocamentoUnidadeDTO> list) {
		List<DeslocamentoUnidade> deslocamentos = DeslocamentoUnidade.findAllByCampus(Campus.find(campusDTO.getId()));
		for(DeslocamentoUnidade deslocamento : deslocamentos) {
			deslocamento.remove();
		}
		for(DeslocamentoUnidadeDTO deslocamentoUnidadeDTO : list) {
			List<DeslocamentoUnidade> deslUniList = ConvertBeans.toDeslocamentoUnidade(deslocamentoUnidadeDTO);
			for(DeslocamentoUnidade desl : deslUniList) {
				desl.persist();
			}
		}
	}
}
