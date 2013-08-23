package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface CurriculosServiceAsync {
	
	/**
	 * @see com.gapso.web.trieda.shared.services.CurriculosService#saveDisciplina(com.gapso.web.trieda.shared.dtos.CurriculoDTO, com.gapso.web.trieda.shared.dtos.CurriculoDisciplinaDTO)
	 */
	void saveDisciplina(CurriculoDTO curriculoDTO, CurriculoDisciplinaDTO curriculoDisciplinaDTO, AsyncCallback<Void> callback);
	
	void getCurriculo( Long id, AsyncCallback< CurriculoDTO > callback );
	void getList( CenarioDTO cenarioDTO, BasePagingLoadConfig loadConfig, AsyncCallback< ListLoadResult< CurriculoDTO > > callback );
	void getBuscaList( CenarioDTO cenarioDTO, CursoDTO cursoDTO, String codigo, String descricao,
		PagingLoadConfig config, AsyncCallback< PagingLoadResult< CurriculoDTO > > callback );
	void save( CurriculoDTO curriculoDTO, AsyncCallback< Void > callback );
	void remove( List< CurriculoDTO> curriculoDTOList, AsyncCallback< Void > callback );
	void getDisciplinasList( CurriculoDTO curriculoDTO, AsyncCallback< ListLoadResult< CurriculoDisciplinaDTO > > callback );
	void removeDisciplina( List< CurriculoDisciplinaDTO > curriculoDisciplinaDTOList, AsyncCallback< Void > callback );
	void getPeriodos( CurriculoDTO curriculoDTO, AsyncCallback< List< Integer > > callback );
	void getListByCurso( CursoDTO cursoDTO, AsyncCallback< ListLoadResult< CurriculoDTO > > callback );
	void getListAll(  CenarioDTO cenarioDTO, AsyncCallback< ListLoadResult< CurriculoDTO > > callback );
}
