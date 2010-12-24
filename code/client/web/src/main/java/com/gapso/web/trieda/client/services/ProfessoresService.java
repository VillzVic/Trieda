package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.AreaTitulacaoDTO;
import com.gapso.web.trieda.client.mvp.model.ProfessorDTO;
import com.gapso.web.trieda.client.mvp.model.TipoContratoDTO;
import com.gapso.web.trieda.client.mvp.model.TitulacaoDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("professores")
public interface ProfessoresService extends RemoteService {

	ProfessorDTO getProfessor(Long id);
	PagingLoadResult<ProfessorDTO> getBuscaList(String cpf, TipoContratoDTO tipoContratoDTO, TitulacaoDTO titulacaoDTO, AreaTitulacaoDTO areaTitulacaoDTO, PagingLoadConfig config);
	TipoContratoDTO getTipoContrato(Long id);
	ListLoadResult<TipoContratoDTO> getTiposContratoAll();
	TitulacaoDTO getTitulacao(Long id);
	ListLoadResult<TitulacaoDTO> getTitulacoesAll();
	void save(ProfessorDTO professorDTO);
	void remove(List<ProfessorDTO> professorDTOList);
	
}
