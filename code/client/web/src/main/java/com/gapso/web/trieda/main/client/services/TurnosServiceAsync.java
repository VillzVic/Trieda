package com.gapso.web.trieda.main.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface TurnosServiceAsync {
	
	void getTurno(Long id, AsyncCallback<TurnoDTO> callback);
	void getList(AsyncCallback<ListLoadResult<TurnoDTO>> callback);
	void getList(BasePagingLoadConfig loadConfig, AsyncCallback<ListLoadResult<TurnoDTO>> callback);
	void save(TurnoDTO turnoDTO, AsyncCallback<Void> callback);
	void remove(List<TurnoDTO> turnoDTOList, AsyncCallback<Void> callback);
	void getBuscaList(String nome, Integer tempo, PagingLoadConfig config, AsyncCallback<PagingLoadResult<TurnoDTO>> callback);
	void getListByCampus(CampusDTO campusDTO, AsyncCallback<ListLoadResult<TurnoDTO>> callback);
	
}
