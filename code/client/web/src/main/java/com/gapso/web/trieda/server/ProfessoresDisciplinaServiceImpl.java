package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Disciplina;
import com.gapso.trieda.domain.Professor;
import com.gapso.trieda.domain.ProfessorDisciplina;
import com.gapso.web.trieda.client.mvp.model.DisciplinaDTO;
import com.gapso.web.trieda.client.mvp.model.ProfessorDTO;
import com.gapso.web.trieda.client.mvp.model.ProfessorDisciplinaDTO;
import com.gapso.web.trieda.client.services.ProfessoresDisciplinaService;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class ProfessoresDisciplinaServiceImpl extends RemoteServiceServlet implements ProfessoresDisciplinaService {

	private static final long serialVersionUID = -7562720011162342660L;

	@Override
	public ProfessorDisciplinaDTO getProfessorDisciplina(Long id) {
		return ConvertBeans.toProfessorDisciplinaDTO(ProfessorDisciplina.find(id));
	}
	
	@Override
	public PagingLoadResult<ProfessorDisciplinaDTO> getBuscaList(ProfessorDTO professorDTO, DisciplinaDTO disciplinaDTO, PagingLoadConfig config) {
		Professor professor = (professorDTO == null)? null : Professor.find(professorDTO.getId());
		Disciplina disciplina = (disciplinaDTO == null)? null : Disciplina.find(disciplinaDTO.getId());
		
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		
		List<ProfessorDisciplinaDTO> list = new ArrayList<ProfessorDisciplinaDTO>();
		for(ProfessorDisciplina professorDisciplina : ProfessorDisciplina.findBy(professor, disciplina, config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toProfessorDisciplinaDTO(professorDisciplina));
		}
		BasePagingLoadResult<ProfessorDisciplinaDTO> result = new BasePagingLoadResult<ProfessorDisciplinaDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(ProfessorDisciplina.count());
		return result;
	}

	@Override
	public void save(ProfessorDisciplinaDTO professorDisciplinaDTO) {
		ProfessorDisciplina professorDisciplina = ConvertBeans.toProfessorDisciplina(professorDisciplinaDTO);
		if(professorDisciplina.getId() != null && professorDisciplina.getId() > 0) {
			professorDisciplina.merge();
		} else {
			professorDisciplina.persist();
		}
	}
	
	@Override
	public void remove(List<ProfessorDisciplinaDTO> professorDisciplinaDTOList) {
		for(ProfessorDisciplinaDTO professorDisciplinaDTO : professorDisciplinaDTOList) {
			ConvertBeans.toProfessorDisciplina(professorDisciplinaDTO).remove();
		}
	}

}