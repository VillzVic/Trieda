package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("campi")
public interface CampiService extends RemoteService {
	
	PagingLoadResult<CampusDTO> getList(PagingLoadConfig config);
	ListLoadResult<CampusDTO> getList();
	ListLoadResult<CampusDTO> getList(BasePagingLoadConfig loadConfig);
	void save(CampusDTO campusDTO);
	void remove(List<CampusDTO> campusDTOList);
	PagingLoadResult<CampusDTO> getBuscaList(String nome, String codigo, String estadoString, String municipio, String bairro, PagingLoadConfig config);
	
}
