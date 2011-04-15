package com.gapso.web.trieda.main.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.DeslocamentoUnidadeDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface UnidadesServiceAsync {

	void getList(AsyncCallback<ListLoadResult<UnidadeDTO>> callback);
	void save(UnidadeDTO unidadeDTO, AsyncCallback<Void> callback);
	void remove(List<UnidadeDTO> unidadeDTOList, AsyncCallback<Void> callback);
	void getBuscaList(CampusDTO campusDTO, String nome, String codigo, PagingLoadConfig config, AsyncCallback<PagingLoadResult<UnidadeDTO>> callback);
	void getList(BasePagingLoadConfig loadConfig, AsyncCallback<ListLoadResult<UnidadeDTO>> callback);
	void getUnidade(Long id, AsyncCallback<UnidadeDTO> callback);
	void getDeslocamento(CampusDTO campusDTO, AsyncCallback<List<DeslocamentoUnidadeDTO>> callback);
	void getListByCampus(CampusDTO campusDTO, AsyncCallback<ListLoadResult<UnidadeDTO>> callback);
	void getHorariosDisponiveis(UnidadeDTO unidadeDTO, AsyncCallback<PagingLoadResult<HorarioDisponivelCenarioDTO>> callback);
	void saveHorariosDisponiveis(UnidadeDTO unidadeDTO, List<HorarioDisponivelCenarioDTO> listDTO, AsyncCallback<Void> callback);
	void saveDeslocamento(CampusDTO campus, List<DeslocamentoUnidadeDTO> list, AsyncCallback<Void> callback);
	
}
