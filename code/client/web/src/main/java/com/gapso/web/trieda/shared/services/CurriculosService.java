package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
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
	ListLoadResult< CurriculoDTO > getList( BasePagingLoadConfig loadConfig );
	PagingLoadResult< CurriculoDTO > getBuscaList( CursoDTO cursoDTO,
		String codigo, String descricao, PagingLoadConfig config );
	void remove( List< CurriculoDTO > curriculoDTOList );
	ListLoadResult< CurriculoDisciplinaDTO > getDisciplinasList( CurriculoDTO curriculoDTO );
	void removeDisciplina( List< CurriculoDisciplinaDTO > curriculoDisciplinaDTOList );
	List< Integer > getPeriodos( CurriculoDTO curriculoDTO );
	ListLoadResult< CurriculoDTO > getListByCurso( CursoDTO cursoDTO );
	ListLoadResult< CurriculoDTO > getListAll();
}
