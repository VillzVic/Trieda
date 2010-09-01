package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.web.trieda.client.mvp.model.HorarioAulaDTO;
import com.gapso.web.trieda.client.services.HorariosAulaService;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class HorariosAulaServiceImpl extends RemoteServiceServlet implements HorariosAulaService {

	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public PagingLoadResult<HorarioAulaDTO> getList(PagingLoadConfig config) {
		List<HorarioAulaDTO> list = new ArrayList<HorarioAulaDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		for(HorarioAula horarioAula : HorarioAula.find(config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toHorarioAulaDTO(horarioAula));
		}
		BasePagingLoadResult<HorarioAulaDTO> result = new BasePagingLoadResult<HorarioAulaDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(HorarioAula.count());
		return result;
	}

	@Override
	public void save(HorarioAulaDTO horarioAulaDTO) {
		HorarioAula horarioDeAula = ConvertBeans.toHorarioAula(horarioAulaDTO);
		if(horarioDeAula.getId() != null && horarioDeAula.getId() > 0) {
			horarioDeAula.merge();
		} else {
			horarioDeAula.persist();
		}
	}
	
	@Override
	public void remove(List<HorarioAulaDTO> horariosAulaDTOList) {
		for(HorarioAulaDTO horarioAulaDTO : horariosAulaDTOList) {
			ConvertBeans.toHorarioAula(horarioAulaDTO).remove();
		}
	}

}
