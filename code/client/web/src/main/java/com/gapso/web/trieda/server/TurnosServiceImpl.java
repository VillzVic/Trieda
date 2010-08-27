package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ModelData;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.client.mvp.model.TurnoDTO;
import com.gapso.web.trieda.client.mvp.model.TurnoModel;
import com.gapso.web.trieda.client.services.TurnosService;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class TurnosServiceImpl extends RemoteServiceServlet implements TurnosService {

	private static final long serialVersionUID = 5250776996542788849L;

	public PagingLoadResult<ModelData> getList() {
		List<ModelData> list = new ArrayList<ModelData>();
		for(Turno turno : Turno.findAllTurnoes()) {
			list.add(new TurnoModel(turno.getNome(), turno.getTempo()));
		}
		return new BasePagingLoadResult<ModelData>(list);
	}

	@Override
	public boolean save(TurnoDTO turnoDTO) {
		Turno turno = new Turno();
		turno.setNome(turnoDTO.getNome());
		turno.setTempo(turnoDTO.getTempo());
		turno.persist();
		return true;
	}

}
