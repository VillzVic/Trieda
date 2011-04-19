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
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface SalasServiceAsync {

	void getList(CampusDTO campusDTO, UnidadeDTO unidadeDTO, PagingLoadConfig config, AsyncCallback<PagingLoadResult<SalaDTO>> callback);
	void getList(AsyncCallback<ListLoadResult<SalaDTO>> callback);
	void save(SalaDTO salaDTO, AsyncCallback<Void> callback);
	void remove(List<SalaDTO> salaDTOList, AsyncCallback<Void> callback);
	void getTipoSalaList(AsyncCallback<ListLoadResult<TipoSalaDTO>> callback);
	void getSala(Long id, AsyncCallback<SalaDTO> callback);
	void getTipoSala(Long id, AsyncCallback<TipoSalaDTO> callback);
	void getAndaresList(AsyncCallback<ListLoadResult<SalaDTO>> callback);
	void getAndaresList(Long unidadeId, AsyncCallback<ListLoadResult<SalaDTO>> callback);
	void getSalasEAndareMap(Long unidadeId, AsyncCallback<Map<String, List<SalaDTO>>> callback);
	void getGruposDeSalas(Long unidadeId, AsyncCallback<List<GrupoSalaDTO>> callback);
	void getBuscaList(UnidadeDTO unidadeDTO, AsyncCallback<ListLoadResult<SalaDTO>> callback);
	void getHorariosDisponiveis(SalaDTO salaDTO, SemanaLetivaDTO semanaLetivaDTO, AsyncCallback<List<HorarioDisponivelCenarioDTO>> callback);
	void saveHorariosDisponiveis(SalaDTO salaDTO, List<HorarioDisponivelCenarioDTO> listDTO, AsyncCallback<Void> callback);
	void getSalasDoAndareList(UnidadeDTO unidade, List<String> andares, AsyncCallback<ListLoadResult<SalaDTO>> callback);
	
}
