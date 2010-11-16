package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.FileModel;
import com.gapso.web.trieda.client.mvp.model.OfertaDTO;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.CurriculoDTO;
import com.gapso.web.trieda.client.mvp.model.CursoDTO;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("ofertas")
public interface OfertasService extends RemoteService {
	
	OfertaDTO getOferta(Long id);
	PagingLoadResult<OfertaDTO> getBuscaList(TurnoDTO turnoDTO, CampusDTO campusDTO, CursoDTO cursoDTO, CurriculoDTO curriculoDTO, PagingLoadConfig config);
	void save(OfertaDTO ofertaDTO);
	void remove(List<OfertaDTO> ofertaDTOList);
	ListLoadResult<FileModel> getListByCampusAndTurno(CampusDTO campusDTO, TurnoDTO turnoDTO);
	
}
