package com.gapso.web.trieda.shared.services;

import java.util.List;
import java.util.Map;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.FaixaCapacidadeSalaDTO;
import com.gapso.web.trieda.shared.dtos.GrupoSalaDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.RelatorioDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.SalaUtilizadaDTO;
import com.gapso.web.trieda.shared.dtos.TipoSalaDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.util.view.RelatorioSalaFiltro;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface SalasServiceAsync
{
	void getList( CenarioDTO cenarioDTO, CampusDTO campusDTO, UnidadeDTO unidadeDTO,
			TipoSalaDTO tipoSalaDTO, String operadorCapacidadeInstalada, Integer capacidadeInstalada,
			String operadorCapacidadeMaxima, Integer capacidadeMaxima, String operadorCustoOperacao, Double custoOperacao,
			String  numero, String descricao, String andar, String codigo,
			PagingLoadConfig config, AsyncCallback< PagingLoadResult< SalaDTO > > callback );
	void getList( AsyncCallback< ListLoadResult< SalaDTO > > callback );
	void save( SalaDTO salaDTO, AsyncCallback< Void > callback );
	void remove( List< SalaDTO > salaDTOList, AsyncCallback< Void > callback );
	void getTipoSalaList( CenarioDTO cenarioDTO, AsyncCallback< ListLoadResult< TipoSalaDTO > > callback );
	void getSala( Long id, AsyncCallback< SalaDTO > callback );
	void getTipoSala( Long id, AsyncCallback< TipoSalaDTO > callback );
	void getAndaresList( AsyncCallback<ListLoadResult< SalaDTO > > callback );
	void getAndaresList( Long unidadeId, AsyncCallback< ListLoadResult< SalaDTO > > callback );
	void getSalasEAndareMap( Long unidadeId, AsyncCallback< Map< String, List< SalaDTO > > > callback );
	void getGruposDeSalas( Long unidadeId, AsyncCallback< List< GrupoSalaDTO > > callback );
	void getBuscaList( UnidadeDTO unidadeDTO, AsyncCallback< ListLoadResult< SalaDTO > > callback );
	void getHorariosDisponiveis( SalaDTO salaDTO, AsyncCallback< List< HorarioDisponivelCenarioDTO > > callback );
	void saveHorariosDisponiveis( SalaDTO salaDTO,
		List< HorarioDisponivelCenarioDTO > listDTO, AsyncCallback< Void > callback );
	void getSalasDoAndareList( UnidadeDTO unidade, List< String > andares,
		AsyncCallback< ListLoadResult< SalaDTO > > callback );
	void getAutoCompleteList(CenarioDTO cenarioDTO, BasePagingLoadConfig loadConfig, AsyncCallback<ListLoadResult<SalaDTO>> callback);
	void vincula(SalaDTO salaDTO, List<DisciplinaDTO> disciplinasDTO, AsyncCallback<Void> callback);
	void desvincula(SalaDTO salaDTO, List<DisciplinaDTO> disciplinasDTO, AsyncCallback<Void> callback);
	void getListDisciplinasVinculadas(SalaDTO salaDTO,	AsyncCallback<List<DisciplinaDTO>> callback);
	void getListDisciplinasNaoVinculadas(CenarioDTO cenarioDTO,SalaDTO salaDTO, AsyncCallback<List<DisciplinaDTO>> callback);
	void vincula(DisciplinaDTO disciplinaDTO, List<SalaDTO> salasDTO, AsyncCallback<Void> callback);
	void desvincula(DisciplinaDTO disciplinaDTO, List<SalaDTO> salasDTO, AsyncCallback<Void> callback);
	void getListVinculadas(DisciplinaDTO disciplinaDTO,	AsyncCallback<List<SalaDTO>> callback);
	void getListNaoVinculadas(CenarioDTO cenarioDTO, DisciplinaDTO disciplinaDTO, AsyncCallback<List<SalaDTO>> callback);
	void getRelatorio(CenarioDTO cenarioDTO, RelatorioSalaFiltro salaFiltro, RelatorioDTO currentNode, AsyncCallback<List<RelatorioDTO>> callback);
	void getFaixasCapacidadeSala(CenarioDTO cenarioDTO, Integer tamanhoFaixa, AsyncCallback<ListLoadResult<FaixaCapacidadeSalaDTO>> callback);
	void getSalasUtilizadas(CenarioDTO cenarioDTO, Long campusDTO,
			RelatorioSalaFiltro salaFiltro, BasePagingLoadConfig loadConfig,
			AsyncCallback<PagingLoadResult<SalaUtilizadaDTO>> callback);
	void getProxSala(CenarioDTO cenarioDTO, SalaDTO salaDTO,
			AsyncCallback<SalaDTO> callback);
	void getAntSala(CenarioDTO cenarioDTO, SalaDTO salaDTO,
			AsyncCallback<SalaDTO> callback);
	void associarDisciplinas(CenarioDTO cenarioDTO, boolean salas, boolean lab,
			AsyncCallback<Void> asyncCallback);
	void getSalasPorTurma(AtendimentoOperacionalDTO turma, AsyncCallback< ListLoadResult< SalaDTO > > callback);
}
