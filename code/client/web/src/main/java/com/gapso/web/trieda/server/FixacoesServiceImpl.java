package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.Fixacao;
import com.gapso.web.trieda.client.mvp.model.FixacaoDTO;
import com.gapso.web.trieda.client.services.FixacoesService;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class FixacoesServiceImpl extends RemoteServiceServlet implements FixacoesService {

	@Override
	public FixacaoDTO getFixacao(Long id) {
		return ConvertBeans.toFixacaoDTO(Fixacao.find(id));
	}
	
	@Override
	public PagingLoadResult<FixacaoDTO> getBuscaList(String codigo, PagingLoadConfig config) {
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		List<FixacaoDTO> list = new ArrayList<FixacaoDTO>();
		for(Fixacao fixacao : Fixacao.findBy(codigo, config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toFixacaoDTO(fixacao));
		}
		BasePagingLoadResult<FixacaoDTO> result = new BasePagingLoadResult<FixacaoDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(Fixacao.count());
		return result;
	}
	
	@Override
	public void save(FixacaoDTO fixacaoDTO) {
		Fixacao fixacao = ConvertBeans.toFixacao(fixacaoDTO);
		if(fixacao.getId() != null && fixacao.getId() > 0) {
			fixacao.merge();
		} else {
			fixacao.persist();
		}
	}
	
	@Override
	public void remove(List<FixacaoDTO> fixacaoDTOList) {
		for(FixacaoDTO fixacaoDTO : fixacaoDTOList) {
			ConvertBeans.toFixacao(fixacaoDTO).remove();
		}
	}

}
