package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.CenarioDTO;
import com.gapso.web.trieda.client.mvp.model.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.client.mvp.model.SemanaLetivaDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface SemanasLetivaServiceAsync {
	
	void getList(PagingLoadConfig config, AsyncCallback<PagingLoadResult<SemanaLetivaDTO>> callback);
	void getList(AsyncCallback<ListLoadResult<SemanaLetivaDTO>> callback);
	void getList(BasePagingLoadConfig loadConfig, AsyncCallback<ListLoadResult<SemanaLetivaDTO>> callback);
	void getBuscaList(String codigo, String descricao, PagingLoadConfig config, AsyncCallback<PagingLoadResult<SemanaLetivaDTO>> callback);
	void save(SemanaLetivaDTO semanaLetivaDTO, AsyncCallback<Void> callback);
	void remove(List<SemanaLetivaDTO> semanaLetivaDTOList, AsyncCallback<Void> callback);
	void getHorariosDisponiveisCenario(SemanaLetivaDTO semanaLetivaDTO, AsyncCallback<PagingLoadResult<HorarioDisponivelCenarioDTO>> callback);
	void saveHorariosDisponiveisCenario(SemanaLetivaDTO semanaLetivaDTO, List<HorarioDisponivelCenarioDTO> listDTO, AsyncCallback<Void> callback);
	void getSemanaLetiva(CenarioDTO cenario, AsyncCallback<SemanaLetivaDTO> callback);
	void getHorariosDisponiveisByCenario(CenarioDTO cenario, AsyncCallback<List<HorarioDisponivelCenarioDTO>> callback);
	
}
