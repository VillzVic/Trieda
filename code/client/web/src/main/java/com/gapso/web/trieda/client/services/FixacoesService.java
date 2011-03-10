package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.FixacaoDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("fixacoes")
public interface FixacoesService extends RemoteService {

	FixacaoDTO getFixacao(Long id);
	PagingLoadResult<FixacaoDTO> getBuscaList(String codigo, PagingLoadConfig config);
	void remove(List<FixacaoDTO> fixacaoDTOList);
	PagingLoadResult<HorarioDisponivelCenarioDTO> getHorariosDisponiveis(DisciplinaDTO disciplinaDTO, SalaDTO salaDTO);
	void save(FixacaoDTO fixacaoDTO, List<HorarioDisponivelCenarioDTO> hdcDTOList);
	List<HorarioDisponivelCenarioDTO> getHorariosSelecionados( FixacaoDTO fixacaoDTO);
	
}
