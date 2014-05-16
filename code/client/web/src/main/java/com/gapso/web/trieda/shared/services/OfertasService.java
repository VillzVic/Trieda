package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDemandaDTO;
import com.gapso.web.trieda.shared.dtos.OfertaDTO;
import com.gapso.web.trieda.shared.dtos.TreeNodeDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("ofertas")
public interface OfertasService extends RemoteService {

	ListLoadResult<OfertaDTO> getListAll( CenarioDTO cenarioDTO );
	OfertaDTO getOferta(Long id);
	PagingLoadResult<OfertaDTO> getBuscaList(CenarioDTO cenarioDTO,TurnoDTO turnoDTO, CampusDTO campusDTO,
			CursoDTO cursoDTO, CurriculoDTO curriculoDTO, 
			String receitaCreditoOperador,Double receitaCredito, PagingLoadConfig config);
	void save(OfertaDTO ofertaDTO);
	void remove(List<OfertaDTO> ofertaDTOList) throws TriedaException;
	ListLoadResult<TreeNodeDTO> getListByCampusAndTurno(CampusDTO campusDTO, TurnoDTO turnoDTO);
	ListLoadResult<DisciplinaDemandaDTO> getDisciplinas(DemandaDTO demandaDTO, OfertaDTO ofertaDTO, Integer periodo);
	
}
