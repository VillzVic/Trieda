package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.gapso.web.trieda.client.services.TurnosService;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class TurnosServiceImpl extends RemoteServiceServlet implements TurnosService {

	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public PagingLoadResult<TurnoDTO> getList() {
		List<TurnoDTO> list = new ArrayList<TurnoDTO>();
		for(Turno turno : Turno.findAllTurnoes()) {
			list.add(ConvertBeans.toTurnoDTO(turno));
		}
		return new BasePagingLoadResult<TurnoDTO>(list);
	}

	@Override
	public boolean save(TurnoDTO turnoDTO) {
		Turno turno = ConvertBeans.toTurno(turnoDTO);
		if(turno.getId() != null && turno.getId() > 0) {
			turno.merge();
		} else {
			turno.persist();
		}
		return true;
	}
	
	@Override
	public boolean remove(List<TurnoDTO> turnoDTOList) {
		for(TurnoDTO turnoDTO : turnoDTOList) {
			ConvertBeans.toTurno(turnoDTO).remove();
		}
		return true;
	}

}
