package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaIncompativelDTO;
import com.gapso.web.trieda.shared.dtos.DivisaoCreditoDTO;
import com.gapso.web.trieda.shared.dtos.GrupoSalaDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.OfertaDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.ResumoDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.SemanaLetivaDTO;
import com.gapso.web.trieda.shared.dtos.TipoDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.TreeNodeDTO;
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath( "disciplinas" )
public interface DisciplinasService
	extends RemoteService
{
	/**
	 * Retorna os horários disponíveis associados com a disciplina em questão. Quando uma disciplina estiver associada com matrizes
	 * curriculares, os horários retornados compreenderão aqueles que pertencem às semanas letivas associadas com as matrizes curriculares
	 * em questão. Caso a disciplina não esteja associada a nenhuma matriz curricular, retorna todos os horários disponíveis de todas as
	 * semanas letivas cadastradas.
	 * 
	 * TRIEDA-1154: Os "horarios disponiveis" de uma disciplina ja associada a alguma matriz curricular devem pertencer somente 'a semana letiva da matriz curricular correspondente.
	 * 
	 * @param disciplinaDTO disciplina
	 * @return lista de horários disponíveis associados com a disciplina em questão.
	 */
	List<HorarioDisponivelCenarioDTO> getHorariosDisponiveis(DisciplinaDTO disciplinaDTO);
	
	/**
	 * Retorna as semanas letivas associadas com a disciplina em questão. Uma semana letiva estará associada a uma disciplina caso a
	 * disciplina pertença a uma matriz curricular associada com a semana letiva em questão.
	 * 
	 * TRIEDA-1154: Os "horarios disponiveis" de uma disciplina ja associada a alguma matriz curricular devem pertencer somente 'a semana letiva da matriz curricular correspondente.
	 * 
	 * @param disciplinaDTO disciplina
	 * @return lista de semanas letivas associadas com a disciplina em questão.
	 */
	List<SemanaLetivaDTO> getSemanasLetivas(DisciplinaDTO disciplinaDTO);
	
	/**
	 * Para toda disciplina que:
	 *    - possue oferta
	 *    - exige o uso de laboratório
	 *    - e não está associada a nenhum laboratório
	 * associa a disciplina com todos os laboratórios do campus que tem relação com a oferta em questão.
	 *
	 * TRIEDA-1181: Funcionalidade para associar disciplinas que exigem laboratórios a todos os laboratórios automaticamente
	 * TRIEDA-1193: Na importação de Associação de Salas com Disciplinas, permitir que as colunas "Cod Curso", "Cod Curriculo" e "Período" estejam em branco
	 * 
	 * @throws TriedaException
	 */
	void associarDisciplinasSemLaboratorioATodosLaboratorios() throws TriedaException;
	
	DisciplinaDTO getDisciplina( Long id );
	ListLoadResult< DisciplinaDTO > getList( BasePagingLoadConfig loadConfig );
	PagingLoadResult<DisciplinaDTO> getBuscaList( String nome, String codigo,
		TipoDisciplinaDTO tipoDisciplinaDTO, PagingLoadConfig config );
	void save( DisciplinaDTO disciplinaDTO );
	void remove( List< DisciplinaDTO > disciplinaDTOList );
	ListLoadResult< TipoDisciplinaDTO > getTipoDisciplinaList();
	TipoDisciplinaDTO getTipoDisciplina( Long id );
	ListLoadResult< DisciplinaDTO > getListByCursoAndName( List< CursoDTO > cursos, String name);
	ListLoadResult< DisciplinaDTO > getListByCurriculoIdAndName( Long curriculoId, String name);
	ListLoadResult< DisciplinaDTO > getListByCursos( List< CursoDTO > cursos );
	ListLoadResult< DisciplinaDTO > getListByCurriculo(long curriculoId);
	List< TreeNodeDTO > getFolderChildren( TreeNodeDTO loadConfig );
	void saveDisciplinaToSala( OfertaDTO ofertaDTO, Integer periodo, CurriculoDisciplinaDTO cdDTO, SalaDTO salaDTO);
	void saveDisciplinaToSala( OfertaDTO ofertaDTO, Integer periodo, CurriculoDisciplinaDTO cdDTO, GrupoSalaDTO grupoSalaDTO);
	void removeDisciplinaToSala( SalaDTO salaDTO, CurriculoDisciplinaDTO cdDTO);
	void removeDisciplinaToSala( GrupoSalaDTO grupoSalaDTO, CurriculoDisciplinaDTO cdDTO);
	void saveHorariosDisponiveis( DisciplinaDTO disciplinaDTO, List< HorarioDisponivelCenarioDTO > listDTO );
	List< TreeNodeDTO > getDisciplinasByTreeSalas( TreeNodeDTO salaTreeNodeDTO,
		TreeNodeDTO ofertaTreeNodeDTO, TreeNodeDTO curriculoDisciplinaTreeNodeDTO );
	List< TreeNodeDTO > getDisciplinasByTreeGrupoSalas( TreeNodeDTO grupoSalaTreeNodeDTO,
		TreeNodeDTO ofertaTreeNodeDTO, TreeNodeDTO curriculoDisciplinaTreeNodeDTO );
	List< DisciplinaIncompativelDTO > getDisciplinasIncompativeis( CurriculoDTO curriculoDTO, Integer periodo );
	void saveDisciplinasIncompativeis( List< DisciplinaIncompativelDTO > list );
	void salvarDivisaoCredito( DisciplinaDTO disciplinaDTO, DivisaoCreditoDTO divisaoCreditoDTO );
	DivisaoCreditoDTO getDivisaoCredito( DisciplinaDTO disciplinaDTO );
	List< ResumoDisciplinaDTO > getResumos( CenarioDTO cenarioDTO, CampusDTO campusDTO );
	void removeDivisaoCredito( DisciplinaDTO disciplinaDTO );
	ListLoadResult<DisciplinaDTO> getDisciplinaNaoAssociada( ProfessorDTO professorDTO, String nome );
}