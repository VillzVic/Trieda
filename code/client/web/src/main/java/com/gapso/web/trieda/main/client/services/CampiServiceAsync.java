package com.gapso.web.trieda.main.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DeslocamentoCampusDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.TreeNodeDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface CampiServiceAsync {
	
	void getListAll(AsyncCallback<ListLoadResult<CampusDTO>> callback);
	void getList(PagingLoadConfig config, AsyncCallback<PagingLoadResult<CampusDTO>> callback);
	void getList(AsyncCallback<ListLoadResult<CampusDTO>> callback);
	void getList(BasePagingLoadConfig loadConfig, AsyncCallback<ListLoadResult<CampusDTO>> callback);
	void save(CampusDTO campusDTO, AsyncCallback<Void> callback);
	void remove(List<CampusDTO> campusDTOList, AsyncCallback<Void> callback);
	void getBuscaList(CenarioDTO cenario, String nome, String codigo, String estadoString, String municipio, String bairro, PagingLoadConfig config, AsyncCallback<PagingLoadResult<CampusDTO>> callback);
	void getCampus(Long id, AsyncCallback<CampusDTO> callback);
	void getDeslocamentos(AsyncCallback<List<DeslocamentoCampusDTO>> callback);
	void getHorariosDisponiveis(CampusDTO campusDTO, AsyncCallback<PagingLoadResult<HorarioDisponivelCenarioDTO>> callback);
	void saveHorariosDisponiveis(CampusDTO campusDTO, List<HorarioDisponivelCenarioDTO> listDTO, AsyncCallback<Void> callback);
	void getResumos(CenarioDTO cenarioDTO, TreeNodeDTO treeNodeDTO, AsyncCallback<List<TreeNodeDTO>> callback);
	void saveDeslocamento(CenarioDTO cenario, List<DeslocamentoCampusDTO> list, AsyncCallback<Void> callback);
	
}
