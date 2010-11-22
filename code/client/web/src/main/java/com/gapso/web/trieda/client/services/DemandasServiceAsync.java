package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.CurriculoDTO;
import com.gapso.web.trieda.client.mvp.model.CursoDTO;
import com.gapso.web.trieda.client.mvp.model.DemandaDTO;
import com.gapso.web.trieda.client.mvp.model.DisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;


public interface DemandasServiceAsync {

	void getBuscaList(CampusDTO campusDTO, CursoDTO cursoDTO, CurriculoDTO curriculoDTO, TurnoDTO turnoDTO, DisciplinaDTO disciplinaDTO, PagingLoadConfig config, AsyncCallback<PagingLoadResult<DemandaDTO>> callback);
	void save(DemandaDTO demandaDTO, AsyncCallback<Void> callback);
	void remove(List<DemandaDTO> demandaDTOList, AsyncCallback<Void> callback);

}
