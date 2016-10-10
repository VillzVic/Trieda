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
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.TreeNodeDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface CampiServiceAsync {
	
	void getCampiNaoSelecionadosParaOtimizacao(CenarioDTO cenarioDTO, List<CampusDTO> campiSelecionados, AsyncCallback<ListLoadResult<CampusDTO>> callback);
	
	void getListAll( CenarioDTO cenarioDTO, AsyncCallback<ListLoadResult< CampusDTO > > callback );
	void getList( PagingLoadConfig config, AsyncCallback< PagingLoadResult< CampusDTO > > callback );
	void getList( AsyncCallback< ListLoadResult< CampusDTO > > callback );
	void getList( BasePagingLoadConfig loadConfig, AsyncCallback< ListLoadResult< CampusDTO > > callback );
	void save( CampusDTO campusDTO, AsyncCallback< Void > callback );
	void remove( List< CampusDTO > campusDTOList, AsyncCallback< Void > callback );
	void getBuscaList( CenarioDTO cenario, String nome, String codigo, String estadoString,
		String municipio, String bairro,
		String operadorCustoMedioCredito, Double custoMedioCredito, Boolean otimizadoOperacional, Boolean otimizadoTatico,
		PagingLoadConfig config,
		AsyncCallback< PagingLoadResult< CampusDTO > > callback );
	void getCampus( Long id, AsyncCallback< CampusDTO > callback );
	void getDeslocamentos( CenarioDTO cenarioDTO, AsyncCallback< List< DeslocamentoCampusDTO > > callback );
	void saveHorariosDisponiveis( CampusDTO campusDTO,
		List< HorarioDisponivelCenarioDTO > listDTO, AsyncCallback< Void > callback );
	void getResumos( CenarioDTO cenarioDTO, TreeNodeDTO treeNodeDTO, AsyncCallback< List< TreeNodeDTO > > callback );
	void saveDeslocamento( CenarioDTO cenario, List< DeslocamentoCampusDTO > list, AsyncCallback< Void > callback );
	void getHorariosDisponiveis( CampusDTO campusDTO,
		AsyncCallback< PagingLoadResult< HorarioDisponivelCenarioDTO > > callback );
	void getListByCurriculo( CenarioDTO cenarioDTO, CurriculoDTO curriculoDTO, AsyncCallback< ListLoadResult< CampusDTO > > callback );
	void getListAllCampiTodos( CenarioDTO cenarioDTO, AsyncCallback<ListLoadResult<CampusDTO>> callback );
	void getCenariosComparados(List<CenarioDTO> cenariosDTO, AsyncCallback<List<BaseTreeModel>> callback);
	void getCampusPorProfessor(ProfessorDTO professorDTO, AsyncCallback<ListLoadResult<CampusDTO>> callback);
}
