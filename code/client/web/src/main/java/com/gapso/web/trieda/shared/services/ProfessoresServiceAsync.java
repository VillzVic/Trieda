package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.AreaTitulacaoDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorCampusDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.RelatorioDTO;
import com.gapso.web.trieda.shared.dtos.RelatorioQuantidadeDTO;
import com.gapso.web.trieda.shared.dtos.TipoContratoDTO;
import com.gapso.web.trieda.shared.dtos.TipoProfessorDTO;
import com.gapso.web.trieda.shared.dtos.TitulacaoDTO;
import com.gapso.web.trieda.shared.util.view.RelatorioProfessorFiltro;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface ProfessoresServiceAsync
{
	void getProfessor( Long id, AsyncCallback< ProfessorDTO > callback );
	void getList( CenarioDTO cenarioDTO, AsyncCallback< ListLoadResult< ProfessorDTO > > callback );
	void getBuscaList( CenarioDTO cenarioDTO, String cpf, TipoContratoDTO tipoContratoDTO, TitulacaoDTO titulacaoDTO,
		AreaTitulacaoDTO areaTitulacaoDTO, PagingLoadConfig config,
		AsyncCallback< PagingLoadResult< ProfessorDTO > > callback );
	void getTipoContrato( Long id, AsyncCallback< TipoContratoDTO > callback );
	void getTiposContratoAll( CenarioDTO cenarioDTO, AsyncCallback< ListLoadResult< TipoContratoDTO > > callback );
	void getTitulacao( Long id, AsyncCallback< TitulacaoDTO > callback );
	void getTitulacoesAll( CenarioDTO cenarioDTO, AsyncCallback< ListLoadResult< TitulacaoDTO > > callback );
	void save( ProfessorDTO professorDTO, AsyncCallback< Void > callback );
	void remove( List< ProfessorDTO > professorDTOList, AsyncCallback< Void > callback );
	void getHorariosDisponiveis( ProfessorDTO professorDTO, AsyncCallback< List< HorarioDisponivelCenarioDTO > > callback );
	void saveHorariosDisponiveis( ProfessorDTO professorDTO,
		List< HorarioDisponivelCenarioDTO > listDTO, AsyncCallback< Void > callback );
	void getProfessorCampusList( CenarioDTO cenarioDTO, CampusDTO campusDTO, ProfessorDTO professorDTO,
		AsyncCallback< PagingLoadResult< ProfessorCampusDTO > > callback );
	void removeProfessorCampus( List< ProfessorCampusDTO > professorCampusDTOList, AsyncCallback< Void > callback );
	void getProfessoresEmCampus( CampusDTO campusDTO, AsyncCallback< ListLoadResult< ProfessorDTO > > callback );
	void getProfessoresNaoEmCampus( CenarioDTO cenarioDTO, CampusDTO campusDTO, AsyncCallback< List< ProfessorDTO > > callback );
	void salvarProfessorCampus( CampusDTO campusDTO, List< ProfessorDTO > professorDTOList, AsyncCallback< Void > callback );
	void getProfessorCampusByCurrentProfessor( CenarioDTO cenarioDTO, AsyncCallback< PagingLoadResult< ProfessorCampusDTO > > callback );
	
	void geraHabilitacaoParaProfessoresVirtuaisCadastrados(AsyncCallback< Void > callback );
	void getAutoCompleteList(CenarioDTO cenarioDTO, BasePagingLoadConfig loadConfig, String tipoComboBox,
			AsyncCallback<ListLoadResult<ProfessorDTO>> callback);
	void getProfessoresDisciplinasHabilitadas(CenarioDTO cenarioDTO, CampusDTO campusDTO, AsyncCallback<List<RelatorioQuantidadeDTO>> callback);
	void getTiposProfessor(AsyncCallback<ListLoadResult<TipoProfessorDTO>> callback);
	void getProfessoresTitulacoes(CenarioDTO cenarioDTO, CampusDTO campusDTO, AsyncCallback<List<RelatorioQuantidadeDTO>> callback);
	void getProfessoresAreasConhecimento(CenarioDTO cenarioDTO, CampusDTO campusDTO, TipoProfessorDTO tipoProfessorDTO,	AsyncCallback<List<RelatorioQuantidadeDTO>> callback);
	void getRelatorio( CenarioDTO cenarioDTO, RelatorioProfessorFiltro professorFiltro,	RelatorioDTO currentNode, AsyncCallback<List<RelatorioDTO>> callback );
	void getBuscaListAtendimentos( CenarioDTO cenarioDTO, String cpf, Long campusDTO, RelatorioProfessorFiltro professorFiltro, PagingLoadConfig config,
			AsyncCallback<PagingLoadResult<ProfessorDTO>> callback );
	void getBuscaListGradeCheia( CenarioDTO cenarioDTO, String cpf, Long campusDTO, RelatorioProfessorFiltro professorFiltro,
			PagingLoadConfig config, AsyncCallback<PagingLoadResult<ProfessorDTO>> callback );
	void getBuscaListBemAlocados( CenarioDTO cenarioDTO, String cpf, Long campusDTO, RelatorioProfessorFiltro professorFiltro,
			PagingLoadConfig config, AsyncCallback<PagingLoadResult<ProfessorDTO>> callback );
	void getBuscaListMalAlocados( CenarioDTO cenarioDTO, String cpf, Long campusDTO, RelatorioProfessorFiltro professorFiltro,
			PagingLoadConfig config, AsyncCallback<PagingLoadResult<ProfessorDTO>> callback );
	void getBuscaListComJanelas( CenarioDTO cenarioDTO, String cpf, Long campusDTO, RelatorioProfessorFiltro professorFiltro,
			PagingLoadConfig config, AsyncCallback<PagingLoadResult<ProfessorDTO>> callback );
	void getBuscaListDeslocamentoUnidades( CenarioDTO cenarioDTO, String cpf, Long campusDTO, RelatorioProfessorFiltro professorFiltro,
			PagingLoadConfig config, AsyncCallback<PagingLoadResult<ProfessorDTO>> callback );
	void getBuscaListDeslocamentoCampi( CenarioDTO cenarioDTO, String cpf, Long campusDTO, RelatorioProfessorFiltro professorFiltro,
			PagingLoadConfig config, AsyncCallback<PagingLoadResult<ProfessorDTO>> callback );
}
