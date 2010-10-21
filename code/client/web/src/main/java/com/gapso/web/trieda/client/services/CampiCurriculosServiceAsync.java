package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.CampusCurriculoDTO;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.CurriculoDTO;
import com.gapso.web.trieda.client.mvp.model.CursoDTO;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface CampiCurriculosServiceAsync {

	void getCampusCurriculo(Long id, AsyncCallback<CampusCurriculoDTO> callback);
	void getBuscaList(TurnoDTO turnoDTO, CampusDTO campusDTO, CursoDTO cursoDTO, CurriculoDTO curriculoDTO, PagingLoadConfig config, AsyncCallback<PagingLoadResult<CampusCurriculoDTO>> callback);
	void save(CampusCurriculoDTO campusCurriculoDTO, AsyncCallback<Void> callback);
	void remove(List<CampusCurriculoDTO> campusCurriculoDTOList, AsyncCallback<Void> callback);
	
}
