package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.AreaTitulacaoDTO;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.mvp.model.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.client.mvp.model.ProfessorCampusDTO;
import com.gapso.web.trieda.client.mvp.model.ProfessorDTO;
import com.gapso.web.trieda.client.mvp.model.TipoContratoDTO;
import com.gapso.web.trieda.client.mvp.model.TitulacaoDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;


public interface ProfessoresServiceAsync {

	void getProfessor(Long id, AsyncCallback<ProfessorDTO> callback);
	void getList(AsyncCallback<ListLoadResult<ProfessorDTO>> callback);
	void getBuscaList(String cpf, TipoContratoDTO tipoContratoDTO, TitulacaoDTO titulacaoDTO, AreaTitulacaoDTO areaTitulacaoDTO, PagingLoadConfig config, AsyncCallback<PagingLoadResult<ProfessorDTO>> callback);
	void getTipoContrato(Long id, AsyncCallback<TipoContratoDTO> callback);
	void getTiposContratoAll(AsyncCallback<ListLoadResult<TipoContratoDTO>> callback);
	void getTitulacao(Long id, AsyncCallback<TitulacaoDTO> callback);
	void getTitulacoesAll(AsyncCallback<ListLoadResult<TitulacaoDTO>> callback);
	void save(ProfessorDTO professorDTO, AsyncCallback<Void> callback);
	void remove(List<ProfessorDTO> professorDTOList, AsyncCallback<Void> callback);
	void getHorariosDisponiveis(ProfessorDTO professorDTO, AsyncCallback<List<HorarioDisponivelCenarioDTO>> callback);
	void saveHorariosDisponiveis(ProfessorDTO professorDTO, List<HorarioDisponivelCenarioDTO> listDTO, AsyncCallback<Void> callback);
	void getProfessorCampusList(CampusDTO campusDTO, ProfessorDTO professorDTO, AsyncCallback<PagingLoadResult<ProfessorCampusDTO>> callback);
	void removeProfessorCampus(List<ProfessorCampusDTO> professorCampusDTOList, AsyncCallback<Void> callback);
	void getProfessoresEmCampus(CampusDTO campusDTO, AsyncCallback<List<ProfessorDTO>> callback);
	void getProfessoresNaoEmCampus(CampusDTO campusDTO, AsyncCallback<List<ProfessorDTO>> callback);
	void salvarProfessorCampus(CampusDTO campusDTO, List<ProfessorDTO> professorDTOList, AsyncCallback<Void> callback);
	
}
