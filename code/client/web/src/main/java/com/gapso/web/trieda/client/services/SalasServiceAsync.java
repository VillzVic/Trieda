package com.gapso.web.trieda.client.services;

import java.util.List;
import java.util.Map;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.GrupoSalaDTO;
import com.gapso.web.trieda.client.mvp.model.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.client.mvp.model.SalaDTO;
import com.gapso.web.trieda.client.mvp.model.TipoSalaDTO;
import com.gapso.web.trieda.client.mvp.model.UnidadeDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface SalasServiceAsync {

	void getList(PagingLoadConfig config, AsyncCallback<PagingLoadResult<SalaDTO>> callback);
	void getList(AsyncCallback<ListLoadResult<SalaDTO>> callback);
	void save(SalaDTO salaDTO, AsyncCallback<Void> callback);
	void remove(List<SalaDTO> salaDTOList, AsyncCallback<Void> callback);
	void getTipoSalaList(AsyncCallback<ListLoadResult<TipoSalaDTO>> callback);
	void getSala(Long id, AsyncCallback<SalaDTO> callback);
	void getTipoSala(Long id, AsyncCallback<TipoSalaDTO> callback);
	void getAndaresList(AsyncCallback<ListLoadResult<SalaDTO>> callback);
	void getAndaresList(Long unidadeId, AsyncCallback<ListLoadResult<SalaDTO>> callback);
	void getSalasDoAndareList(List<String> andares, AsyncCallback<ListLoadResult<SalaDTO>> callback);
	void getSalasEAndareMap(Long unidadeId, AsyncCallback<Map<String, List<SalaDTO>>> callback);
	void getGruposDeSalas(Long unidadeId, AsyncCallback<List<GrupoSalaDTO>> callback);
	void getBuscaList(UnidadeDTO unidadeDTO, AsyncCallback<ListLoadResult<SalaDTO>> callback);
	void getHorariosDisponiveis(SalaDTO salaDTO, AsyncCallback<List<HorarioDisponivelCenarioDTO>> callback);
	void saveHorariosDisponiveis(SalaDTO salaDTO, List<HorarioDisponivelCenarioDTO> listDTO, AsyncCallback<Void> callback);
	
}
