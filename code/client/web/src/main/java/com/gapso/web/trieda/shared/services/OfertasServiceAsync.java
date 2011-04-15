package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.OfertaDTO;
import com.gapso.web.trieda.shared.dtos.TreeNodeDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface OfertasServiceAsync {

	void getOferta(Long id, AsyncCallback<OfertaDTO> callback);
	void getBuscaList(TurnoDTO turnoDTO, CampusDTO campusDTO, CursoDTO cursoDTO, CurriculoDTO curriculoDTO, PagingLoadConfig config, AsyncCallback<PagingLoadResult<OfertaDTO>> callback);
	void save(OfertaDTO ofertaDTO, AsyncCallback<Void> callback);
	void remove(List<OfertaDTO> ofertaDTOList, AsyncCallback<Void> callback);
	void getListByCampusAndTurno(CampusDTO campusDTO, TurnoDTO turnoDTO, AsyncCallback<ListLoadResult<TreeNodeDTO>> callback);
	
}
