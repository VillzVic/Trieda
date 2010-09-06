package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadConfig;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Calendario;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.client.mvp.model.CalendarioDTO;
import com.gapso.web.trieda.client.services.CalendariosService;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class CalendariosServiceImpl extends RemoteServiceServlet implements CalendariosService {

	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public PagingLoadResult<CalendarioDTO> getList(PagingLoadConfig config) {
		
		List<CalendarioDTO> list = new ArrayList<CalendarioDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		
		for(Calendario calendario : Calendario.find(config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toCalendarioDTO(calendario));
		}
		
		BasePagingLoadResult<CalendarioDTO> result = new BasePagingLoadResult<CalendarioDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Calendario.count());
		return result;
	}
	
	@Override
	public ListLoadResult<CalendarioDTO> getList(BasePagingLoadConfig loadConfig) {
		return getBuscaList(loadConfig.get("query").toString(), loadConfig);
	}
	
	@Override
	public PagingLoadResult<CalendarioDTO> getBuscaList(String codigo, PagingLoadConfig config) {
		List<CalendarioDTO> list = new ArrayList<CalendarioDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		for(Calendario calendario : Calendario.findByCodigoLike(codigo, config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toCalendarioDTO(calendario));
		}
		BasePagingLoadResult<CalendarioDTO> result = new BasePagingLoadResult<CalendarioDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Turno.count());
		return result;
	}
	
	@Override
	public ListLoadResult<CalendarioDTO> getList() {
		List<CalendarioDTO> list = new ArrayList<CalendarioDTO>();
		for(Calendario calendario : Calendario.findAll()) {
			list.add(ConvertBeans.toCalendarioDTO(calendario));
		}
		return new BaseListLoadResult<CalendarioDTO>(list);
	}

	@Override
	public void save(CalendarioDTO calendarioDTO) {
		Calendario calendario = ConvertBeans.toCalendario(calendarioDTO);
		if(calendario.getId() != null && calendario.getId() > 0) {
			calendario.merge();
		} else {
			calendario.persist();
		}
	}
	
	@Override
	public void remove(List<CalendarioDTO> calendarioDTOList) {
		for(CalendarioDTO calendarioDTO : calendarioDTOList) {
			ConvertBeans.toCalendario(calendarioDTO).remove();
		}
	}
	
}
