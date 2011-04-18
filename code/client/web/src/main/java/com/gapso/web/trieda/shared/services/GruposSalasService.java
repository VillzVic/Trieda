package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.GrupoSalaDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("gruposSalas")
public interface GruposSalasService extends RemoteService {
	
	GrupoSalaDTO getGrupoSala(Long id);
	GrupoSalaDTO save(GrupoSalaDTO grupoSalaDTO);
	void remove(List<GrupoSalaDTO> grupoSalaDTOList);
	PagingLoadResult<GrupoSalaDTO> getBuscaList(String nome, String codigo, UnidadeDTO unidadeDTO, PagingLoadConfig config);
	ListLoadResult<GrupoSalaDTO> getList(BasePagingLoadConfig loadConfig);
	ListLoadResult<SalaDTO> getSalas(GrupoSalaDTO grupoSalaDTO);
	void saveSalas(List<SalaDTO> salaDTOList, GrupoSalaDTO grupoSalaDTO);
	ListLoadResult<GrupoSalaDTO> getListByUnidade(UnidadeDTO unidadeDTO);
	
}