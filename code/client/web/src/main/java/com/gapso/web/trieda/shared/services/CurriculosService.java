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
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath( "curriculos" )
public interface CurriculosService
	extends RemoteService {
	
	/**
	 * Associa uma disciplina (pertencente ao objeto CurriculoDisciplina) com uma matriz curricular. No entanto, caso a disciplina
	 * já tenha relação com outra matriz curricular, uma exceção é lançada.
	 * @param curriculoDTO matriz curricular
	 * @param curriculoDisciplinaDTO par [disciplina, matriz curricular]
	 * @throws TriedaException lança uma exceção caso a disciplina a ser associada com a matriz curricular já tenha associação com outra matriz curricular.
	 */
	void saveDisciplina(CurriculoDTO curriculoDTO, CurriculoDisciplinaDTO curriculoDisciplinaDTO) throws TriedaException;
	
	/**
	 * Salva no BD a matriz curricular.
	 * 
	 *    TRIEDA-1171: Não permitir a otimização se houver uma semana letiva sem turno e horário de aula associado, e houver matriz curricular associada a esta semana letiva.
	 *    
	 * @param curriculoDTO currículo (ou matriz curricular)
	 * @throws TriedaException lança uma exceção caso: 
	 *    - ocorra uma falha na operação de salvar a entidade no BD
	 *    - a semana letiva a ser associada com o currículo esteja vazia, isto é, sem turnos e horários de aula    
	 */
	void save(CurriculoDTO curriculoDTO) throws TriedaException;
	
	CurriculoDTO getCurriculo( Long id );
	ListLoadResult< CurriculoDTO > getList( CenarioDTO cenarioDTO, SemanaLetivaDTO semanaLetivaDTO, BasePagingLoadConfig loadConfig );
	PagingLoadResult< CurriculoDTO > getBuscaList( CenarioDTO cenarioDTO, CursoDTO cursoDTO, SemanaLetivaDTO semanaLetivaDTO,
		String codigo, String descricao, String periodo,   PagingLoadConfig config );
	void remove( List< CurriculoDTO > curriculoDTOList );
	ListLoadResult< CurriculoDisciplinaDTO > getDisciplinasList( CurriculoDTO curriculoDTO );
	void removeDisciplina( List< CurriculoDisciplinaDTO > curriculoDisciplinaDTOList );
	List< Integer > getPeriodos( CurriculoDTO curriculoDTO, CenarioDTO cenarioDTO );
	ListLoadResult< CurriculoDTO > getListByCurso( CursoDTO cursoDTO );
	ListLoadResult< CurriculoDTO > getListAll( CenarioDTO cenarioDTO );
	PagingLoadResult<DisciplinaRequisitoDTO> getDisciplinasPreRequisitosList( CenarioDTO cenarioDTO,
			DisciplinaDTO disciplinaDTO, CurriculoDTO curriculoDTO, Integer periodo, PagingLoadConfig config);
	PagingLoadResult<DisciplinaRequisitoDTO> getDisciplinasCoRequisitosList( CenarioDTO cenarioDTO,
			DisciplinaDTO disciplinaDTO, CurriculoDTO curriculoDTO, Integer periodo, PagingLoadConfig config);
	void saveDisciplinaPreRequisito(CenarioDTO cenarioDTO, DisciplinaRequisitoDTO disciplinaRequisitoDTO,
			DisciplinaDTO disciplinaDTO);
	void removeDisciplinasPreRequisitos(CenarioDTO cenarioDTO, List<DisciplinaRequisitoDTO> disciplinasRequisitosDTO);
	void saveDisciplinaCoRequisito(CenarioDTO cenarioDTO, DisciplinaRequisitoDTO disciplinaRequisitoDTO,
			DisciplinaDTO disciplinaDTO);
	void removeDisciplinasCoRequisitos(CenarioDTO cenarioDTO, List<DisciplinaRequisitoDTO> disciplinasRequisitosDTO);
	PagingLoadResult<AlunoDisciplinaCursadaDTO> getAlunosDisciplinasCursadasList( CenarioDTO cenarioDTO,
			DisciplinaDTO disciplinaDTO, CurriculoDTO curriculoDTO, 
			CursoDTO curso, String matricula, Integer periodo, PagingLoadConfig config );
	void saveAlunoDisciplinaCursada(CenarioDTO cenarioDTO, AlunoDTO alunoDTO,
			List<CurriculoDisciplinaDTO> curriculosDisciplinasDTO);
	void removeAlunosDisciplinasCursadas(CenarioDTO cenarioDTO,
			List<AlunoDisciplinaCursadaDTO> alunosDisciplinasCursadasDTO);
	ListLoadResult<CurriculoDisciplinaDTO> getAlunosDisciplinasNaoCursadasList(CenarioDTO cenarioDTO, AlunoDTO alunoDTO,
			CurriculoDTO curriculoDTO);
}
