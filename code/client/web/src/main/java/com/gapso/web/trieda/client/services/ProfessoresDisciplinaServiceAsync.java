package com.gapso.web.trieda.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.client.mvp.model.DisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.ProfessorDTO;
import com.gapso.web.trieda.client.mvp.model.ProfessorDisciplinaDTO;
import com.google.gwt.user.client.rpc.AsyncCallback;


public interface ProfessoresDisciplinaServiceAsync {

	void getProfessorDisciplina(Long id, AsyncCallback<ProfessorDisciplinaDTO> callback);
	void getBuscaList(ProfessorDTO professorDTO, DisciplinaDTO disciplinaDTO, PagingLoadConfig config, AsyncCallback<PagingLoadResult<ProfessorDisciplinaDTO>> callback);
	void save(ProfessorDisciplinaDTO professorDisciplinaDTO, AsyncCallback<Void> callback);
	void remove(List<ProfessorDisciplinaDTO> professorDisciplinaDTOList, AsyncCallback<Void> callback);

}