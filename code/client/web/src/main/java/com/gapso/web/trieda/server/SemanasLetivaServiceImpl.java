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
import com.gapso.trieda.domain.SemanaLetiva;
import com.gapso.trieda.domain.Turno;
import com.gapso.web.trieda.client.mvp.model.SemanaLetivaDTO;
import com.gapso.web.trieda.client.services.SemanasLetivaService;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class SemanasLetivaServiceImpl extends RemoteServiceServlet implements SemanasLetivaService {

	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public PagingLoadResult<SemanaLetivaDTO> getList(PagingLoadConfig config) {
		
		List<SemanaLetivaDTO> list = new ArrayList<SemanaLetivaDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		
		for(SemanaLetiva semanaLetiva : SemanaLetiva.find(config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toSemanaLetivaDTO(semanaLetiva));
		}
		
		BasePagingLoadResult<SemanaLetivaDTO> result = new BasePagingLoadResult<SemanaLetivaDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(SemanaLetiva.count());
		return result;
	}
	
	@Override
	public ListLoadResult<SemanaLetivaDTO> getList(BasePagingLoadConfig loadConfig) {
		return getBuscaList(loadConfig.get("query").toString(), null, loadConfig);
	}
	
	@Override
	public PagingLoadResult<SemanaLetivaDTO> getBuscaList(String codigo, String descricao, PagingLoadConfig config) {
		List<SemanaLetivaDTO> list = new ArrayList<SemanaLetivaDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		for(SemanaLetiva semanaLetiva : SemanaLetiva.findByCodigoLikeAndDescricaoLike(codigo, descricao, config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toSemanaLetivaDTO(semanaLetiva));
		}
		BasePagingLoadResult<SemanaLetivaDTO> result = new BasePagingLoadResult<SemanaLetivaDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Turno.count());
		return result;
	}
	
	@Override
	public ListLoadResult<SemanaLetivaDTO> getList() {
		List<SemanaLetivaDTO> list = new ArrayList<SemanaLetivaDTO>();
		for(SemanaLetiva semanaLetiva : SemanaLetiva.findAll()) {
			list.add(ConvertBeans.toSemanaLetivaDTO(semanaLetiva));
		}
		return new BaseListLoadResult<SemanaLetivaDTO>(list);
	}

	@Override
	public void save(SemanaLetivaDTO semanaLetivaDTO) {
		SemanaLetiva semanaLetiva = ConvertBeans.toSemanaLetiva(semanaLetivaDTO);
		if(semanaLetiva.getId() != null && semanaLetiva.getId() > 0) {
			semanaLetiva.merge();
		} else {
			semanaLetiva.persist();
		}
	}
	
	@Override
	public void remove(List<SemanaLetivaDTO> semanaLetivaDTOList) {
		for(SemanaLetivaDTO semanaLetivaDTO : semanaLetivaDTOList) {
			ConvertBeans.toSemanaLetiva(semanaLetivaDTO).remove();
		}
	}
	
}
