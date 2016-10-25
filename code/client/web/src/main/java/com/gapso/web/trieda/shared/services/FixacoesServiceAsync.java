package com.gapso.web.trieda.shared.services;

import java.util.Collection;
import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.FixacaoDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface FixacoesServiceAsync
{
	void getFixacao( Long id, AsyncCallback< FixacaoDTO > callback );
	void getBuscaList(CenarioDTO cenarioDTO, String descricao, PagingLoadConfig config, AsyncCallback< PagingLoadResult< FixacaoDTO > > callback );
	void save(FixacaoDTO fixacaoDTO, List<HorarioDisponivelCenarioDTO> hdcDTOList, List<ProfessorDTO> professorDTOList, List<UnidadeDTO> unidadeDTOList, List<SalaDTO> salaDTOList,	AsyncCallback<Void> callback);
	void remove( List< FixacaoDTO > fixacaoDTOList, AsyncCallback< Void > callback );
	void getProfessoresFixados(FixacaoDTO fixacaoDTO, AsyncCallback<List<ProfessorDTO>> callback);
	void getUnidadesFixadas( FixacaoDTO fixacaoDTO, AsyncCallback< List< UnidadeDTO > > callback );
	void getSalasFixadas(FixacaoDTO fixacaoDTO, AsyncCallback<Collection<SalaDTO>> callback);
	void getHorariosSelecionados( FixacaoDTO fixacaoDTO, AsyncCallback< List< HorarioDisponivelCenarioDTO > > callback );
	void getHorariosDisponiveis(DisciplinaDTO disciplinaDTO, AtendimentoOperacionalDTO turma, AsyncCallback<PagingLoadResult<HorarioDisponivelCenarioDTO>> callback);
}
