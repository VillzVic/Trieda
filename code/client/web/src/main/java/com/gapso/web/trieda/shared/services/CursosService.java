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
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("cursos")
public interface CursosService extends RemoteService {

	/**
	 * Cadastra no BD um par de cursos que não permitem compartilhamento de turmas entre disciplinas
	 * @param dto par de cursos que não permitem compartilhamento de turmas entre disciplinas
	 * @throws TriedaException
	 */
	void insereBDProibicaoCompartilhamentoCursos(CursoDescompartilhaDTO dto) throws TriedaException;
	
	/**
	 * Remove do BD pares de cursos que não permitem compartilhamento de turmas entre disciplinas
	 * @param dtos pares de cursos que não permitem compartilhamento de turmas entre disciplinas
	 * @throws TriedaException
	 */
	void removeBDProibicaoCompartilhamentoCursos(List<CursoDescompartilhaDTO> dtos) throws TriedaException;
	
	/**
	 * Retorna uma lista de pares de cursos que não permitem compartilhamento de turmas entre disciplinas.
	 * @param dto o parametro que está associado com a lista de pares de cursos
	 * @return uma lista de pares de cursos que não permitem compartilhamento de turmas entre disciplinas
	 * @throws TriedaException
	 */
	List<CursoDescompartilhaDTO> getParesCursosQueNaoPermitemCompartilhamentoDeTurmas(ParametroDTO dto) throws TriedaException;
	
	CursoDTO getCurso(Long id);
	ListLoadResult<CursoDTO> getList(CenarioDTO cenarioDTO, BasePagingLoadConfig loadConfig);
	PagingLoadResult<CursoDTO> getBuscaList(CenarioDTO cenarioDTO, String nome, String codigo, TipoCursoDTO tipoCurso, PagingLoadConfig config);
	void save(CursoDTO cursoDTO);
	void remove(List<CursoDTO> cursoDTOList);
	ListLoadResult<CursoDTO> getListAll(CenarioDTO cenarioDTO);
	ListLoadResult<CursoDTO> getListByCampi(List<CampusDTO> campiDTOs, List<CursoDTO> retirarCursosDTO);
	List<ResumoCursoDTO> getResumos(CampusDTO campusDTO);
	
}
