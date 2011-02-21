package com.gapso.web.trieda.client.services;

import java.util.List;
import java.util.Set;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.CenarioDTO;
import com.gapso.web.trieda.client.mvp.model.FileModel;
import com.gapso.web.trieda.client.mvp.model.SemanaLetivaDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface CenariosServiceAsync {

	void getMasterData(AsyncCallback<CenarioDTO> callback);
	void getCenario(Long id, AsyncCallback<CenarioDTO> callback);
	void getList(PagingLoadConfig config, AsyncCallback<PagingLoadResult<CenarioDTO>> callback);
	void getBuscaList(Integer ano, Integer semestre, PagingLoadConfig config, AsyncCallback<PagingLoadResult<CenarioDTO>> callback);
	void editar(CenarioDTO cenarioDTO, AsyncCallback<Void> callback);
	void criar(CenarioDTO cenarioDTO, SemanaLetivaDTO semanaLetivaDTO, Set<CampusDTO> campiDTO, AsyncCallback<Void> callback);
	void clonar(CenarioDTO cenarioDTO, AsyncCallback<Void> callback);
	void remove(List<CenarioDTO> cenarioDTOList, AsyncCallback<Void> callback);
	void getResumos(CenarioDTO cenario, AsyncCallback<List<FileModel>> callback);
	
}
