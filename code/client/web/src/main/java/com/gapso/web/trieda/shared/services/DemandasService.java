package com.gapso.web.trieda.shared.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.CampusDTO;
import com.gapso.web.trieda.shared.dtos.CenarioDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.CursoDTO;
import com.gapso.web.trieda.shared.dtos.DemandaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDTO;
import com.gapso.web.trieda.shared.dtos.DisciplinaDemandaDTO;
import com.gapso.web.trieda.shared.dtos.ParametroGeracaoDemandaDTO;
import com.gapso.web.trieda.shared.dtos.QuintetoDTO;
import com.gapso.web.trieda.shared.dtos.ResumoMatriculaDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.gapso.web.trieda.shared.util.view.TriedaException;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("demandas")
public interface DemandasService extends RemoteService {

	PagingLoadResult<DemandaDTO> getBuscaList(CenarioDTO cenarioDTO, CampusDTO campusDTO, CursoDTO cursoDTO, 
			CurriculoDTO curriculoDTO, TurnoDTO turnoDTO, DisciplinaDTO disciplinaDTO, PagingLoadConfig config);
	void save(DemandaDTO demandaDTO, List<DisciplinaDemandaDTO> disciplinas, Integer periodo) throws TriedaException;
	void remove(List<DemandaDTO> demandaDTOList);
	Integer findPeriodo(DemandaDTO demandaDTO) throws TriedaException;
	ParametroGeracaoDemandaDTO getParametroGeracaoDemanda(CenarioDTO cenarioDTO);
	void calculaPrioridadesParaDisciplinasNaoCursadasPorAluno(CenarioDTO cenarioDTO,
			ParametroGeracaoDemandaDTO parametroGeracaoDemandaDTO);
	QuintetoDTO<CampusDTO, DemandaDTO, DisciplinaDTO, Integer, Integer> getDemandaDTO(CenarioDTO cenarioDTO,	ResumoMatriculaDTO resumoMatriculaDTO);
}
