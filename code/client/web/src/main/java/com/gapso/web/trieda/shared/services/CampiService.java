package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.BaseTreeModel;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.DeslocamentoCampusDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.TreeNodeDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath( "campi" )
public interface CampiService
	extends RemoteService
{
	ListLoadResult<CampusDTO> getCampiNaoSelecionadosParaOtimizacao(CenarioDTO cenarioDTO, List<CampusDTO> campiSelecionados);
	PagingLoadResult< CampusDTO > getList( PagingLoadConfig config );
	ListLoadResult< CampusDTO > getListAll( CenarioDTO cenarioDTO );
	ListLoadResult< CampusDTO > getList();
	ListLoadResult< CampusDTO > getList( BasePagingLoadConfig loadConfig );
	void save( CampusDTO campusDTO );
	void remove( List< CampusDTO > campusDTOList );
	PagingLoadResult<CampusDTO> getBuscaList( CenarioDTO cenario, String nome, String codigo,
		String estadoString, String municipio, String bairro,
		String operadorCustoMedioCredito, Double custoMedioCredito, Boolean otimizadoOperacional, Boolean otimizadoTatico,
		PagingLoadConfig config );
	CampusDTO getCampus( Long id );
	List< DeslocamentoCampusDTO > getDeslocamentos( CenarioDTO cenarioDTO );
	PagingLoadResult< HorarioDisponivelCenarioDTO > getHorariosDisponiveis( CampusDTO campusDTO );
	void saveHorariosDisponiveis( CampusDTO campusDTO,
		List< HorarioDisponivelCenarioDTO > listDTO );
	List< TreeNodeDTO > getResumos( CenarioDTO cenarioDTO, TreeNodeDTO treeNodeDTO );
	void saveDeslocamento( CenarioDTO cenario, List< DeslocamentoCampusDTO > list );
	ListLoadResult< CampusDTO > getListByCurriculo( CenarioDTO cenarioDTO, CurriculoDTO curriculoDTO );
	ListLoadResult<CampusDTO> getListAllCampiTodos( CenarioDTO cenarioDTO );
	List<BaseTreeModel> getCenariosComparados(List<CenarioDTO> cenariosDTO);
	ListLoadResult<CampusDTO> getCampusPorCenario(CenarioDTO cenario);
}
