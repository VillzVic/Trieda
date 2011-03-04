package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.util.view.TriedaException;
import com.gapso.web.trieda.shared.dtos.DeslocamentoUnidadeDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("unidades")
public interface UnidadesService extends RemoteService {
	
	ListLoadResult<UnidadeDTO> getList();
	void save(UnidadeDTO unidadeDTO);
	void remove(List<UnidadeDTO> unidadeDTOList) throws TriedaException;
	PagingLoadResult<UnidadeDTO> getBuscaList(CampusDTO campusDTO, String nome, String codigo, PagingLoadConfig config);
	ListLoadResult<UnidadeDTO> getList(BasePagingLoadConfig loadConfig);
	UnidadeDTO getUnidade(Long id);
	List<DeslocamentoUnidadeDTO> getDeslocamento(CampusDTO campusDTO);
	ListLoadResult<UnidadeDTO> getListByCampus(CampusDTO campusDTO);
	PagingLoadResult<HorarioDisponivelCenarioDTO> getHorariosDisponiveis(UnidadeDTO unidadeDTO);
	void saveHorariosDisponiveis(UnidadeDTO unidadeDTO, List<HorarioDisponivelCenarioDTO> listDTO);
	void saveDeslocamento(CampusDTO campus, List<DeslocamentoUnidadeDTO> list);
}
