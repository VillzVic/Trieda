package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDisciplinaDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("professoresDisciplina")
public interface ProfessoresDisciplinaService extends RemoteService {

	ProfessorDisciplinaDTO getProfessorDisciplina(Long id);
	PagingLoadResult<ProfessorDisciplinaDTO> getBuscaList(ProfessorDTO professorDTO, DisciplinaDTO disciplinaDTO, PagingLoadConfig config);
	void save(ProfessorDisciplinaDTO professorDisciplinaDTO);
	void remove(List<ProfessorDisciplinaDTO> professorDisciplinaDTOList);
	
}
