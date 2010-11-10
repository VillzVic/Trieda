package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import org.springframework.transaction.annotation.Transactional;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Cenario;
import com.gapso.web.trieda.client.mvp.model.CenarioDTO;
import com.gapso.web.trieda.client.services.CenariosService;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
@Transactional
public class CenariosServiceImpl extends RemoteServiceServlet implements CenariosService {

	private static final long serialVersionUID = -5951529933566541220L;

	@Override
	public CenarioDTO getMasterData() {
		return ConvertBeans.toCenarioDTO(Cenario.findMasterData());
	}

	@Override
	public CenarioDTO getCenario(Long id) {
		return ConvertBeans.toCenarioDTO(Cenario.find(id));
	}
	
	@Override
	public PagingLoadResult<CenarioDTO> getList(PagingLoadConfig config) {
		List<CenarioDTO> list = new ArrayList<CenarioDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		for(Cenario cenario : Cenario.find(config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toCenarioDTO(cenario));
		}
		BasePagingLoadResult<CenarioDTO> result = new BasePagingLoadResult<CenarioDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Cenario.count());
		return result;
	}

	@Override
	public PagingLoadResult<CenarioDTO> getBuscaList(Integer ano, Integer semestre, PagingLoadConfig config) {
		List<CenarioDTO> list = new ArrayList<CenarioDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		for(Cenario cenario : Cenario.findByAnoAndSemestre(ano, semestre, config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toCenarioDTO(cenario));
		}
		BasePagingLoadResult<CenarioDTO> result = new BasePagingLoadResult<CenarioDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Cenario.count());
		return result;
	}
		
	@Override
	public void save(CenarioDTO cenarioDTO) {
		Cenario cenario = ConvertBeans.toCenario(cenarioDTO);
		if(cenario.getId() != null && cenario.getId() > 0) {
			cenario.merge();
		} else {
			cenario.persist();
		}
	}
	
	@Override
	public void remove(List<CenarioDTO> cenarioDTOList) {
		for(CenarioDTO cenarioDTO : cenarioDTOList) {
			ConvertBeans.toCenario(cenarioDTO).remove();
		}
	}
	
}
