package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.AtendimentoTatico;
import com.gapso.trieda.domain.Sala;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.client.mvp.model.AtendimentoTaticoDTO;
import com.gapso.web.trieda.client.mvp.model.SalaDTO;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.gapso.web.trieda.client.services.AtendimentosService;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class AtendimentosServiceImpl extends RemoteServiceServlet implements AtendimentosService {

	private static final long serialVersionUID = -1505176338607927637L;

	@Override
	public PagingLoadResult<AtendimentoTaticoDTO> getList() {
		List<AtendimentoTaticoDTO> list = new ArrayList<AtendimentoTaticoDTO>();
		List<AtendimentoTatico> atendimentosTatico = AtendimentoTatico.findAll();
		for(AtendimentoTatico atendimentoTatico : atendimentosTatico) {
			list.add(ConvertBeans.toAtendimentoTaticoDTO(atendimentoTatico));
		}
		BasePagingLoadResult<AtendimentoTaticoDTO> result = new BasePagingLoadResult<AtendimentoTaticoDTO>(list);
		result.setOffset(0);
		result.setTotalLength(100);
		return result;
	}

	@Override
	public List<AtendimentoTaticoDTO> getBusca(SalaDTO salaDTO, TurnoDTO turnoDTO) {
		Sala sala = Sala.find(salaDTO.getId());
		Turno turno = Turno.find(turnoDTO.getId());
		List<AtendimentoTaticoDTO> list = new ArrayList<AtendimentoTaticoDTO>();
		List<AtendimentoTatico> atendimentosTatico = AtendimentoTatico.findBySalaAndTurno(sala, turno);
		for(AtendimentoTatico atendimentoTatico : atendimentosTatico) {
			list.add(ConvertBeans.toAtendimentoTaticoDTO(atendimentoTatico));
		}
		return list;
	}
	
}
