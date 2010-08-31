package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.HorarioAula;
import com.gapso.web.trieda.client.mvp.model.PeriodoDeAulaDTO;
import com.gapso.web.trieda.client.services.PeriodosDeAulaService;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class PeriodosDeAulaServiceImpl extends RemoteServiceServlet implements PeriodosDeAulaService {

	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public PagingLoadResult<PeriodoDeAulaDTO> getList(PagingLoadConfig config) {
		List<PeriodoDeAulaDTO> list = new ArrayList<PeriodoDeAulaDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		for(HorarioAula horarioAula : HorarioAula.find(config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toPeriodoDeAulaDTO(horarioAula));
		}
		BasePagingLoadResult<PeriodoDeAulaDTO> result = new BasePagingLoadResult<PeriodoDeAulaDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(HorarioAula.count());
		return result;
	}

	@Override
	public void save(PeriodoDeAulaDTO periodoDeAulaDTO) {
		HorarioAula horarioDeAula = ConvertBeans.toHorarioAula(periodoDeAulaDTO);
		if(horarioDeAula.getId() != null && horarioDeAula.getId() > 0) {
			horarioDeAula.merge();
		} else {
			horarioDeAula.persist();
		}
	}
	
	@Override
	public void remove(List<PeriodoDeAulaDTO> periodoDeAulaDTOList) {
		for(PeriodoDeAulaDTO periodoDeAulaDTO : periodoDeAulaDTOList) {
			ConvertBeans.toHorarioAula(periodoDeAulaDTO).remove();
		}
	}

}
