package com.gapso.web.trieda.server;

import java.util.ArrayList;
import java.util.List;

import com.extjs.gxt.ui.client.Style.SortDir;
import com.extjs.gxt.ui.client.data.BasePagingLoadResult;
import com.extjs.gxt.ui.client.data.PagingLoadConfig;
import com.extjs.gxt.ui.client.data.PagingLoadResult;
import com.gapso.trieda.domain.AreaTitulacao;
import com.gapso.web.trieda.client.mvp.model.AreaTitulacaoDTO;
import com.gapso.web.trieda.client.services.AreasTitulacaoService;
import com.gapso.web.trieda.server.util.ConvertBeans;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

/**
 * The server side implementation of the RPC service.
 */
public class AreasTitulacaoServiceImpl extends RemoteServiceServlet implements AreasTitulacaoService {

	private static final long serialVersionUID = 5250776996542788849L;

	@Override
	public PagingLoadResult<AreaTitulacaoDTO> getBuscaList(String nome, String descricao, PagingLoadConfig config) {
		List<AreaTitulacaoDTO> list = new ArrayList<AreaTitulacaoDTO>();
		String orderBy = config.getSortField();
		if(orderBy != null) {
			if(config.getSortDir() != null && config.getSortDir().equals(SortDir.DESC)) {
				orderBy = orderBy + " asc";
			} else {
				orderBy = orderBy + " desc";
			}
		}
		for(AreaTitulacao areaTitulacao : AreaTitulacao.findByCodigoLikeAndDescricaoLike(nome, descricao, config.getOffset(), config.getLimit(), orderBy)) {
			list.add(ConvertBeans.toAreaTitulacaoDTO(areaTitulacao));
		}
		BasePagingLoadResult<AreaTitulacaoDTO> result = new BasePagingLoadResult<AreaTitulacaoDTO>(list);
		result.setOffset(config.getOffset());
		result.setTotalLength(AreaTitulacao.count());
		return result;
	}

	@Override
	public void save(AreaTitulacaoDTO areaTitulacaoDTO) {
		AreaTitulacao areaTitulacao = ConvertBeans.toAreaTitulacao(areaTitulacaoDTO);
		if(areaTitulacao.getId() != null && areaTitulacao.getId() > 0) {
			areaTitulacao.merge();
		} else {
			areaTitulacao.persist();
		}
	}
	
	@Override
	public void remove(List<AreaTitulacaoDTO> areaTitulacaoDTOList) {
		for(AreaTitulacaoDTO areaTitulacaoDTO : areaTitulacaoDTOList) {
			ConvertBeans.toAreaTitulacao(areaTitulacaoDTO).remove();
		}
	}

}
