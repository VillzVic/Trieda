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
import com.gapso.web.trieda.shared.dtos.ProfessorVirtualDTO;
import com.gapso.web.trieda.shared.dtos.RelatorioDTO;
import com.gapso.web.trieda.shared.dtos.RelatorioQuantidadeDTO;
import com.gapso.web.trieda.shared.dtos.TipoContratoDTO;
import com.gapso.web.trieda.shared.dtos.TipoProfessorDTO;
import com.gapso.web.trieda.shared.dtos.TitulacaoDTO;
import com.gapso.web.trieda.shared.util.view.RelatorioProfessorFiltro;
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath( "professores" )
public interface ProfessoresService
	extends RemoteService
{
	ProfessorDTO getProfessor( Long id );
	ListLoadResult< ProfessorDTO > getList( CenarioDTO cenarioDTO );

	PagingLoadResult<ProfessorDTO> getBuscaList(CenarioDTO cenarioDTO,
			String cpf, String nome, TipoContratoDTO tipoContratoDTO,
			TitulacaoDTO titulacaoDTO, AreaTitulacaoDTO areaTitulacaoDTO,
			String operadorCargaHorariaMin, Integer cargaHorariaMin,
			String operadorCargaHorariaMax, Integer cargaHorariaMax,
			String operadorNotaDesempenho, Double notaDesempenho,
			String operadorCargaHorariaAnterior, Integer cargaHorariaAnterior,
			String operadorCustoCreditoSemanal, Double custoCreditoSemanal,
			String operadorMaxDiasSemana, Integer maxDiasSemana,
			String operadorMinCreditosSemanais, Integer minCreditosSemanais,
			String operadorTotalCreditosSemanais,
			Long totalCreditosSemanais, String operadorCargaHorariaSemanal,
			Long cargaHorariaSemanal, PagingLoadConfig config);
	TipoContratoDTO getTipoContrato( Long id );
	ListLoadResult< TipoContratoDTO > getTiposContratoAll( CenarioDTO cenarioDTO );
	TitulacaoDTO getTitulacao( Long id );
	ListLoadResult< TitulacaoDTO > getTitulacoesAll( CenarioDTO cenarioDTO );
	void save( ProfessorDTO professorDTO ) throws TriedaException;
	void remove( List< ProfessorDTO > professorDTOList );
	List< HorarioDisponivelCenarioDTO > getHorariosDisponiveis( ProfessorDTO professorDTO );
	void saveHorariosDisponiveis( ProfessorDTO professorDTO, List< HorarioDisponivelCenarioDTO > listDTO );
	PagingLoadResult< ProfessorCampusDTO > getProfessorCampusList( CenarioDTO cenarioDTO, CampusDTO campusDTO, ProfessorDTO professorDTO );
	void removeProfessorCampus( List< ProfessorCampusDTO > professorCampusDTOList );
	ListLoadResult<ProfessorDTO> getProfessoresEmCampus( CampusDTO campusDTO );
	List< ProfessorDTO > getProfessoresNaoEmCampus( CenarioDTO cenarioDTO, CampusDTO campusDTO );
	void salvarProfessorCampus( CampusDTO campusDTO, List< ProfessorDTO > professorDTOList );
	PagingLoadResult< ProfessorCampusDTO > getProfessorCampusByCurrentProfessor( CenarioDTO cenarioDTO );
	
	void geraHabilitacaoParaProfessoresVirtuaisCadastrados();
	ListLoadResult<ProfessorDTO> getAutoCompleteList(
			CenarioDTO cenarioDTO, BasePagingLoadConfig loadConfig, String tipoComboBox);
	List<RelatorioQuantidadeDTO> getProfessoresDisciplinasHabilitadas( CenarioDTO cenarioDTO, CampusDTO campusDTO );
	ListLoadResult<TipoProfessorDTO> getTiposProfessor();
	List<RelatorioQuantidadeDTO> getProfessoresTitulacoes( CenarioDTO cenarioDTO, CampusDTO campusDTO ) throws TriedaException;
	List<RelatorioQuantidadeDTO> getProfessoresAreasConhecimento( CenarioDTO cenarioDTO, CampusDTO campusDTO, TipoProfessorDTO tipoProfessorDTO, boolean somenteAlocados );
	List<RelatorioDTO> getRelatorio( CenarioDTO cenarioDTO, RelatorioProfessorFiltro professorFiltro, RelatorioDTO currentNode );
	PagingLoadResult<ProfessorDTO> getBuscaListAtendimentos( CenarioDTO cenarioDTO, String cpf, Long campusDTO,
			RelatorioProfessorFiltro professorFiltro, PagingLoadConfig config);
	PagingLoadResult<ProfessorDTO> getBuscaListTodosAtendimentos( CenarioDTO cenarioDTO, String cpf, Long campusDTO,
			RelatorioProfessorFiltro professorFiltro, PagingLoadConfig config);
	PagingLoadResult<ProfessorDTO> getBuscaListGradeCheia( CenarioDTO cenarioDTO, String cpf, Long campusDTO,
			RelatorioProfessorFiltro professorFiltro, PagingLoadConfig config );
	PagingLoadResult<ProfessorDTO> getBuscaListBemAlocados( CenarioDTO cenarioDTO, String cpf, Long campusDTO,
			RelatorioProfessorFiltro professorFiltro, PagingLoadConfig config );
	PagingLoadResult<ProfessorDTO> getBuscaListMalAlocados( CenarioDTO cenarioDTO, String cpf, Long campusDTO,
			RelatorioProfessorFiltro professorFiltro, PagingLoadConfig config );
	PagingLoadResult<ProfessorDTO> getBuscaListComJanelas( CenarioDTO cenarioDTO, String cpf, Long campusDTO,
			RelatorioProfessorFiltro professorFiltro, PagingLoadConfig config );
	PagingLoadResult<ProfessorDTO> getBuscaListDeslocamentoUnidades( CenarioDTO cenarioDTO, String cpf, Long campusDTO,
			RelatorioProfessorFiltro professorFiltro, PagingLoadConfig config );
	PagingLoadResult<ProfessorDTO> getBuscaListDeslocamentoCampi( CenarioDTO cenarioDTO, String cpf, Long campusDTO,
			RelatorioProfessorFiltro professorFiltro, PagingLoadConfig config );
	ProfessorDTO getProxProfessor(CenarioDTO cenarioDTO,
			ProfessorDTO professorDTO, String order);
	ProfessorDTO getAntProfessor(CenarioDTO cenarioDTO,
			ProfessorDTO professorDTO, String order);
	ProfessorVirtualDTO getProxProfessorVirtual(CenarioDTO cenarioDTO,
			ProfessorVirtualDTO professorVirtualDTO, String order);
	ProfessorVirtualDTO getAntProfessorVirtual(CenarioDTO cenarioDTO,
			ProfessorVirtualDTO professorVirtualDTO, String order);
	
}
