package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaIncompativelDTO;
import com.gapso.web.trieda.shared.dtos.DivisaoCreditoDTO;
import com.gapso.web.trieda.shared.dtos.GrupoSalaDTO;
import com.gapso.web.trieda.shared.dtos.HorarioDisponivelCenarioDTO;
import com.gapso.web.trieda.shared.dtos.OfertaDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.TipoDisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.TreeNodeDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("disciplinas")
public interface DisciplinasService extends RemoteService {

	DisciplinaDTO getDisciplina(Long id);
	ListLoadResult<DisciplinaDTO> getList(BasePagingLoadConfig loadConfig);
	PagingLoadResult<DisciplinaDTO> getBuscaList(String nome, String codigo, TipoDisciplinaDTO tipoDisciplinaDTO, PagingLoadConfig config);
	void save(DisciplinaDTO disciplinaDTO);
	void remove(List<DisciplinaDTO> disciplinaDTOList);
	ListLoadResult<TipoDisciplinaDTO> getTipoDisciplinaList();
	TipoDisciplinaDTO getTipoDisciplina(Long id);
	ListLoadResult<DisciplinaDTO> getListByCursos(List<CursoDTO> cursos);
	List<TreeNodeDTO> getFolderChildren(TreeNodeDTO loadConfig);
	void saveDisciplinaToSala(OfertaDTO ofertaDTO, Integer periodo, CurriculoDisciplinaDTO cdDTO, SalaDTO salaDTO);
	void saveDisciplinaToSala(OfertaDTO ofertaDTO, Integer periodo, CurriculoDisciplinaDTO cdDTO, GrupoSalaDTO grupoSalaDTO);
	void removeDisciplinaToSala(SalaDTO salaDTO, CurriculoDisciplinaDTO cdDTO);
	void removeDisciplinaToSala(GrupoSalaDTO grupoSalaDTO, CurriculoDisciplinaDTO cdDTO);
	void saveHorariosDisponiveis(DisciplinaDTO disciplinaDTO,List<HorarioDisponivelCenarioDTO> listDTO);
	List<HorarioDisponivelCenarioDTO> getHorariosDisponiveis(DisciplinaDTO disciplinaDTO);
	List<TreeNodeDTO> getDisciplinasByTreeSalas(TreeNodeDTO salaTreeNodeDTO, TreeNodeDTO ofertaTreeNodeDTO, TreeNodeDTO curriculoDisciplinaTreeNodeDTO);
	List<TreeNodeDTO> getDisciplinasByTreeGrupoSalas(TreeNodeDTO grupoSalaTreeNodeDTO, TreeNodeDTO ofertaTreeNodeDTO, TreeNodeDTO curriculoDisciplinaTreeNodeDTO);
	List<DisciplinaIncompativelDTO> getDisciplinasIncompativeis(CurriculoDTO curriculoDTO, Integer periodo);
	void saveDisciplinasIncompativeis(List<DisciplinaIncompativelDTO> list);
	void salvarDivisaoCredito(DisciplinaDTO disciplinaDTO, DivisaoCreditoDTO divisaoCreditoDTO);
	DivisaoCreditoDTO getDivisaoCredito(DisciplinaDTO disciplinaDTO);
	
}
