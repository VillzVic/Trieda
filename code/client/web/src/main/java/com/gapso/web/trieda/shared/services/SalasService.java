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
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.RelatorioDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.SalaUtilizadaDTO;
import com.gapso.web.trieda.shared.dtos.TipoSalaDTO;
import com.gapso.web.trieda.shared.dtos.UnidadeDTO;
import com.gapso.web.trieda.shared.util.view.RelatorioSalaFiltro;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath( "salas" )
public interface SalasService
	extends RemoteService
{
	PagingLoadResult< SalaDTO > getList( CenarioDTO cenarioDTO, CampusDTO campusDTO, UnidadeDTO unidadeDTO,
			TipoSalaDTO tipoSalaDTO, String operadorCapacidadeInstalada, Integer capacidadeInstalada,
			String operadorCapacidadeMaxima, Integer capacidadeMaxima, String operadorCustoOperacao, Double custoOperacao,
			String  numero, String descricao, String andar,	String codigo, PagingLoadConfig config );
	ListLoadResult< SalaDTO > getList();
	void save( SalaDTO salaDTO );
	void remove( List< SalaDTO > salaDTOList );
	ListLoadResult< TipoSalaDTO > getTipoSalaList( CenarioDTO cenarioDTO );
	SalaDTO getSala( Long id );
	TipoSalaDTO getTipoSala( Long id );
	ListLoadResult< SalaDTO > getAndaresList();
	ListLoadResult< SalaDTO > getAndaresList(Long unidadeId);
	Map< String, List< SalaDTO > > getSalasEAndareMap( Long unidadeId );
	List< GrupoSalaDTO > getGruposDeSalas( Long unidadeId );
	ListLoadResult< SalaDTO > getBuscaList( UnidadeDTO unidadeDTO );
	List< HorarioDisponivelCenarioDTO > getHorariosDisponiveis(SalaDTO salaDTO);
	void saveHorariosDisponiveis( SalaDTO salaDTO, List< HorarioDisponivelCenarioDTO > listDTO );
	ListLoadResult< SalaDTO > getSalasDoAndareList( UnidadeDTO unidade, List< String > andares );
	ListLoadResult<SalaDTO> getAutoCompleteList(CenarioDTO cenarioDTO, BasePagingLoadConfig loadConfig);
	void vincula(SalaDTO salaDTO, List<DisciplinaDTO> disciplinasDTO);
	void desvincula(SalaDTO salaDTO, List<DisciplinaDTO> disciplinasDTO);
	List<DisciplinaDTO> getListDisciplinasVinculadas(SalaDTO salaDTO);
	List<DisciplinaDTO> getListDisciplinasNaoVinculadas(CenarioDTO cenarioDTO, SalaDTO salaDTO);
	void vincula(DisciplinaDTO disciplinaDTO, List<SalaDTO> salasDTO);
	void desvincula(DisciplinaDTO disciplinaDTO, List<SalaDTO> salasDTO);
	List<SalaDTO> getListVinculadas(DisciplinaDTO disciplinaDTO);
	List<SalaDTO> getListNaoVinculadas(CenarioDTO cenarioDTO, DisciplinaDTO disciplinaDTO);
	List<RelatorioDTO> getRelatorio( CenarioDTO cenarioDTO, RelatorioSalaFiltro salaFiltro, RelatorioDTO currentNode );
	ListLoadResult<FaixaCapacidadeSalaDTO> getFaixasCapacidadeSala(CenarioDTO cenarioDTO,	Integer tamanhoFaixa);
	PagingLoadResult<SalaUtilizadaDTO> getSalasUtilizadas( CenarioDTO cenarioDTO, Long campusDTO, RelatorioSalaFiltro salaFiltro, BasePagingLoadConfig loadConfig);
	SalaDTO getProxSala(CenarioDTO cenarioDTO, SalaDTO salaDTO);
	SalaDTO getAntSala(CenarioDTO cenarioDTO, SalaDTO salaDTO);
	void associarDisciplinas(CenarioDTO cenarioDTO, boolean salas, boolean lab);
	ListLoadResult<SalaDTO> getSalasOtimizadas(AtendimentoOperacionalDTO turma, CampusDTO campusDTO, DisciplinaDTO disciplinaDTO);
	
}
