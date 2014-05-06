package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.AlunoDTO;
import com.gapso.web.trieda.shared.dtos.AlunoDisciplinaCursadaDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaRequisitoDTO;
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
	void getPeriodos(CurriculoDTO curriculoDTO, CenarioDTO cenarioDTO,
			AsyncCallback<List<Integer>> callback);
	void getListByCurso( CursoDTO cursoDTO, AsyncCallback< ListLoadResult< CurriculoDTO > > callback );
	void getListAll(  CenarioDTO cenarioDTO, AsyncCallback< ListLoadResult< CurriculoDTO > > callback );
	void getDisciplinasPreRequisitosList( CenarioDTO cenarioDTO, DisciplinaDTO disciplinaDTO, CurriculoDTO curriculoDTO,
			Integer periodo, PagingLoadConfig config, AsyncCallback<PagingLoadResult<DisciplinaRequisitoDTO>> callback );
	void getDisciplinasCoRequisitosList( CenarioDTO cenarioDTO, DisciplinaDTO disciplinaDTO, CurriculoDTO curriculoDTO,
			Integer periodo, PagingLoadConfig config, AsyncCallback<PagingLoadResult<DisciplinaRequisitoDTO>> callback );
	void saveDisciplinaPreRequisito(CenarioDTO cenarioDTO, DisciplinaRequisitoDTO disciplinaRequisitoDTO,
			DisciplinaDTO disciplinaDTO, AsyncCallback<Void> callback);
	void removeDisciplinasPreRequisitos(CenarioDTO cenarioDTO, List<DisciplinaRequisitoDTO> disciplinasRequisitosDTO,
			AsyncCallback<Void> callback);
	void saveDisciplinaCoRequisito(CenarioDTO cenarioDTO, DisciplinaRequisitoDTO disciplinaRequisitoDTO,
			DisciplinaDTO disciplinaDTO, AsyncCallback<Void> callback);
	void removeDisciplinasCoRequisitos(CenarioDTO cenarioDTO, List<DisciplinaRequisitoDTO> disciplinasRequisitosDTO,
			AsyncCallback<Void> callback);
	void getAlunosDisciplinasCursadasList(CenarioDTO cenarioDTO, DisciplinaDTO disciplinaDTO, CurriculoDTO curriculoDTO,
			Integer periodo, PagingLoadConfig config, AsyncCallback<PagingLoadResult<AlunoDisciplinaCursadaDTO>> callback);
	void saveAlunoDisciplinaCursada(CenarioDTO cenarioDTO, AlunoDTO alunoDTO,
			List<CurriculoDisciplinaDTO> curriculosDisciplinasDTO, AsyncCallback<Void> callback);
	void removeAlunosDisciplinasCursadas(CenarioDTO cenarioDTO,	List<AlunoDisciplinaCursadaDTO> alunosDisciplinasCursadasDTO,
			AsyncCallback<Void> callback);
	void getAlunosDisciplinasNaoCursadasList(CenarioDTO cenarioDTO,	AlunoDTO alunoDTO, CurriculoDTO curriculoDTO,
			AsyncCallback<ListLoadResult<CurriculoDisciplinaDTO>> callback);
}
