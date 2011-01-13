package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.DeslocamentoUnidadeDTO;
import com.gapso.web.trieda.client.mvp.model.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.client.mvp.model.UnidadeDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface UnidadesServiceAsync {

	void getList(PagingLoadConfig config, AsyncCallback<PagingLoadResult<UnidadeDTO>> callback);
	void getList(AsyncCallback<ListLoadResult<UnidadeDTO>> callback);
	void save(UnidadeDTO unidadeDTO, AsyncCallback<Void> callback);
	void remove(List<UnidadeDTO> unidadeDTOList, AsyncCallback<Void> callback);
	void getBuscaList(CampusDTO campusDTO, String nome, String codigo, PagingLoadConfig config, AsyncCallback<PagingLoadResult<UnidadeDTO>> callback);
	void getList(BasePagingLoadConfig loadConfig, AsyncCallback<ListLoadResult<UnidadeDTO>> callback);
	void getUnidade(Long id, AsyncCallback<UnidadeDTO> callback);
	void getDeslocamento(CampusDTO campusDTO, AsyncCallback<List<DeslocamentoUnidadeDTO>> callback);
	void saveDeslocamento(List<DeslocamentoUnidadeDTO> list, AsyncCallback<Void> callback);
	void getListByCampus(CampusDTO campusDTO, AsyncCallback<ListLoadResult<UnidadeDTO>> callback);
	void getHorariosDisponiveis(UnidadeDTO unidadeDTO, AsyncCallback<PagingLoadResult<HorarioDisponivelCenarioDTO>> callback);
	void saveHorariosDisponiveis(UnidadeDTO unidadeDTO, List<HorarioDisponivelCenarioDTO> listDTO, AsyncCallback<Void> callback);
	
}
