package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
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
	public PagingLoadResult<TurnoDTO> getList(PagingLoadConfig config) {
		List<TurnoDTO> list = new ArrayList<TurnoDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		for(Turno turno : Turno.find(config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toTurnoDTO(turno));
		}
		BasePagingLoadResult<TurnoDTO> result = new BasePagingLoadResult<TurnoDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Turno.count());
		return result;
	}

	@Override
	public void save(TurnoDTO turnoDTO) {
		Turno turno = ConvertBeans.toTurno(turnoDTO);
		if(turno.getId() != null && turno.getId() > 0) {
			turno.merge();
		} else {
			turno.persist();
		}
	}
	
	@Override
	public void remove(List<TurnoDTO> turnoDTOList) {
		for(TurnoDTO turnoDTO : turnoDTOList) {
			ConvertBeans.toTurno(turnoDTO).remove();
		}
	}

}
