package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.GrupoSalaDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface GruposSalasServiceAsync {

	void getGrupoSala(Long id, AsyncCallback<GrupoSalaDTO> callback);
	void save(GrupoSalaDTO grupoSalaDTO, AsyncCallback<GrupoSalaDTO> callback);
	void remove(List<GrupoSalaDTO> grupoSalaDTOList, AsyncCallback<Void> callback);
	void getBuscaList(CenarioDTO cenarioDTO, String nome, String codigo, UnidadeDTO unidadeDTO, PagingLoadConfig config, AsyncCallback<PagingLoadResult<GrupoSalaDTO>> callback);
	void getList(CenarioDTO cenarioDTO, BasePagingLoadConfig loadConfig, AsyncCallback<ListLoadResult<GrupoSalaDTO>> callback);
	void getSalas(GrupoSalaDTO grupoSalaDTO, AsyncCallback<ListLoadResult<SalaDTO>> callback);
	void saveSalas(List<SalaDTO> salaDTOList, GrupoSalaDTO grupoSalaDTO, AsyncCallback<Void> callback);
	void getListByUnidade(UnidadeDTO unidadeDTO, AsyncCallback<ListLoadResult<GrupoSalaDTO>> callback);
	void vincula(DisciplinaDTO disciplinaDTO, List<GrupoSalaDTO> gruposSalasDTO, AsyncCallback<Void> callback);
	void desvincula(DisciplinaDTO disciplinaDTO, List<GrupoSalaDTO> gruposSalasDTO, AsyncCallback<Void> callback);
	void getListVinculadas(DisciplinaDTO disciplinaDTO,	AsyncCallback<List<GrupoSalaDTO>> callback);
	void getListNaoVinculadas(CenarioDTO cenarioDTO, DisciplinaDTO disciplinaDTO, AsyncCallback<List<GrupoSalaDTO>> callback);
	
}
