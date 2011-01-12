package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.DisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.FixacaoDTO;
import com.gapso.web.trieda.client.mvp.model.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.client.mvp.model.SalaDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;


public interface FixacoesServiceAsync {

	void getFixacao(Long id, AsyncCallback<FixacaoDTO> callback);
	void getBuscaList(String codigo, PagingLoadConfig config, AsyncCallback<PagingLoadResult<FixacaoDTO>> callback);
	void save(FixacaoDTO fixacaoDTO, List<HorarioDisponivelCenarioDTO> hdcDTOList, AsyncCallback<Void> callback);
	void remove(List<FixacaoDTO> fixacaoDTOList, AsyncCallback<Void> callback);
	void getHorariosDisponiveis(DisciplinaDTO disciplinaDTO, SalaDTO salaDTO, AsyncCallback<PagingLoadResult<HorarioDisponivelCenarioDTO>> callback);
	void getHorariosSelecionados(FixacaoDTO fixacaoDTO, AsyncCallback<List<HorarioDisponivelCenarioDTO>> callback);
	
}
