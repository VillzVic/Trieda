package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.CurriculoDisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.CursoDTO;
import com.gapso.web.trieda.client.mvp.model.DisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.FileModel;
import com.gapso.web.trieda.client.mvp.model.GrupoSalaDTO;
import com.gapso.web.trieda.client.mvp.model.OfertaDTO;
import com.gapso.web.trieda.client.mvp.model.SalaDTO;
import com.gapso.web.trieda.client.mvp.model.TipoDisciplinaDTO;
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
	List<FileModel> getFolderChildren(FileModel loadConfig);
	List<FileModel> getFolderChildrenSala(FileModel model);
	void saveDisciplinaToSala(OfertaDTO ofertaDTO, Integer periodo, CurriculoDisciplinaDTO cdDTO, SalaDTO salaDTO);
	void saveDisciplinaToSala(OfertaDTO ofertaDTO, Integer periodo, CurriculoDisciplinaDTO cdDTO, GrupoSalaDTO grupoSalaDTO);
	void removeDisciplinaToSala(SalaDTO salaDTO, CurriculoDisciplinaDTO cdDTO);
	void removeDisciplinaToSala(GrupoSalaDTO grupoSalaDTO, CurriculoDisciplinaDTO cdDTO);
	
}
