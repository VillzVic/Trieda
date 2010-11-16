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
import com.google.gwt.user.client.rpc.AsyncCallback;

public interface DisciplinasServiceAsync {
	
	void getList(BasePagingLoadConfig loadConfig, AsyncCallback<ListLoadResult<DisciplinaDTO>> callback);
	void getBuscaList(String nome, String codigo, TipoDisciplinaDTO tipoDisciplinaDTO, PagingLoadConfig config, AsyncCallback<PagingLoadResult<DisciplinaDTO>> callback);
	void save(DisciplinaDTO disciplinaDTO, AsyncCallback<Void> callback);
	void remove(List<DisciplinaDTO> disciplinaDTOList, AsyncCallback<Void> callback);
	void getTipoDisciplinaList(AsyncCallback<ListLoadResult<TipoDisciplinaDTO>> callback);
	void getTipoDisciplina(Long id, AsyncCallback<TipoDisciplinaDTO> callback);
	void getListByCursos(List<CursoDTO> cursos, AsyncCallback<ListLoadResult<DisciplinaDTO>> callback);
	void getFolderChildren(FileModel loadConfig, AsyncCallback<List<FileModel>> callback);
	void getFolderChildrenSala(FileModel model, AsyncCallback<List<FileModel>> callback);
	void saveDisciplinaToSala(OfertaDTO ofertaDTO, Integer periodo, CurriculoDisciplinaDTO cdDTO, SalaDTO salaDTO, AsyncCallback<Void> callback);
	void saveDisciplinaToSala(OfertaDTO ofertaDTO, Integer periodo, CurriculoDisciplinaDTO cdDTO, GrupoSalaDTO grupoSalaDTO, AsyncCallback<Void> callback);
	void removeDisciplinaToSala(SalaDTO salaDTO, CurriculoDisciplinaDTO cdDTO, AsyncCallback<Void> callback);
	void removeDisciplinaToSala(GrupoSalaDTO grupoSalaDTO, CurriculoDisciplinaDTO cdDTO, AsyncCallback<Void> callback);
	
}
