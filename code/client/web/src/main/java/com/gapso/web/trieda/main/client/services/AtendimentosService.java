package com.gapso.web.trieda.main.client.services;

import java.util.List;

import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.web.trieda.shared.dtos.AtendimentoOperacionalDTO;
import com.gapso.web.trieda.shared.dtos.AtendimentoTaticoDTO;
import com.gapso.web.trieda.shared.dtos.CurriculoDTO;
import com.gapso.web.trieda.shared.dtos.ParDTO;
import com.gapso.web.trieda.shared.dtos.ProfessorDTO;
import com.gapso.web.trieda.shared.dtos.SalaDTO;
import com.gapso.web.trieda.shared.dtos.TurnoDTO;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

/**
 * The client side stub for the RPC service.
 */
@RemoteServiceRelativePath("atendimentos")
public interface AtendimentosService extends RemoteService {
	
	PagingLoadResult<AtendimentoTaticoDTO> getList();
	List<AtendimentoTaticoDTO> getBusca(SalaDTO sala, TurnoDTO turno);
	ParDTO<List<AtendimentoTaticoDTO>, List<Integer>> getBusca(CurriculoDTO curriculoDTO, Integer periodo, TurnoDTO turnoDTO);
	List<AtendimentoOperacionalDTO> getAtendimentosOperacional(ProfessorDTO professorDTO, TurnoDTO turnoDTO);
	
}
