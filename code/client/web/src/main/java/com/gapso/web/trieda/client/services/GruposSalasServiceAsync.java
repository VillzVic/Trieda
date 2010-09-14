package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.GrupoSalaDTO;
import com.gapso.web.trieda.client.mvp.model.UnidadeDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface GruposSalasServiceAsync {

	void getGrupoSala(Long id, AsyncCallback<GrupoSalaDTO> callback);
	void save(GrupoSalaDTO grupoSalaDTO, AsyncCallback<Void> callback);
	void remove(List<GrupoSalaDTO> grupoSalaDTOList, AsyncCallback<Void> callback);
	void getBuscaList(String nome, String codigo, UnidadeDTO unidadeDTO, PagingLoadConfig config, AsyncCallback<PagingLoadResult<GrupoSalaDTO>> callback);
	void getList(BasePagingLoadConfig loadConfig, AsyncCallback<ListLoadResult<GrupoSalaDTO>> callback);
	
}
