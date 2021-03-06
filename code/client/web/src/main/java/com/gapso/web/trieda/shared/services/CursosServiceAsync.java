package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDescompartilhaDTO;
import com.gapso.web.trieda.shared.dtos.ParametroDTO;
import com.gapso.web.trieda.shared.dtos.ResumoCursoDTO;
import com.gapso.web.trieda.shared.dtos.TipoCursoDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface CursosServiceAsync {
	
	/**
	 * @see com.gapso.web.trieda.shared.services.CursosService#insereBDProibicaoCompartilhamentoCursos(com.gapso.web.trieda.shared.dtos.CursoDescompartilhaDTO)
	 */
	void insereBDProibicaoCompartilhamentoCursos(CursoDescompartilhaDTO dto, AsyncCallback<Void> callback);
	
	/**
	 * @see com.gapso.web.trieda.shared.services.CursosService#removeBDProibicaoCompartilhamentoCursos(java.util.List)
	 */
	void removeBDProibicaoCompartilhamentoCursos(List<CursoDescompartilhaDTO> dtos, AsyncCallback<Void> callback);
	
	/**
	 * @see com.gapso.web.trieda.shared.services.CursosService#getParesCursosQueNaoPermitemCompartilhamentoDeTurmas()
	 */
	void getParesCursosQueNaoPermitemCompartilhamentoDeTurmas(ParametroDTO dto, AsyncCallback<List<CursoDescompartilhaDTO>> callback);
	
	void getCurso( Long id, AsyncCallback< CursoDTO > callback );
	void getList( CenarioDTO cenarioDTO, BasePagingLoadConfig loadConfig, AsyncCallback< ListLoadResult< CursoDTO > > callback );
	void getBuscaList( CenarioDTO cenarioDTO, String nome, String codigo, TipoCursoDTO tipoCurso,
			String operadorMinPercentualDoutor, Integer minPercentualDoutor,
			String operadorMinPercentualMestre, Integer minPercentualMestre,
			String operadorMinTempoIntegralParcial, Integer minTempoIntegralParcial,
			String operadorMinTempoIntegral, Integer minTempoIntegral,
			String operadorMaxDisciplinasProfessor, Integer maxDisciplinasProfessor,
			Boolean maisDeUmaDisciplinaProfessor,
		PagingLoadConfig config, AsyncCallback< PagingLoadResult< CursoDTO > > callback );
	void save( CursoDTO cursoDTO, AsyncCallback< Void > callback );
	void remove( List< CursoDTO > cursoDTOList, AsyncCallback< Void > callback );
	void getListAll( CenarioDTO cenarioDTO, AsyncCallback< ListLoadResult< CursoDTO > > callback );
	void getListByCampi( List<CampusDTO> campiDTOs, List< CursoDTO > retirarCursosDTO,
		AsyncCallback< ListLoadResult< CursoDTO > > callback );
	void getResumos( CampusDTO campusDTO, AsyncCallback< List< ResumoCursoDTO > > callback );
}
