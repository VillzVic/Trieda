package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BaseListLoadResult;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.ListLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Campus;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.client.mvp.model.CampusDTO;
import com.gapso.web.trieda.client.services.CampiService;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class CampiServiceImpl extends RemoteServiceServlet implements CampiService {

	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public PagingLoadResult<CampusDTO> getList(PagingLoadConfig config) {
		List<CampusDTO> list = new ArrayList<CampusDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		for(Campus campus : Campus.find(config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toCampusDTO(campus));
		}
		BasePagingLoadResult<CampusDTO> result = new BasePagingLoadResult<CampusDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Campus.count());
		return result;
	}

	@Override
	public PagingLoadResult<CampusDTO> getBuscaList(String nome, String codigo, PagingLoadConfig config) {
		List<CampusDTO> list = new ArrayList<CampusDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		for(Campus campus : Campus.findByNomeLikeAndCodigoLike(nome, codigo, config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toCampusDTO(campus));
		}
		BasePagingLoadResult<CampusDTO> result = new BasePagingLoadResult<CampusDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Turno.count());
		return result;
	}
	
	@Override
	public ListLoadResult<CampusDTO> getList() {
		List<CampusDTO> list = new ArrayList<CampusDTO>();
		for(Campus campus : Campus.findAll()) {
			list.add(ConvertBeans.toCampusDTO(campus));
		}
		return new BaseListLoadResult<CampusDTO>(list);
	}
	
	@Override
	public void save(CampusDTO campusDTO) {
		Campus campus = ConvertBeans.toCampus(campusDTO);
		if(campus.getId() != null && campus.getId() > 0) {
			campus.merge();
		} else {
			campus.persist();
		}
	}
	
	@Override
	public void remove(List<CampusDTO> campusDTOList) {
		for(CampusDTO campusDTO : campusDTOList) {
			ConvertBeans.toCampus(campusDTO).remove();
		}
	}
	
}
