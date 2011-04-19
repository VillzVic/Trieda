package com.gapso.web.trieda.shared.services;

import java.util.List;
import java.util.Map;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.GrupoSalaDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.TipoSalaDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("salas")
public interface SalasService extends RemoteService {
	
	PagingLoadResult<SalaDTO> getList(CampusDTO campusDTO, UnidadeDTO unidadeDTO, PagingLoadConfig config);
	ListLoadResult<SalaDTO> getList();
	void save(SalaDTO salaDTO);
	void remove(List<SalaDTO> salaDTOList);
	ListLoadResult<TipoSalaDTO> getTipoSalaList();
	SalaDTO getSala(Long id);
	TipoSalaDTO getTipoSala(Long id);
	ListLoadResult<SalaDTO> getAndaresList();
	ListLoadResult<SalaDTO> getAndaresList(Long unidadeId);
	Map<String, List<SalaDTO>> getSalasEAndareMap(Long unidadeId);
	List<GrupoSalaDTO> getGruposDeSalas(Long unidadeId);
	ListLoadResult<SalaDTO> getBuscaList(UnidadeDTO unidadeDTO);
	List<HorarioDisponivelCenarioDTO> getHorariosDisponiveis(SalaDTO salaDTO, SemanaLetivaDTO semanaLetivaDTO);
	void saveHorariosDisponiveis(SalaDTO salaDTO, List<HorarioDisponivelCenarioDTO> listDTO);
	ListLoadResult<SalaDTO> getSalasDoAndareList(UnidadeDTO unidade, List<String> andares);
	
}
